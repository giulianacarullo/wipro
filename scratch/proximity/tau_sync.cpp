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
		return rand() % getTauValue() + 1;
	}
};
