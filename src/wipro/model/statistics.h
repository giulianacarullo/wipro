/*
 * statistics.h
 *
 *  Created on: 10/feb/2015
 *      Author: superjulietta
 */

#ifndef STATISTICS_H_
#define STATISTICS_H_

#include "ns3/core-module.h"
#include <stdlib.h>
#include <string>

using namespace ns3;

class statistics {
private:
	//those variable stores the number of peers
	//immediately recognized as soon as the become available
	int undropped;
	int dkp;
	int dukp;
	//probably redundant
	std::set<std::string> dropped;
	//initially it contains the first time the packet is
	//dropped. Then, when a node is recognized for the first time (update done in removeIfDropped()),
	//it contains the delay until discovery (single peer discovery time)
	std::map<std::string, Time> dropped_but_recognized;

public:
	statistics();
	virtual ~statistics();

	/*
	 * Increments the total number of already known peers dropped
	 * during the whole execution
	 */
	void addDroppedKnownPacket();
	/*
	 * Increments the total number of unknown peers dropped
	 * during the whole execution
	 */
	void addDroppedUnknownPacket(std::string ssid);

	/*
	* Print the total number of already known peers dropped
	* during the whole execution, plus those unknown during the execution.
	* It also prints the effective number of unrecognized peers
	* during the execution. Hopefully, this number will be less than
	* droppedUnknown packets, since even a packet is dropped, a peer
	* may be able to recognize that peer later on
	*/
	void printDropped(int id);

	/*
	 * This method needs to be called when a new peer is discovered
	 */
	void removeIfDropped(std::string ssid);
};


#endif /* STATISTICS_H_ */
