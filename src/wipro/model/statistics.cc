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
	//undropped = 0;
	dropped = std::set<std::string>();
	dropped_but_recognized = std::map<std::string, Time>();
}

statistics::statistics(int id_peer) {
	id = id_peer;
	dkp = 0;
	dukp = 0;
	//undropped = 0;
	dropped = std::set<std::string>();
	dropped_times = std::map<std::string, Time>();
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
		//if(id == 5)
		//	NS_LOG_UNCOND("Dropped : " <<ssid);
		dropped.insert(ssid);
		dropped_times[ssid] =  Simulator::Now();
		//dropped_but_recognized[ssid]= Simulator::Now();
	}
}

void
statistics::removeIfDropped(std::string ssid){
	if(dropped.count(ssid) != 0){
		//if(id == 5)
		//	NS_LOG_UNCOND("Removing " <<ssid << "from dropped nodes");
		//std::set<std::string>::iterator pos = dropped.find(ssid);
		dropped.erase(ssid); //required to properly manage dropped_but_recognized
		//storing the delay between first receive and current time.
		//computing the single peer discovery time.
		Time now = Simulator::Now();
		//Time delay = Time(now.GetMilliSeconds() - dropped_times[ssid].GetMilliSeconds());
		Time delay = Time::FromDouble(now.GetMilliSeconds() - dropped_times[ssid].GetMilliSeconds(), Time::MS);
		dropped_but_recognized[ssid] = delay;
	}
	//else manage undropped packets PROBABLY THERE IS A BUG!
	//simply undropped = num nodes - not recognized - dropped_but_rec
	else {
		//undropped++;
	}
}

void
statistics::printDropped(){
		std::stringstream convert_id; // stringstream used for the conversion
		convert_id << id;
		std::string ssid = convert_id.str();

		std::string drop= "Node "+ ssid + " - dropped known: ";
		std::stringstream convert;
		convert << dkp;
		drop = drop + convert.str() + " dropped unknown: " ;
		std::stringstream convert_uk;
		convert_uk << dukp;
		drop = drop + convert_uk.str() + " - not discovered peer cause dropped: " ;
		std::stringstream convert_not_recognized;
		convert_not_recognized << dropped.size();
		drop = drop + convert_not_recognized.str() + " - dropped but recognized:  " ;

		std::stringstream convert_drop_rec;
		convert_drop_rec << dropped_but_recognized.size();
				drop = drop + convert_drop_rec.str() + "; " ;
		NS_LOG_UNCOND("STATS - "<< drop);
		printSinglePeerDiscoveryTime();
}

void
statistics::printSinglePeerDiscoveryTime(){
	NS_LOG_UNCOND("STATS - Single Peer Discovery Time for node "<<id);
	for (std::map<std::string, Time>::iterator it=dropped_but_recognized.begin(); it!=dropped_but_recognized.end(); ++it)
	    NS_LOG_UNCOND (it->first << " => " << it->second.GetMilliSeconds());
}
