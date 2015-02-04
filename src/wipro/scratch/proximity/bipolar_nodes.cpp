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


#include <map>
#include <fstream>
#include <iostream>
#include <string>

//NS_LOG_COMPONENT_DEFINE ("bipolar_nodes_manager");

using namespace ns3;

class bipolar_node{
	 Ptr<Node> sta;
	 Ptr<Node> ap;
	 
	 public:
		bipolar_node(Ptr<Node> sta_init, Ptr<Node> ap_init) {
			sta = sta_init;
			ap = ap_init;
		};
};

class bipolar_nodes{
	// Sta, ap
	std::map <Ptr<Node>, Ptr<Node> > nodes;
	
	public:
		bipolar_nodes(){};
	
		void add(Ptr<Node> sta, Ptr<Node> ap) {
			nodes[sta] = ap;
		}
		
		bool compare(Ptr<Node> sta, Ptr<Node> ap) {
			//if () {
			return nodes.find(sta)->first->GetId() == ap->GetId() ? true : false;	
		}
};

class bipolar_nodes_addr{
	// Sta, ap
	std::map <Address, Address> nodes;
	;
	
	public:
		bipolar_nodes_addr(){};
	
		void add(Address sta, Address ap) {
			nodes[sta] = ap;
			
		}
		
		bool compare(Address sta, Address ap) {
			//std::cout <<"Comparing STA "<< sta << "with "<< nodes.find(sta)->first<<std::endl;
			return nodes.find(sta)->second == ap ? true : false;	
		}
		bool not_another_sta(Address ap) {
			//std::cout <<"Comparing STA "<< sta << "with "<< nodes.find(sta)->first<<std::endl;
			return nodes.count(ap)== 0;	
		}
};



class bipolar_nodes_man{
		
};

class bipolar_nodes_manager{
	NodeContainer wifiClient;
	NodeContainer ap;
	uint32_t num_nodes;
	bool verbose;
	NetDeviceContainer staDevices;
    NetDeviceContainer apDevices;
    std::string phyMode;
	YansWifiPhyHelper wifiPhy;
	
	bipolar_nodes nodes;
	public:
	bipolar_nodes_manager(uint32_t n){
		verbose = false;
		phyMode = "DsssRate1Mbps";
		num_nodes = n;
		wifiClient.Create (num_nodes);
		ap.Create(num_nodes);
		nodes = bipolar_nodes();
	}
	
	void configure_network(){
		  // The below set of helpers will help us to put together the wifi NICs we want
		  WifiHelper wifi;
		  if (verbose)
			{
			  wifi.EnableLogComponents ();  // Turn on all Wifi logging
			}

		  wifiPhy =  YansWifiPhyHelper::Default ();
		  // set it to zero; otherwise, gain will be added
		  wifiPhy.Set ("RxGain", DoubleValue (-10) ); 
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
		  //                      "Ssid", SsidValue (ssid),
		  //                      "ActiveProbing", BooleanValue (false));
		  //    NetDeviceContainer staDevices = wifiInfra.Install (wifiPhy, macInfra, stas);
		  NetDeviceContainer staDevices = wifi.Install (wifiPhy, wifiMac, wifiClient);
		  NetDeviceContainer apDevices;
		  //Setup ap
		  // Create unique SSIDs for these networks
		  for (uint32_t i = 0; i < num_nodes; ++i) {
				std::string ssidString ("wiPro");
				std::stringstream ss;
				//ss << i;
				ss << ap.Get(i)->GetId();
				ssidString += ss.str ();		
				Ssid ssid = Ssid (ssidString);
				std::cout << "SSID: " << ssidString << std::endl;
				wifiMac.SetType ("ns3::ApWifiMac", "Ssid", SsidValue (ssid));
				// setup ap.
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
		  
	}
	NodeContainer* get_client(){
		return &wifiClient;
	}
	
	NodeContainer* get_ap(){
		return &ap;
	}
	
	NetDeviceContainer* get_sta_net_device(){
		return &staDevices;
	}
	NetDeviceContainer* get_ap_net_device(){
		return &apDevices;
	}
	
	YansWifiPhyHelper* get_wifiPhy(){
		return &wifiPhy;
	} 
};

