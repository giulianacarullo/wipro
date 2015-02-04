/*
 * duty_cycle_application_helper.h
 *
 *  Created on: 03/feb/2015
 *      Author: superjulietta
 */

#ifndef DUTY_CYCLE_APPLICATION_HELPER_H_H
#define DUTY_CYCLE_APPLICATION_HELPER_H_H
#include "duty_cycle_application.h"
/*#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/config-store-module.h"
#include "ns3/wifi-module.h"*/
using namespace ns3;


class duty_cycle_application_helper {
	private:
		  std::string m_protocol;
		  Address m_remote;
		  RandomVariable m_packetSize;
		  RandomVariable m_delay;
		  //ObjectFactory m_factory;
	public:
		duty_cycle_application_helper();
		duty_cycle_application_helper(std::string protocol);
		virtual ~duty_cycle_application_helper();
		ApplicationContainer Install (NodeContainer nodes);
};
 /* namespace ns3 */

#endif /* DUTY_CYCLE_APPLICATION_HELPER_H_ */
