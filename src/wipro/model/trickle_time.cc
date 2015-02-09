/*
 * trickletime.cpp
 *
 *  Created on: 31/gen/2015
 *      Author: superjulietta
 */

#include "trickle_time.h"

trickle_time::trickle_time() {
	// TODO Auto-generated constructor stub
}

trickle_time::~trickle_time() {
	// TODO Auto-generated destructor stub
}



	//Simulating a kind of shared obj
	//needs to be changed in order to manage both sync or notsync
	trickle_time trickle_time::getCurrentTrickleTime(){
		if(tt)
			tt = new trickle_time(true);
		return tt;
	}

	trickle_time::trickle_time(bool syncNeeded){
		//manage tau
		k = 2;
		net_changed = false;
		c = 0;
		//tau_v = (syncNeeded)? tau_to_sync():tau_sync();
		sync_required = syncNeeded;
		if(syncNeeded) {
			tau_v_ts = tau_to_sync();
			t = tau_v_ts.getRandomInTheInterval();
		}
		else {
			tau_v_s = tau_sync();
			t = tau_v_s.getRandomInTheInterval();
		}

	}

	/*
	 * When a mote hears metadata identical to its own, it increments
	 * the constant c
	 */
	void trickle_time::sameMetadata(){
		c++;
	}

	/*
	 * At time t (please also check t), the mote broadcast if this
	 * condition holds.
	 *
	 */
	bool trickle_time::shouldIBroadcast(){
		return (c<k || net_changed)? true: false;
	}

	/*
	 * When the interval of size tau completes, c is reset to zero
	 * and t is reset to a new random value in the range [0, tau]
	 * (if sync assumption holds) or to a new random value in the
	 * range [tau/2, tau] (if sync is needed)
	 */
	void trickle_time::intervalCompleted(){
		c = 0;
		net_changed = false;
		if(sync_required) {
			tau_v_ts.tauExpired();
			t = tau_v_ts.getRandomInTheInterval();
		}
		else {
			tau_v_s.tauExpired();
			t = tau_v_s.getRandomInTheInterval();
		}
	}

	int trickle_time::getOnlyListeningTime(){
		return t;
	}

	/**
	 * My mod: this method should be invoked both when a peer in my
	 * neighbourhood informs me that its neighbourhood changed
	 * and when my neighbourhood change.
	 */
	void trickle_time::netChanged(){
		net_changed = true;
		if(sync_required)
			tau_v_ts.networkChanged();
		else
			tau_v_s.networkChanged();
	}

	int trickle_time::getIntervalLength(){
		if(sync_required)
			return tau_v_ts.getTauValue();
		return tau_v_s.getTauValue();
	}

	bool trickle_time::isNetChanged(){
		return net_changed;
	}

