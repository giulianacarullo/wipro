/*
 * trickletime.h
 *
 *  Created on: 31/gen/2015
 *      Author: superjulietta
 */

#ifndef TRICKLETIME_H_
#define TRICKLETIME_H_
#include "ns3/core-module.h"
#include "tau.h"
#include "tau_to_sync.h"
#include "tau_sync.h"

using namespace ns3;

class trickle_time {

private:
	int c; //counter
	int k; //threshold (usually a small fixed integer, e.g. 1 or 2)
	tau_to_sync tau_v_ts; //time constant
	tau_sync tau_v_s;
	int t; //timer in range [0, tau] or [] [tau/2, tau] (depending on sync assumptions)
	trickle_time *tt;
    bool sync_required;
	bool net_changed;


public:
	trickle_time();
	trickle_time(bool syncNeeded);
	virtual ~trickle_time();
	void sameMetadata();
	bool shouldIBroadcast();
	void intervalCompleted();
	int getOnlyListeningTime();
	void netChanged();
	int getIntervalLength();
	bool isNetChanged();
	trickle_time getCurrentTrickleTime();
};

#endif /* TRICKLETIME_H_ */
