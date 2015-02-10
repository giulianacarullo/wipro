/*
 * statistics.cc
 *
 *  Created on: 10/feb/2015
 *      Author: superjulietta
 */

#include "statistics.h"


statistics::statistics() {
	// TODO Auto-generated constructor stub
	dropped_known_packets = std::map<std::string, int>();
	dropped_unknown_packets = std::map<std::string, int>();
}

statistics::~statistics() {
	// TODO Auto-generated destructor stub
}
void
statistics::addDroppedKnownPacket(std::string ssid){
	if(dropped_known_packets.count(ssid)==0)
		dropped_known_packets[ssid]=0; //init
	else
		//dropped_known_packets[ssid] ++;
		dropped_known_packets[ssid] = dropped_known_packets[ssid] ++;
	//dropped_known_packets.insert();
}
void
statistics::addDroppedUnknownPacket(std::string ssid){
	if(dropped_unknown_packets.count(ssid)==0) {
			NS_LOG_UNCOND("first unknown");
			dropped_unknown_packets[ssid]=0; //init
	}
	else{
		NS_LOG_UNCOND("adding unknown");
		dropped_unknown_packets[ssid] ++;
	}
}

void
statistics::printDropped(){
	std::string drop= "Dropped known:\n";
	std::stringstream convert; // stringstream used for the conversion


	for(std::map<std::string, int>::iterator iter = dropped_known_packets.begin(); iter != dropped_known_packets.end(); ++iter)
		{
			convert << iter->second;
			drop = drop + iter->first + " - " + convert.str() + "; " ;
		}
	NS_LOG_UNCOND( "STATS - "<<drop<<"\n");

	drop= "Dropped unknown:\n";
		for(std::map<std::string, int>::iterator iter = dropped_known_packets.begin(); iter != dropped_known_packets.end(); ++iter)
		{
			convert << iter->second;
			drop = drop + iter->first + " - " + convert.str()  + "; " ;
		}
		NS_LOG_UNCOND("STATS - "<< drop<<"\n");
}
void
statistics::printDropped(uint32_t id){
		std::stringstream convert_id; // stringstream used for the conversion
		convert_id << id;
		std::map<std::string, int>::iterator it = dropped_known_packets.find(convert_id.str());
		//(it != dropped_known_packets.end())
		std::string drop= "Node "+ convert_id.str() + " - dropped known: ";
		std::stringstream convert;
		convert << it->second;
		drop = drop + convert.str() + " dropped unknown: " ;
		//it = dropped_unknown_packets.find(convert.str());
		std::stringstream convert_uk;
		convert_uk << it->second;
		drop = drop + convert_uk.str() + "; " ;
		NS_LOG_UNCOND("STATS - "<< drop<<"\n");
}
