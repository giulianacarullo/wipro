/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 University of Washington
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

//
// This program configures a grid (default 5x5) of nodes on an 
// 802.11b physical layer, with
// 802.11b NICs in adhoc mode, and by default, sends one packet of 1000 
// (application) bytes to node 1.
//
// The default layout is like this, on a 2-D grid.
//
// n20  n21  n22  n23  n24
// n15  n16  n17  n18  n19
// n10  n11  n12  n13  n14
// n5   n6   n7   n8   n9
// n0   n1   n2   n3   n4
//
// the layout is affected by the parameters given to GridPositionAllocator;
// by default, GridWidth is 5 and numNodes is 25..
//
// There are a number of command-line options available to control
// the default behavior.  The list of available command-line options
// can be listed with the following command:
// ./waf --run "wifi-simple-adhoc-grid --help"
//
// Note that all ns-3 attributes (not just the ones exposed in the below
// script) can be changed at command line; see the ns-3 documentation.
//
// For instance, for this configuration, the physical layer will
// stop successfully receiving packets when distance increases beyond
// the default of 500m.
// To see this effect, try running:
//
// ./waf --run "wifi-simple-adhoc --distance=500"
// ./waf --run "wifi-simple-adhoc --distance=1000"
// ./waf --run "wifi-simple-adhoc --distance=1500"
// 
// The source node and sink node can be changed like this:
// 
// ./waf --run "wifi-simple-adhoc --sourceNode=20 --sinkNode=10"
//
// This script can also be helpful to put the Wifi layer into verbose
// logging mode; this command will turn on all wifi logging:
// 
// ./waf --run "wifi-simple-adhoc-grid --verbose=1"
//
// By default, trace file writing is off-- to enable it, try:
// ./waf --run "wifi-simple-adhoc-grid --tracing=1"
//
// When you are done tracing, you will notice many pcap trace files 
// in your directory.  If you have tcpdump installed, you can try this:
//
// tcpdump -r wifi-simple-adhoc-grid-0-0.pcap -nn -tt
//

//WORKING BROADCAST SCRIPT!!!!!!!!

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/duty_cycle_application_helper.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

//Custom
//#include "proximity/duty_cycle_application_helper.h"
//#include "proximity/duty_cycle_application_helper.cpp"
//#include "proximity/duty_cycle_application_helper.h"

NS_LOG_COMPONENT_DEFINE ("thesis_simulation");

using namespace ns3;

void PrintNodePosition(NodeContainer nodes, uint32_t nodeIndex){
	Ptr<Node> object = nodes.Get(nodeIndex);
    Ptr<MobilityModel> position = object->GetObject<MobilityModel> ();
    NS_ASSERT (position != 0);
    Vector pos = position->GetPosition ();
    std::cout << object->GetId() << "   " << "x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << std::endl;
}


void PrintNodesPosition(NodeContainer nodes){
   // iterate our nodes and print their position.
   std::cout << "-----------------------" << std::endl;
   std::cout << "Current nodes position" << std::endl;
   for (NodeContainer::Iterator j = nodes.Begin (); j != nodes.End (); ++j) {
          Ptr<Node> object = *j;
          Ptr<MobilityModel> position = object->GetObject<MobilityModel> ();
          NS_ASSERT (position != 0);
          Vector pos = position->GetPosition ();
          std::cout << object->GetId() << "   "<< "x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << std::endl;
   }
   std::cout << "-----------------------" << std::endl;
}

int main (int argc, char *argv[])
{
  std::string phyMode ("DsssRate1Mbps");
  double distance = 50;  // m my_comment: the distance is the actual distance between each couple of nodes
  uint32_t packetSize = 1000; // bytes
  uint32_t numPackets = 1;
  uint32_t numNodes = 8;  // by default, 5x5
  uint32_t sinkNode = 0;
  uint32_t sourceNode = 24;
  double sim_len = 20.0;
  double interval = 1.0; // seconds
  bool verbose = false;
  bool tracing = false;

  CommandLine cmd;

  cmd.AddValue ("phyMode", "Wifi Phy mode", phyMode);
  cmd.AddValue ("distance", "distance (m)", distance);
  cmd.AddValue ("packetSize", "size of application packet sent", packetSize);
  cmd.AddValue ("numPackets", "number of packets generated", numPackets);
  cmd.AddValue ("interval", "interval (seconds) between packets", interval);
  cmd.AddValue ("verbose", "turn on all WifiNetDevice log components", verbose);
  cmd.AddValue ("tracing", "turn on ascii and pcap tracing", tracing);
  cmd.AddValue ("numNodes", "number of nodes", numNodes);
  cmd.AddValue ("sinkNode", "Receiver node number", sinkNode);
  cmd.AddValue ("sourceNode", "Sender node number", sourceNode);
  cmd.AddValue ("simLen", "simulation length", sim_len);

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

  NodeContainer c;
  c.Create (numNodes);

  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  if (verbose)
    {
      wifi.EnableLogComponents ();  // Turn on all Wifi logging
    }

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  // set it to zero; otherwise, gain will be added
  wifiPhy.Set ("RxGain", DoubleValue (-10) ); 
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO); 

  YansWifiChannelHelper wifiChannel;
  wifiChannel.SetPropagationDelay ("ns3::ConstantSpeedPropagationDelayModel");
  wifiChannel.AddPropagationLoss ("ns3::FriisPropagationLossModel");
  wifiPhy.SetChannel (wifiChannel.Create ());

  // Add a non-QoS upper mac, and disable rate control
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default ();
  wifi.SetStandard (WIFI_PHY_STANDARD_80211b);
  wifi.SetRemoteStationManager ("ns3::ConstantRateWifiManager",
                                "DataMode",StringValue (phyMode),
                                "ControlMode",StringValue (phyMode));
  // Set it to adhoc mode
 wifiMac.SetType ("ns3::AdhocWifiMac");

  NetDeviceContainer devices = wifi.Install (wifiPhy, wifiMac, c);

  MobilityHelper mobility;
  // setup the grid itself: objects are layed out
  // started from (0.0,0.0) with 5 (GridWidth) objects per row, 
  // the x interval between each object is *distance* meters 
  // and the y interval between each object is *distance* meters
  mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator",
                                  "X", StringValue ("100.0"),
                                  "Y", StringValue ("100.0"),
                                  "Rho", StringValue ("ns3::UniformRandomVariable[Min=10|Max=10]"));
  /*mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue (0.0),
                                   "MinY", DoubleValue (0.0),
                                   "DeltaX", DoubleValue (distance),
                                   "DeltaY", DoubleValue (distance),
                                   "GridWidth", UintegerValue (200),
                                   "LayoutType", StringValue ("RowFirst"));*/
   mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                              "Mode", StringValue ("Time"),
                              "Time", StringValue ("2s"),
                              //"Speed", StringValue ("ns3::ConstantRandomVariable[Constant=1]"),
                              "Speed", StringValue ("ns3::UniformRandomVariable[Min=0.0|Max=5.0]"),
                              //"Distance", StringValue("10"),
                              "Bounds", StringValue ("0|200|0|200"));
  /*
   *
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (distance),
                                 "DeltaY", DoubleValue (distance),
                                 "GridWidth", UintegerValue (50),
                                 "LayoutType", StringValue ("RowFirst"));*/
  // each object will be attached a static position.
  // i.e., once set by the "position allocator", the
  // position will never change.
  //mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  //mobility.SetMobilityModel("ns3::RandomWalk2dMobilityModel", "Bounds", RectangleValue(Rectangle(0,5,5,5)));

  //mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  //for (uint32_t i = 0; i<c.GetN(); i++){
//	  Ptr <ConstantVelocityMobilityModel> cv =c.Get(i)->GetObject<ConstantVelocityMobilityModel>();
//	  cv->SetVelocity(Vector3D(-16.0, 0.0, 0.0));

  //}
 // mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel",
                                  //"Position", Vector3DValue (Vector3D (5, 0, 0)),
                                  //"Velocity", Vector3DValue (Vector3D (-100, 0, 0)));
  // finalize the setup by attaching to each object
  // in the input array a position and initializing
  // this position with the calculated coordinates.
  mobility.Install (c);
  //printing nodes' position
  PrintNodesPosition(c);
  
  // Enable OLSR
  OlsrHelper olsr;
  Ipv4StaticRoutingHelper staticRouting;

  Ipv4ListRoutingHelper list;
  list.Add (staticRouting, 0);
  list.Add (olsr, 10);

  InternetStackHelper internet;
  internet.SetRoutingHelper (list); // has effect on the next Install ()
  internet.Install (c);

  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (devices);

  /***********
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  Ptr<Socket> recvSink;
  InetSocketAddress local = InetSocketAddress (Ipv4Address::GetAny (), 80);
 
  for (uint32_t sinkNode = 0; sinkNode < (numNodes); sinkNode++) {
	recvSink = Socket::CreateSocket (c.Get (sinkNode), tid);
	recvSink->Bind (local);
    recvSink->SetRecvCallback (MakeCallback (&ReceivePacket));
  }
    
  //Currently only the source node is allowed for broadcast
  Ptr<Socket> source = Socket::CreateSocket (c.Get (sourceNode), tid);
  // trying to go for broadcast
	static const InetSocketAddress beaconBroadcast = InetSocketAddress(Ipv4Address("255.255.255.255"), 80);
	source->SetAllowBroadcast(true);
	source->Connect(beaconBroadcast);
  // end broadcast
  
  // Single message
  //InetSocketAddress remote = InetSocketAddress (i.GetAddress (sinkNode, 0), 80);
  //source->Connect (remote);
   *
   *************************************/
  // end single message
  if (tracing == true)
    {
      AsciiTraceHelper ascii;
      wifiPhy.EnableAsciiAll (ascii.CreateFileStream ("thesis_simulation.tr"));
      wifiPhy.EnablePcap ("thesis_simulation", devices);
      // Trace routing tables
      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("thesis_simulation", std::ios::out);
      olsr.PrintRoutingTableAllEvery (Seconds (2), routingStream);

      // To do-- enable an IP-level trace that shows forwarding events only
    }

  // Give OLSR time to converge-- 30 seconds perhaps
  //Simulator::Schedule (Seconds (10.0), &GenerateTraffic,
  //                     source, packetSize, numPackets, interPacketInterval, true);

  //duty_cycle_application_helper app =
  duty_cycle_application_helper app= duty_cycle_application_helper ("ns3::UdpSocketFactory");
  app.setDataForStats(numNodes, 20);
 ApplicationContainer apps = app.Install(c);
  // Output what we are doing
  //NS_LOG_UNCOND ("Testing broadcast from node " << sourceNode << " with grid distance " << distance);

  apps.Start(Seconds(0));
  apps.Stop(Seconds (sim_len-0.1));
  //for(int i = 0; i<5; i++)
  //	  Simulator::Schedule(Seconds(i), &PrintNodesPosition, c);

  Simulator::Stop (Seconds (sim_len));
  Simulator::Run ();
  Simulator::Destroy ();


  return 0;
}

