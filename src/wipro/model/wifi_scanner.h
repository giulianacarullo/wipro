/*
 * wifi_scanner.h
 *
 *  Created on: 02/feb/2015
 *      Author: superjulietta
 */

#ifndef WIFI_SCANNER_H_
#define WIFI_SCANNER_H_

#include "ns3/core-module.h"
#include <timer.h>
#include "trickle_time.h"

namespace ns3;

class wifi_scanner {
public:
	wifi_scanner();
	virtual ~wifi_scanner();
};


#endif /* WIFI_SCANNER_H_ */
