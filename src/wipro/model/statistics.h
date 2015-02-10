/*
 * statistics.h
 *
 *  Created on: 10/feb/2015
 *      Author: superjulietta
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include "ns3/core-module.h"
#include <string>

using namespace ns3;

class statistics {
private:
	std::map<std::string, int> dropped_known_packets;
	std::map<std::string, int> dropped_unknown_packets;
public:
	statistics();
	virtual ~statistics();
	void addDroppedKnownPacket(std::string ssid);
	void addDroppedUnknownPacket(std::string ssid);
	void printDropped();
	void printDropped(uint32_t id);
};


#endif /* STATISTICS_H_ */
