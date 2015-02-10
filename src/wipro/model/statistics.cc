/*
 * statistics.cc
 *
 *  Created on: 10/feb/2015
 *      Author: superjulietta
 */

#include "statistics.h"


statistics::statistics() {
	// TODO Auto-generated constructor stub
	dropped_known_packets = std::map<int, int>();
	dropped_unknown_packets = std::map<int, int>();
}

statistics::~statistics() {
	// TODO Auto-generated destructor stub
}
void
statistics::addDroppedKnownPacket(std::string ssid){
	int id = atoi(ssid.c_str());
	//if(dropped_known_packets.count(id)==0)
	//	dropped_known_packets[id]=0; //init
	//else
		dropped_known_packets[id] ++;
	//	dropped_known_packets[id] +=1;
		//dropped_known_packets[ssid] = dropped_known_packets[ssid] ++;
	//dropped_known_packets.insert();
}
void
statistics::addDroppedUnknownPacket(std::string ssid){
	NS_LOG_UNCOND("SIZE "<<dropped_unknown_packets.size());
	/*
	if(dropped_unknown_packets.count(ssid)==0) {
			NS_LOG_UNCOND("first unknown");
			dropped_unknown_packets[ssid]=0; //init
	}
	else{
		NS_LOG_UNCOND("adding unknown");
		dropped_unknown_packets[ssid] ++;
		dropped_unknown_packets[ssid] += 1;
	}*/
	int id = atoi(ssid.c_str());
	dropped_unknown_packets[id] ++;
}

void
statistics::printDropped(){
	/*
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
		*/
}
void
statistics::printDropped(int id){
		std::stringstream convert_id; // stringstream used for the conversion
		convert_id << id;
		std::string ssid = convert_id.str();
		//NS_LOG_UNCOND("SIZE"<< dropped_known_packets.size() << " - "<< dropped_known_packets.count(ssid));
		std::map<int, int>::iterator it = dropped_known_packets.find(id);
		if(it != dropped_known_packets.end()){
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
		else
		NS_LOG_UNCOND("DOH THE PROBLEM IS HERE");
}
