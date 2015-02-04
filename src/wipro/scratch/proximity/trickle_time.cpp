/*
 * trickletime.cpp
 *
 *  Created on: 31/gen/2015
 *      Author: superjulietta
 */

#include "trickle_time.h"
/*
trickle_time::trickle_time() {
	// TODO Auto-generated constructor stub

}

trickle_time::~trickle_time() {
	// TODO Auto-generated destructor stub
}*/

class trickle_time {
	private:
	int c; //counter
	int k = 2; //threshold (usually a small fixed integer, e.g. 1 or 2)
	tau tau_v; //time constant
	int t; //timer in range [0, tau] or [] [tau/2, tau] (depending on sync assumptions)
	static trickle_time tt;

	bool net_changed = false;
	public:
	//Simulating a kind of shared obj
	//needs to be changed in order to manage both sync or notsync
	static trickle_time getCurrentTrickleTime(){
		if(tt)
			tt = new trickle_time(true);
		return tt;
	}

	trickle_time(bool syncNeeded){
		//manage tau
		c = 0;
		tau_v = (syncNeeded)? new tau_to_sync():new tau_sync();
		t = tau_v.getRandomInTheInterval();
	}

	/*
	 * When a mote hears metadata identical to its own, it increments
	 * the constant c
	 */
	void sameMetadata(){
		c++;
	}

	/*
	 * At time t (please also check t), the mote broadcast if this
	 * condition holds.
	 *
	 */
	bool shouldIBroadcast(){
		return (c<k || net_changed)? true: false;
	}

	/*
	 * When the interval of size tau completes, c is reset to zero
	 * and t is reset to a new random value in the range [0, tau]
	 * (if sync assumption holds) or to a new random value in the
	 * range [tau/2, tau] (if sync is needed)
	 */
	void intervalCompleted(){
		c = 0;
		net_changed = false;
		tau_v.tauExpired();
		t = tau_v.getRandomInTheInterval();
	}

	int getOnlyListeningTime(){
		return t;
	}

	/**
	 * My mod: this method should be invoked both when a peer in my
	 * neighbourhood informs me that its neighbourhood changed
	 * and when my neighbourhood change.
	 */
	void netChanged(){
		net_changed = true;
		tau_v.networkChanged();
	}

	int getIntervalLength(){
		return tau_v.getTauValue();
	}

	bool isNetChanged(){
		return net_changed;
	}
};

