#include <iostream>
#include <string>
//#include "ns3/core-module.h"
#include ""
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/adhoc-wifi-mac.h"

// Default Network Topology
//
//   Wifi 10.1.3.0
//
//  *    *    *    * ... *
//  |    |    |    |     |
// n0   n1   n2   n3   nWifi-1
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("ThirdScriptExampleWifiOnly");

//Returns the position of a certain node given in input
static Vector GetPosition (Ptr<Node> node) {
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition();
}

int main (int argc, char *argv[]) {
  bool verbose = true; //log if true
  bool twoBroadcast = false; //understand me and change me
  uint32_t nWifi = 10; //number of wifi devices
  double latDistance = 50; //distance between nodes
  double rndStart = 0.000016; //dunno, need to understand (may be roundstart)
  UniformVariable rndStartValue(0.0, rndStart);

  
  Config::SetDefault ("ns3::OnOffApplication::PacketSize", StringValue ("500"));
  Config::SetDefault ("ns3::OnOffApplication::DataRate", StringValue ("6000kb/s"));

  CommandLine cmd;
  //use --<argument>=0 for false when <argument> is boolean
  cmd.AddValue ("nWifi", "Number of wifi adhoc devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("latDistance", "Distance between nodes, in meters", latDistance);
  cmd.AddValue ("twoBroadcast", "Sets a second node to broadcast if true", twoBroadcast);

  cmd.Parse (argc,argv);

  if (verbose){
      LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
  }

  //Creating a container for the nWifi nodes
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);

  //Creating the wifi devices and the interconnection channel between these wifi nodes
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiHelper wifi = WifiHelper::Default ();
  phy.Set ("EnergyDetectionThreshold", DoubleValue (-100.0) );
  phy.Set ("CcaMode1Threshold", DoubleValue (-90.0) );
  phy.Set ("TxGain", DoubleValue (5.0) );
  // I used higher gain to extend range to about 300 meters
  phy.Set ("RxGain", DoubleValue (5.0) );
  phy.Set ("TxPowerLevels", UintegerValue (1) );
  phy.Set ("TxPowerEnd", DoubleValue (16.0206) );
  phy.Set ("TxPowerStart", DoubleValue (16.0206) );
  phy.Set ("RxNoiseFigure", DoubleValue (3) );

  wifi.SetStandard(WIFI_PHY_STANDARD_80211a);
  //The SetRemoteStationManager method tells the helper the type of rate control algorithm to use. 
  //I should change this part
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager", "DataMode", StringValue ("OfdmRate6Mbps"));

  NqosWifiMacHelper mac = NqosWifiMacHelper::Default ();
  mac.SetType ("ns3::AdhocWifiMac", "Slot", StringValue ("16us"));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);
  
  //Allocating nodes
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc =  CreateObject<ListPositionAllocator> ();
  for (uint32_t x = 0; x < nWifi; x++)   {
        positionAlloc->Add (Vector ((x*latDistance), 0.0, 0.0));
  }
  mobility.SetPositionAllocator (positionAlloc);

  // Nodes are stationary, enabling them to move followinf the constant position mobility model
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiStaNodes);
	std::cout <<"Current location for the nodes:\n";
	std::cout <<"\t Node x y\n";
	std::cout <<"\t__________\n";
    // print current location of the nodes
    for (uint32_t i = 0; i < nWifi; ++i)  {
      Vector pos = GetPosition(wifiStaNodes.Get(i));
      std::cout << "\t  " << i << "  " << pos.x << " " << pos.y ;
      std::cout<< " \n";
    }
  InternetStackHelper stack;
  stack.Install (wifiStaNodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.3.0", "255.255.255.0");
  address.Assign (staDevices);

  NS_LOG_INFO ("Create Applications.");
  std::cout <<"Creating Applications\n";
  
  uint16_t port = 9;   // Discard port is 9 (RFC 863)
  Ipv4Address remoteAddr = "255.255.255.255"; // send to the broadcast address
  
  //setting protocol type (UDP) and address (broadcast)
  //http://www.nsnam.org/docs/release/3.10/doxygen/classns3_1_1_on_off_application.html
  OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (remoteAddr, port))); 
  std::cout <<"Setting ontime\n";
  //name: the name of the application attribute to set, value: the value of the application attribute to set 
  //onoff.SetAttribute ("OnTime", RandomVariableValue(ConstantVariable (10))); //also this part should be changed
  onoff.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=100]")); 
  std::cout <<"Setting offtime\n";
  onoff.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

  // set Node 0 to broadcast
    ApplicationContainer apps;
    Ptr<Node> appSource = NodeList::GetNode (0);
    std::cout << "Node 0 is now sending cbr packets.\n";
    apps = onoff.Install (appSource);
    apps.Start (Seconds (rndStartValue.GetValue()));
    apps.Stop (Seconds (10.0));

 // set furthest node to broadcast also
  if (twoBroadcast == true) {
	std::cout << "Node "<<nWifi-1 << " is now sending cbr packets.\n";

    Ptr<Node> appSource = NodeList::GetNode (nWifi-1);
    apps = onoff.Install (appSource);
    apps.Start (Seconds (rndStartValue.GetValue()));//Starts after
    apps.Stop (Seconds (10.0));//Executes for
  }

// Create a packet sink to receive these packets on each of the middle Nodes
// Node (0) and Node (nWifi-1) do not have a sink installed
// Output does not change if the sink is not installed
  PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));
  for (uint32_t nNodes = 1; nNodes < (nWifi-1); nNodes++) {
    std::cout << "Node " << nNodes << " is a sink." << std::endl;
    Ptr<Node> appSink = NodeList::GetNode (nNodes);
    apps = sink.Install (appSink);
    apps.Start (Seconds (0.0));
  }

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // Tracing configuration
  NS_LOG_INFO ("Configure Tracing.");
  // std::ofstream ascii;
  // ascii.open ("third_adhoc.txt");
   AsciiTraceHelper wifiAscii;
   Ptr<OutputStreamWrapper> wifiStream = wifiAscii.CreateFileStream ("gpf-wifi-ascii.tr");
   phy.EnableAsciiAll (wifiStream);

//  AsciiTraceHelperForIpv4 internetAscii;
//  Ptr<OutputStreamWrapper> internetStream = internetAscii.CreateFileStream ("gpf-internet-ascii.tr");
  stack.EnableAsciiIpv4All ("internetStream");
// YansWifiPhyHelper::EnableAsciiAll ("asciiPhy");
//  InternetStackHelper::EnableAsciiAll (ascii);
  phy.EnablePcapAll ("third_adhoc");

  Simulator::Stop (Seconds (10.0));

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
} 
