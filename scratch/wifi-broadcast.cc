#include <iostream>
#include <string>
#include "ns3/core-module.h"
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


void ReceivePacket (Ptr<Socket> socket) {
    NS_LOG_UNCOND ("Received one packet!");
    std::cout << "Received packet\n";
    Ptr< Packet > pp = socket->Recv();
    //pp->m_buffer;
    //Packet *p =socket->Recv();
    unsigned char *data = new unsigned char(pp->GetSize());
    //uint8_t *buffer = new uint8_t(p->GetSize ());
    pp->CopyData (data, pp->GetSize());
    //const uint8_t *data = pp->PeekData();

    std::cout<<"Data "  << data <<" \n";

  }

  void accept(Ptr<Socket> socket,const ns3::Address& from){

      std::cout<<"Connection accepted"<< std::endl;
      socket->SetRecvCallback (MakeCallback (&ReceivePacket));
  }


//Returns the position of a certain node given in input
static Vector GetPosition (Ptr<Node> node) {
  Ptr<MobilityModel> mobility = node->GetObject<MobilityModel> ();
  return mobility->GetPosition();
}



static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, uint32_t pktCount, Time pktInterval ) {
  if (pktCount > 0) {
	  uint8_t *buffer = new uint8_t(pktSize);
	  //strcpy((char*)line_buf,"Hello World");
	  buffer = (uint8_t *)"Finalmente!\n";
	  int nodeID=0;
	  std::cout << "Node "<< nodeID << " sending packet whose content is: "<< buffer<< " \n";
	  Packet *p = new Packet(buffer, pktSize);
	  socket->Bind();
      socket->Send (p);
      Simulator::Schedule (pktInterval, &GenerateTraffic,
                           socket, pktSize, pktCount-1, pktInterval);
  }
  else {
      socket->Close ();
  }
}

int main (int argc, char *argv[]) {
  bool verbose = true; //log if true
  uint32_t nWifi = 3; //number of wifi devices
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
  std::cout <<"\nCreating Applications\n";
  
  uint16_t port = 80;   // Discard port is 9 (RFC 863)
  Ipv4Address remoteAddr = "255.255.255.255"; // send to the broadcast address
  
  //setting protocol type (UDP) and address (broadcast)
  //http://www.nsnam.org/docs/release/3.10/doxygen/classns3_1_1_on_off_application.html
  OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (remoteAddr, port)));

  //name: the name of the application attribute to set, value: the value of the application attribute to set 
  //onoff.SetAttribute ("OnTime", RandomVariableValue(ConstantVariable (10))); //also this part should be changed
  onoff.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=100]")); 
  onoff.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

  // set Node 0 to broadcast
    ApplicationContainer apps;
    //Ptr<Node> appSource = NodeList::GetNode (0);
    //std::cout << "Node 0 is now sending cbr packets.\n";
    //apps = onoff.Install (appSource);
    //apps.Start (Seconds (rndStartValue.GetValue()));
    //apps.Stop (Seconds (10.0));

// Create a packet sink to receive these packets on each of the Nodes
// Output does not change if the sink is not installed

  PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), port));

  //PacketSocketHelper packetSocket;

  for (uint32_t nNodes = 0; nNodes < (nWifi); nNodes++) {
    std::cout << "Node " << nNodes << " is a sink." << std::endl;
    Ptr<Node> appSink = NodeList::GetNode (nNodes);
    apps = sink.Install (appSink);
   //apps.Start (Seconds (0.0));
    //apps.Stop(Seconds(100.0));
    //Try try try
    //packetSocket.Install (NodeList::GetNode (nNodes));
    uint16_t beaconPort = 80;
    TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
    Ptr<Socket> beacon_sink = Socket::CreateSocket(wifiStaNodes.Get(nNodes),tid);
    InetSocketAddress beacon_local = InetSocketAddress(Ipv4Address::GetAny(), beaconPort);
    beacon_sink->Bind(beacon_local);
    //beacon_sink->SetRecvCallback(MakeCallback(&ReceivePacket));
    beacon_sink->SetRecvCallback (MakeCallback (&ReceivePacket));
    beacon_sink->Listen();
  }
  apps.Start(Seconds (0.0));
  //////////////////MINE//////////////////////

    //uint16_t beaconPort = 80;
    uint32_t packetSize = 1000; // bytes
    uint32_t numPackets = 1;
    double interval = 1.0;
    Time interPacketInterval = Seconds (interval);
    TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      static const InetSocketAddress beaconBroadcast = InetSocketAddress(Ipv4Address("255.255.255.255"));

      for (int nodeID = 0; nodeID < (int) nWifi; nodeID++){
    	  /*
    	  Ptr<Socket> beacon_sink = Socket::CreateSocket(wifiStaNodes.Get(nodeID),tid);
    	  InetSocketAddress beacon_local = InetSocketAddress(Ipv4Address::GetAny(), beaconPort);
    	  beacon_sink->Bind(beacon_local);
    	  beacon_sink->Listen();
    	  beacon_sink->SetRecvCallback(MakeCallback(&ReceivePacket));*/
    	  //beacon_sink->Bind();
    	  //beacon_sink->Listen();
    	  //beacon_sink->SetAcceptCallback (MakeNullCallback<bool, Ptr<Socket>,const Address &> (),MakeCallback(&ReceivePacket));

    	  Ptr<Socket> beacon_source = Socket::CreateSocket(wifiStaNodes.Get(nodeID), tid);
    	  beacon_source->SetAllowBroadcast(true);
    	  beacon_source->Connect(beaconBroadcast);
		  //ns3::Ptr<ns3::Node> appSource;
    	  //appSource = NodeList::GetNode (nodeID);
    	  //apps = onoff.Install (beacon_source);
    	  //apps.Start (Seconds (rndStartValue.GetValue()));
    	  //apps.Stop (Seconds (10.0));
    	  Simulator::ScheduleWithContext(nodeID, Seconds(1.0), &GenerateTraffic,
                                      beacon_source, packetSize, numPackets, interPacketInterval);
      }

    /////////////////END MINE//////////////////

    // set Node 0 to broadcast

     // std::cout << "Node 0 is now sending cbr packets.\n";
     //apps = onoff.Install (appSource);
          //apps.Start (Seconds (rndStartValue.GetValue()));
          //apps.Stop (Seconds (10.0));
  //This line has been commented since we don't want to enable packets to be routed
  //Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

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
