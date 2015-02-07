/*
 * duty_cyclea_pplication.h
 *
 *  Created on: 03/feb/2015
 *      Author: superjulietta
 */

#ifndef DUTY_CYCLE_APPLICATION_H_
#define DUTY_CYCLE_APPLICATION_H_
//#include <application.h>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/inet-socket-address.h"
//#include "ns3/olsr-helper.h"
//#include "ns3/ipv4-static-routing-helper.h"
//#include "ns3/ipv4-list-routing-helper.h"
#include "trickle_time.h"
#include "timer.h"
#include "wifi_receiver.h"

using namespace ns3;


class duty_cycle_application: public Application {

private:
  uint32_t m_delay;
  bool scanning;
  //RandomVariable m_size;
  Ptr<Socket> m_socket;
  Ptr<Socket> r_socket;
  InetSocketAddress beaconBroadcast;
  InetSocketAddress local;

    wifi_receiver receiver_wifi;
  	trickle_time tt;
  	std::timer resultsTimer;
  	int interval;
  	bool firstExecution = true;
  	bool checkRestartRequired();
  	void HandleMessage (Ptr<Socket> socket);
  	void doInback();
  	//Managing packets
  	uint32_t packetSize = 1000; // bytes
  	uint32_t numPackets = 1;
  	Time interPacketInterval = Seconds (1.0);
  virtual void StartApplication (void);
  virtual void StopApplication (void);
  void flipScanning();
  //void generate_traffic (Ptr<Socket> socket, uint32_t pktSize,
//	              uint32_t pktCount, Time pktInterval, bool repeat);
  //ObjectFactory m_factory;

public:
  duty_cycle_application();
	  //InetSocketAddress beaconBroadcast = InetSocketAddress(Ipv4Address("255.255.255.255"), 80);
	  //duty_cycle_application::local = InetSocketAddress (Ipv4Address::GetAny (), 80);

  virtual ~duty_cycle_application();
  //void SetDelay (RandomVariable delay);
  //void SetSize (RandomVariable size);
  void SetRemote (std::string socketType);
  void set_receive(std::string socketType);
  void setDelay(unsigned int val);
  void generate_traffic (Ptr<Socket> socket, uint32_t pktSize, uint32_t pktCount, Time pktInterval, bool repeat);

};
/* namespace ns3 */

#endif /* DUTY_CYCLE_APPLICATION_H_ */
