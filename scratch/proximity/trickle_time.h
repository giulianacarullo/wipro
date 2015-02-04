/*
 * trickletime.h
 *
 *  Created on: 31/gen/2015
 *      Author: superjulietta
 */

#ifndef TRICKLETIME_H_
#define TRICKLETIME_H_
#include <tau.h>
#include <tau_to_sync.h>
#include <tau_sync.h>

class trickle_time {
public:
	trickle_time();
	virtual ~trickle_time();
	void sameMetadata();
	bool shouldIBroadcast();
	void intervalCompleted();
	int getOnlyListeningTime();
	void netChanged();
	int getIntervalLength();
	bool isNetChanged();
	static trickle_time getCurrentTrickleTime();
};

#endif /* TRICKLETIME_H_ */
