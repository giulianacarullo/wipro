/*
 * tausync.cpp
 *
 *  Created on: 31/gen/2015
 *      Author: superjulietta
 */

#include "tau_to_sync.h"

tau_to_sync::tau_to_sync() {
	// TODO Auto-generated constructor stub

}

tau_to_sync::~tau_to_sync() {
	// TODO Auto-generated destructor stub
}

int
tau_to_sync::getRandomInTheInterval(){
		//return tau_value /2 + (int)(Math.random() * ((tau_value-(tau_value/2)) + 1));

		int tau_value = tau::getTauValue();
		NS_LOG_UNCOND("Current tau value: "<< tau_value);
		return tau_value/2 + (rand() % tau_value - (tau_value/2) + 1);

}
