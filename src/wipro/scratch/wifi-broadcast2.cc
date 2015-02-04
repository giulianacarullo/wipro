#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

NS_LOG_COMPONENT_DEFINE ("WifiExperiment1");

using namespace ns3;

void ReceivePacket (Ptr<Socket> socket)
{
  NS_LOG_UNCOND ("Received one packet!");
  std::cout << "REceived packaet\n";
}

static void GenerateTraffic (Ptr<Socket> socket, uint32_t pktSize, 
                             uint32_t pktCount, Time pktInterval )
{
  if (pktCount > 0)
    {
      socket->Send (Create<Packet> (pktSize));
      Simulator::Schedule (pktInterval, &GenerateTraffic, 
                           socket, pktSize,pktCount-1, pktInterval);
    }
  else
    {
      socket->Close ();
    }
}


int main (int argc, char *argv[])
{
  std::string phyMode ("DsssRate1Mbps");
  double distance = 50; // m
  uint32_t numNodes = 25;
  uint32_t packetSize = 1000; // bytes
  uint32_t numPackets = 1;
  uint16_t beaconPort = 80;
  double interval = 1.0; // seconds
  bool verbose = false;
  //bool tracing = false;

  CommandLine cmd;

  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("packetSize", "size of application packet sent", packetSize);
  cmd.AddValue ("numPackets", "number of packets generated", numPackets);
  cmd.AddValue ("interval", "interval (seconds) between packets", interval);
  cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);
  cmd.AddValue ("beaconPort", "port number to send beacons on", verbose);

  cmd.Parse (argc, argv);
  // Convert to time object
  Time interPacketInterval = Seconds (interval);

  // disable fragmentation for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::FragmentationThreshold", StringValue ("2200"));
  // turn off RTS/CTS for frames below 2200 bytes
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue ("2200"));
  // Fix non-unicast data rate to be the same as that of unicast
  Config::SetDefault ("ns3::WifiRemoteStationManager::NonUnicastMode", 
                      StringValue (phyMode));

  NodeContainer source_nodes;
  source_nodes.Create (numNodes);

  NodeContainer sink_nodes;
  sink_nodes.Add(source_nodes);
  //sink_nodes.Create(numNodes);

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  if (verbose)
    {
      wifi.EnableLogComponents ();  // Turn on all Wifi logging
    }
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  // This is one parameter that matters when using FixedRssLossModel
  // set it to zero; otherwise, gain will be added
  wifiPhy.Set ("RxGain", DoubleValue (0) ); 
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO); 

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  // The below FixedRssLossModel will cause the rss to be fixed regardless
  // of the distance between the two stations, and the transmit power
  wifiChannel.AddPropagationLoss("ns3::FriisPropagationLossModel"); //Use Friis Propagation loss Model
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a non-QoS upper mac, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));
  // Set it to adhoc mode
  wifiMac.SetType ("ns3::AdhocWifiMac");
  NetDeviceContainer source_devices = wifi.Install (wifiPhy, wifiMac, source_nodes);
  NetDeviceContainer sink_devices = wifi.Install (wifiPhy, wifiMac, sink_nodes);


  // Note that with FixedRssLossModel, the positions below are not 
  // used for received signal strength. 
  MobilityHelper mobility;
  // For now let's use 5x5 grid
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                  "MinX", DoubleValue (0.0),
                                  "MinY", DoubleValue (0.0),
                                  "DeltaX", DoubleValue (distance),
                                  "DeltaY", DoubleValue (distance),
                                  "GridWidth", UintegerValue (5),
                                  "LayoutType", StringValue ("RowFirst"));
   mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
   mobility.Install (source_nodes);

   mobility.Install(sink_nodes);


  InternetStackHelper internet;
  internet.Install (source_nodes);


  Ipv4AddressHelper ipv4_source;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4_source.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i_source = ipv4_source.Assign (source_devices);

  Ipv4AddressHelper ipv4_ink;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4_ink.SetBase ("10.1.2.0", "255.255.255.0");
  Ipv4InterfaceContainer i_sink = ipv4_ink.Assign (sink_devices);


  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  static const InetSocketAddress beaconBroadcast = InetSocketAddress(Ipv4Address("255.255.255.255"));

  for (int n = 0; n < (int)numNodes; n++){
	  Ptr<Socket> beacon_sink = Socket::CreateSocket(sink_nodes.Get(n),tid);
	  InetSocketAddress beacon_local = InetSocketAddress(Ipv4Address::GetAny(), beaconPort);
	  beacon_sink->Bind(beacon_local);
	  beacon_sink->SetRecvCallback(MakeCallback(&ReceivePacket));

	  Ptr<Socket> beacon_source = Socket::CreateSocket(source_nodes.Get(n), tid);
	  beacon_source->Connect(beaconBroadcast);

	  Simulator::ScheduleWithContext(n, Seconds(1.0), &GenerateTraffic,
                                  beacon_source, packetSize, numPackets, interPacketInterval);
  }

 // Tracing
 wifiPhy.EnablePcap ("wifi-experiment1", sink_devices);


  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}