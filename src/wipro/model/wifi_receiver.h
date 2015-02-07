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

using namespace ns3;

class wifi_receiver {
private:
//private HashMap<String, ScanResult> map;
//private HashMap<String, ScanResult> potentiallyLeft;
std::map<std::string, std::string> mappina;
trickle_time tt;

public:
	wifi_receiver();
	virtual ~wifi_receiver();
	wifi_receiver(trickle_time t);
	void add_SSID(std::string SSID);
};


#endif /* WIFIRECEIVER_H_ */
