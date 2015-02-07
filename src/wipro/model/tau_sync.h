/*
 * tausync.h
 *
 *  Created on: 31/gen/2015
 *      Author: superjulietta
 */


#ifndef TAUSYNC_H_
#define TAUSYNC_H_
	/**
	 * Sync assumption holds
	 * @author giucar
	 *
	 */
#include <ns3/tau.h>
#include <cstdlib>
using namespace ns3;

class tau_sync: public tau{
public:
	tau_sync();
	virtual ~tau_sync();
	int getRandomInTheInterval();
	//int getTauValue();

};

#endif /* TAUSYNC_H_ */
