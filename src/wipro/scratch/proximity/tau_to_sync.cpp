/*
 * tausync.cpp
 *
 *  Created on: 31/gen/2015
 *      Author: superjulietta
 */

#include "tau_sync.h"

tau_sync::tau_sync() {
	// TODO Auto-generated constructor stub

}

tau_sync::~tau_sync() {
	// TODO Auto-generated destructor stub
}
class tau_sync : public tau {
	int getRandomInTheInterval(){
		//return tau_value /2 + (int)(Math.random() * ((tau_value-(tau_value/2)) + 1));
		int tau_value = getTauValue();
		return tau_value/2 + (rand % tau_value- (tau_value/2) + 1);

	}
};
