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


using namespace ns3;


class duty_cycle_application: public Application {

private:
  RandomVariable m_delay;
  //RandomVariable m_size;
  Ptr<Socket> m_socket;
  Ptr<Socket> r_socket;
  static InetSocketAddress beaconBroadcast;
  static InetSocketAddress local;
  virtual void StartApplication (void);
  virtual void StopApplication (void);
  void DoGenerate (void);
  //ObjectFactory m_factory;

public:
  duty_cycle_application() {
	  beaconBroadcast = InetSocketAddress(Ipv4Address("255.255.255.255"), 80);
	  local = InetSocketAddress (Ipv4Address::GetAny (), 80);
  };
  virtual ~duty_cycle_application();
  void SetDelay (RandomVariable delay);
  //void SetSize (RandomVariable size);
  void SetRemote (std::string socketType);
  void set_receive(std::string socketType);

};
/* namespace ns3 */

#endif /* DUTY_CYCLE_APPLICATION_H_ */
