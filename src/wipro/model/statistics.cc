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
}

statistics::~statistics() {
	// TODO Auto-generated destructor stub
}
void
statistics::addDroppedKnownPacket(){
		dkp++;
}
void
statistics::addDroppedUnknownPacket(){
	dukp++;
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
		drop = drop + convert_uk.str() + "; " ;
		NS_LOG_UNCOND("STATS - "<< drop<<"\n");
}
