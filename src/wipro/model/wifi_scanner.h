/*
 * wifi_scanner.h
 *
 *  Created on: 02/feb/2015
 *      Author: superjulietta
 */

#ifndef WIFI_SCANNER_H_
#define WIFI_SCANNER_H_

#include "ns3/core-module.h"
#include "trickle_time.h"
#include "timer.h"
#include "wifi_receiver.h"
using namespace ns3;

class wifi_scanner {
private:
	std::string SSID = "";
	//private WifiManager mainWifi;
	wifi_receiver receiver_wifi;
	//private BeaconStuffing bs;
	//private final Handler handler = new Handler();
	trickle_time tt;
	std::timer resultsTimer;
	int interval;
	bool firstExecution = true;

public:
	wifi_scanner();
	virtual ~wifi_scanner();
	void handleMessage();
	void doInback();
};


#endif /* WIFI_SCANNER_H_ */
