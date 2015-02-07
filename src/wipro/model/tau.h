
#ifndef TAU_H_
#define TAU_H_
	/**
	 * Sync assumption holds
	 * @author giucar
	 *
	 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/inet-socket-address.h"
#include <iostream>
#include <string>

using namespace ns3;

class tau {
protected:
		int tl; //tau lower bound
		int tu; //tau upper bound
		int tau_value;
public:
	tau();
	tau(int tl_ms, int tu_ms);
	virtual ~tau();
	void networkChanged();
	int getTauValue();
	void tauExpired();
	int getRandomInTheInterval();
};

#endif /* TAU_H_ */
