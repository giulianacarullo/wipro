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
#include "my_timer.h"
#include "wifi_receiver.h"
using namespace ns3;

class wifi_scanner {
private:
	string SSID;
	//private WifiManager mainWifi;
	wifi_receiver receiver_wifi;
	//private BeaconStuffing bs;
	//private final Handler handler = new Handler();
	trickle_time tt;
	my_timer resultsTimer;
	int interval;
	bool firstExecution;

public:
	wifi_scanner();
	virtual ~wifi_scanner();
	void handleMessage();
	void doInback();
};


#endif /* WIFI_SCANNER_H_ */
