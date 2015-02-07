/*
 * tausync.h
 *
 *  Created on: 31/gen/2015
 *      Author: superjulietta
 */


#ifndef TAU_TO_SYNC_H_
#define TAU_TO_SYNC_H_
	/**
	 * Sync assumption holds
	 * @author giucar
	 *
	 */
#include "ns3/core-module.h"
#include <ns3/tau.h>
#include <cstdlib>

using namespace ns3;

class tau_to_sync : public tau {
public:
	tau_to_sync();
	virtual ~tau_to_sync();
	int getRandomInTheInterval();

};

#endif /* TAUSYNC_H_ */
