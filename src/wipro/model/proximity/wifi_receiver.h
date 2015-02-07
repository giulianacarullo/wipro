/*
 * wifireceiver.h
 *
 *  Created on: 02/feb/2015
 *      Author: superjulietta
 */

#ifndef WIFIRECEIVER_H_
#define WIFIRECEIVER_H_
#include <trickle_time.h>
#include <SSID_formatter.h>
#include <map>

namespace std {

class wifi_receiver {
public:
	wifi_receiver();
	virtual ~wifi_receiver();
};

} /* namespace std */

#endif /* WIFIRECEIVER_H_ */
