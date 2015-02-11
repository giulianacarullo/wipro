/*
 * statistics.cc
 *
 *  Created on: 10/feb/2015
 *      Author: superjulietta
 */

#include "statistics.h"


statistics::statistics() {
	dkp = 0;
	dukp = 0;
	undropped = 0;
	dropped = std::set<std::string>();
	dropped_but_recognized = std::map<std::string, Time>();
}

statistics::~statistics() {
	// TODO Auto-generated destructor stub
}
void
statistics::addDroppedKnownPacket(){
		dkp++;
}
void
statistics::addDroppedUnknownPacket(std::string ssid){
	dukp++;
	if(dropped.count(ssid) == 0){
		dropped.insert(ssid);
		dropped_but_recognized[ssid]= Simulator::Now();
	}
}

void
statistics::removeIfDropped(std::string ssid){
	if(dropped.count(ssid) == 0){
		//dropped.erase(ssid);
		//storing the delay between first receive and current time.
		//computing the single peer discovery time.
		Time now = Simulator::Now();
		Time delay = Time(now.GetMilliSeconds() - dropped_but_recognized[ssid].GetMilliSeconds());
		dropped_but_recognized[ssid] = delay;
	}
	//else manage undropped packets PROBABLY THERE IS A BUG!
	else {
		undropped++;
	}
}

void
statistics::printDropped(int id){
		std::stringstream convert_id; // stringstream used for the conversion
		convert_id << id;
		std::string ssid = convert_id.str();

		std::string drop= "Node "+ ssid + " - dropped known: ";
		std::stringstream convert;
		convert << dkp;
		drop = drop + convert.str() + " dropped unknown: " ;
		std::stringstream convert_uk;
		convert_uk << dukp;
		drop = drop + convert_uk.str() + " - not discovered peer: " ;
		std::stringstream convert_not_recognized;
		convert_not_recognized << dropped.size();
		drop = drop + convert_not_recognized.str() + " undropped:  " ;

		std::stringstream convert_undrop;
		convert_undrop << undropped;
				drop = drop + convert_undrop.str() + "; " ;
		NS_LOG_UNCOND("STATS - "<< drop<<"\n");
}
