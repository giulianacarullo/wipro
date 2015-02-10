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
	int dkp;
	int dukp;
public:
	statistics();
	virtual ~statistics();
	void addDroppedKnownPacket();
	void addDroppedUnknownPacket();
	void printDropped(int id);
};


#endif /* STATISTICS_H_ */
