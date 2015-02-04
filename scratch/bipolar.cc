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
 * 
 * Author: Giuliana Carullo <giulietta.giugiu@gmail.com>
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

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/olsr-helper.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/constant-velocity-mobility-model.h"
#include "ns3/mobility-module.h"
#include "ns3/mesh-wifi-beacon.h" 

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

//Custom #include
#include "scratch/proximity/bipolar_nodes.cpp"

NS_LOG_COMPONENT_DEFINE ("bipolar");

using namespace ns3;
static bipolar_nodes_addr addresses = bipolar_nodes_addr();
static const ns3::InetSocketAddress broadcastAddress = ns3::InetSocketAddress(ns3::Ipv4Address("255.255.255.255"),0);
static NodeContainer wifiClient;
//Adding an ap for each sta node in the network
static NodeContainer ap;
  
/*
void ReceivePacket(:Receive (Ptr<Packet> packet, WifiMacHeader const *hdr)) {
       packet->PeekHeader (beacon_hdr);
       std::cout<< "Beacon received from " << hdr->GetAddr2 () << " I am " << GetAddress () << "with SSID " 
       <<beacon_hdr.GetSsid ()<< " at "
} */

void PrintNodesPosition(NodeContainer nodes, std::string type){
   // iterate our nodes and print their position.
   std::cout << "-----------------------" << std::endl;
   std::cout << "Current "<< type <<"'s position" << std::endl;
   for (NodeContainer::Iterator j = nodes.Begin (); j != nodes.End (); ++j) {
          Ptr<Node> object = *j;
          Ptr<MobilityModel> position = object->GetObject<MobilityModel> ();
          NS_ASSERT (position != 0);
          Vector pos = position->GetPosition ();
          std::cout << object->GetId() << "   "<< "x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << std::endl;
   }
   std::cout << "-----------------------" << std::endl;
}

void AdhocWifiMac::Receive (Ptr<Packet> packet, const WifiMacHeader *hdr) {
     //NS_LOG_FUNCTION (this << packet << hdr);
     //Ma gli altri indirizzi...che roba sono????
	//std::cout << "Addr1" <<hdr->GetAddr1()<<" addr 2 "<<hdr->GetAddr2()<<" Addr 3"<<hdr->GetAddr3()<<std::endl;
     if(hdr->IsBeacon()){
		 //PrintNodesPosition(wifiClient, "wifiClient receive");
		  MgtBeaconHeader beacon_hdr;
          packet->PeekHeader (beacon_hdr);
          //std::cout<< "is broadcast" << GetAddress().IsBroadcast() <<"same address:(0 = same address) "<<addresses.compare(GetAddress (), hdr->GetAddr2())<<std::endl;

          if(!addresses.compare(GetAddress (), hdr->GetAddr2()))
				std::cout<<" STA " << GetAddress () << " received beacon from AP " << hdr->GetAddr2 () << " with SSID " 
				<< beacon_hdr.GetSsid ()<< " at " << Simulator::Now ().GetMicroSeconds () << " microseconds" <<std::endl;
				//std::cout << "Addr1" <<hdr->GetAddr1()<<" addr 2 "<<hdr->GetAddr2()<<" Addr 3"<<hdr->GetAddr3()<<std::endl;
				//AdhocWifiMac::Enqueue(packet, Mac48Address("255.255.255.255"));
    }
   // else if(!addresses.compare(GetAddress (), hdr->GetAddr2()) && addresses.not_another_sta(hdr->GetAddr2()))
   //		std::cout<<" STA " << GetAddress () << " received message from AP " << hdr->GetAddr2 () <<std::endl;
}

void PrintNodePosition(NodeContainer nodes, uint32_t nodeIndex){
	Ptr<Node> object = nodes.Get(nodeIndex);
    Ptr<MobilityModel> position = object->GetObject<MobilityModel> ();
    NS_ASSERT (position != 0);
    Vector pos = position->GetPosition ();
    std::cout << object->GetId() << "   " << "x=" << pos.x << ", y=" << pos.y << ", z=" << pos.z << std::endl;
}

void CourseChange(std::string context, Ptr<const MobilityModel> model)
{
	Vector position = model -> GetPosition();
	std::cout << context << "x = " << position.x << ", y = " << position.y  << std::endl;
	NS_LOG_UNCOND(context << "x = " << position.x << ", y = " << position.y );
}



  
int main (int argc, char *argv[])
{
  std::string phyMode ("DsssRate1Mbps");
  double distance = 10;  // m my_comment: the distance is the actual distance between each couple of nodes
  uint32_t packetSize = 1000; // bytes
  uint32_t numPackets = 1;
  uint32_t numNodes = 5;  // by default, 5x5
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

  wifiClient.Create (numNodes);
  ap.Create(numNodes);
  
  // The below set of helpers will help us to put together the wifi NICs we want
  WifiHelper wifi;
  if (verbose)
    {
      wifi.EnableLogComponents ();  // Turn on all Wifi logging
    }

  YansWifiPhyHelper wifiPhy =  YansWifiPhyHelper::Default ();
  // set it to zero; otherwise, gain will be added
  wifiPhy.Set ("RxGain", DoubleValue (-2) ); //-10 
  // ns-3 supports RadioTap and Prism tracing extensions for 802.11b
  wifiPhy.SetPcapDataLinkType (YansWifiPhyHelper::DLT_IEEE802_11_RADIO); 

  //Using a single channel, otherwise, if two APs were created with two different channels, the STA
  //node would still only know about one of them.
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
  // setup stas
  //wifiMac.SetType ("ns3::StaWifiMac",
  //                      "Ssid", SsidValue (Ssid ("Try")), 
  //                      "ActiveProbing", BooleanValue (false));
  NetDeviceContainer staDevices = wifi.Install (wifiPhy, wifiMac, wifiClient);
  NetDeviceContainer apDevices;
  //Setup ap
  // Create unique SSIDs for these networks
  for (uint32_t i = 0; i < numNodes; ++i) {
	    std::string ssidString ("wiPro");
		std::stringstream ss;
		//ss << i;
		ss << ap.Get(i)->GetId();
		ssidString += ss.str ();		
		Ssid ssid = Ssid (ssidString);
		//std::cout << "SSID: " << ssidString << std::endl;
		wifiMac.SetType ("ns3::ApWifiMac", "Ssid", SsidValue (ssid));
		// setup ap.
		//ApWifiMac apWifiMac = ();
		wifiMac.SetType ("ns3::ApWifiMac",
                        "Ssid", SsidValue (ssid),
                        "BeaconGeneration", BooleanValue (true), //Whether or not beacons are generated
                        "BeaconInterval", TimeValue(Seconds(10))); //Delay between two beacons
       if(i==0)
          apDevices = wifi.Install (wifiPhy, wifiMac, ap.Get(i));
	   else
	      apDevices.Add(wifi.Install (wifiPhy, wifiMac, ap.Get(i)));
  }
 
  // Setting the rate control algorithm
  wifi.SetRemoteStationManager ("ns3::ArfWifiManager");
  
 
 /********************************************************
  bipolar_nodes_manager manager = bipolar_nodes_manager(numNodes);
  manager.configure_network();
  NodeContainer* wifiClient = manager.get_client();
  NodeContainer* ap = manager.get_ap();
  NetDeviceContainer* staDevices = manager.get_sta_net_device();
  NetDeviceContainer* apDevices = manager.get_ap_net_device();
  YansWifiPhyHelper* wifiPhy = manager.get_wifiPhy();
  *********************************************************/
  MobilityHelper mobility;
  MobilityHelper mobilityAP;
  
  // setup the grid itself: objects are layed out
  // started from (0.0,0.0) with 5 (GridWidth) objects per row, 
  // the x interval between each object is *distance* meters 
  // and the y interval between each object is *distance* meters
  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (distance),
                                 "DeltaY", DoubleValue (distance),
                                 "GridWidth", UintegerValue (5),
                                 "LayoutType", StringValue ("RowFirst"));
  //Setting mobility model for APs
   mobilityAP.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (distance),
                                 "DeltaY", DoubleValue (distance),
                                 "GridWidth", UintegerValue (5),
                                 "LayoutType", StringValue ("RowFirst"));
  // each object will be attached a static position.
  // i.e., once set by the "position allocator", the
  // position will never change.
  //mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
  //                           "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
  //mobilityAP.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
  //                           "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));                           
  //mobility.SetMobilityModel ("ns3::RandomDirection2dMobilityModel",
  //                               "Bounds", RectangleValue (Rectangle (-10, 10, -10, 10)),
  //                               "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=2]"),
  //                               "Pause", StringValue ("ns3::ConstantRandomVariable[Constant=50]"));
  //mobilityAP.SetMobilityModel ("ns3::RandomDirection2dMobilityModel",
  //                               "Bounds", RectangleValue (Rectangle (-10, 10, -10, 10)),
  //                               "Speed", StringValue ("ns3::ConstantRandomVariable[Constant=2]"),
  //                               "Pause", StringValue ("ns3::ConstantRandomVariable[Constant=50]"));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  mobilityAP.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  // finalize the setup by attaching to each object
  // in the input array a position and initializing
  // this position with the calculated coordinates.
  mobility.Install (wifiClient);
  mobilityAP.Install (ap);
  
  //Setting up nodes' velocities
  for (uint n=0 ; n < numNodes ; n++) {
	    distance = distance +1;
      	Ptr<ConstantPositionMobilityModel> mob = ap.Get(n)->GetObject<ConstantPositionMobilityModel>();
      	mob->SetPosition(Vector(distance, distance, 0));
		mob = wifiClient.Get(n)->GetObject<ConstantPositionMobilityModel>();
		mob->SetPosition(Vector(distance, distance, 0));
  }
  // Enable OLSR
  OlsrHelper olsr;
  Ipv4StaticRoutingHelper staticRouting;

  Ipv4ListRoutingHelper list;
  list.Add (staticRouting, 0);
  list.Add (olsr, 10);

  InternetStackHelper internet;
  internet.SetRoutingHelper (list); // has effect on the next Install ()
  internet.Install (wifiClient);
  internet.Install (ap);
  
  Ipv4AddressHelper ipv4;
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  //ipv4.SetBase ("10.1.255.255", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (staDevices);
  ipv4.Assign (apDevices);

  if (tracing == true)
    {
      AsciiTraceHelper ascii;
      wifiPhy.EnableAsciiAll (ascii.CreateFileStream ("bipolar.tr"));
      wifiPhy.EnablePcap ("bipolar", staDevices);
      // Trace routing tables
      Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("bipolar.routes", std::ios::out);
      olsr.PrintRoutingTableAllEvery (Seconds (2), routingStream);

      // To do-- enable an IP-level trace that shows forwarding events only
    }

  // Give OLSR time to converge-- 30 seconds perhaps
  //Simulator::Schedule (Seconds (30.0), &GenerateTraffic, 
  //                      source, packetSize, numPackets, interPacketInterval);
  std::ostringstream oss;
	oss << "/NodeList/" << wifiClient.Get(0)-> GetId() << "/$ns3::MobilityModel/CourseChange";

	Config::Connect(oss.str(), MakeCallback(&CourseChange));
	
  Simulator::Schedule (Seconds (3.0), &PrintNodesPosition, wifiClient, "wifiClient");
  Simulator::Schedule (Seconds (3.0), &PrintNodesPosition, ap, "ap");

  // Output what we are doing
 // NS_LOG_UNCOND ("Testing from node " << sourceNode << " to " << sinkNode << " with grid distance " << distance);
	
  //Ipv4NixVectorRouting::GetAdjacentNetDevices (Ptr<NetDevice> netDevice, Ptr<Channel> channel, NetDeviceContainer & netDeviceContainer)
  //Ptr<Node> first_node = wifiClient.Get(0);
  //Ptr<NetDevice> first_node_dev = first_node->GetDevice(0);
  //std::cout << "Net Device associated with the first node " <<first_node_dev->GetNode()->GetId() << std::endl;
  //NetDeviceContainer netDeviceContainer =  	NetDeviceContainer ();
  //Ptr<NetDevice> localNetDevice = m_node->GetDevice (i);
  //Ptr<Channel> channel = localNetDevice->GetChannel ();
  /*Ptr<Channel> channel = first_node_dev->GetChannel ();
   for (uint32_t i = 0; i < channel->GetNDevices (); i++)
    {
      Ptr<NetDevice> remoteDevice = channel->GetDevice (i);
      if (remoteDevice != first_node_dev) {
              netDeviceContainer.Add (channel->GetDevice (i));
      }
    }
    std::cout<<"Adjacent nodes: " << netDeviceContainer.GetN() <<std::endl;
    
    */
    //first_node->SetRecvCallback (MakeCallback (&ReceivePacket));
	//populating addresses
    for (uint32_t i = 0; i < numNodes; i++){
	  
      addresses.add(Mac48Address::ConvertFrom(staDevices.Get(i)->GetAddress()), Mac48Address::ConvertFrom(apDevices.Get(i)->GetAddress()));
      std::cout <<"STA " << Mac48Address::ConvertFrom(staDevices.Get(i)->GetAddress()) << " corresponds to" << " AP "<< Mac48Address::ConvertFrom(apDevices.Get(i)->GetAddress()) << std::endl;
      //if (remoteDevice) {
      //        netDeviceContainer.Add (channel->GetDevice (i));
      //}
    }

 // GetAdjacentNetDevices (first_node_dev, &wifiChannel, NetDeviceContainer & netDeviceContainer)
  PrintNodesPosition(wifiClient,"wifi");
  Simulator::Stop (Seconds (50.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}

