/*
 * duty_cycle_application_helper.cpp
 *
 *  Created on: 03/feb/2015
 *      Author: superjulietta
 */

#include "duty_cycle_application_helper.h"


duty_cycle_application_helper::duty_cycle_application_helper() {
	  	// TODO Auto-generated constructor stub

 }

duty_cycle_application_helper::~duty_cycle_application_helper() {}



duty_cycle_application_helper::duty_cycle_application_helper (std::string protocol){
	  //duty_cycle_application_helper::duty_cycle_application_helper();
	  m_protocol = protocol;
  }

ApplicationContainer
duty_cycle_application_helper::Install (NodeContainer nodes){
	    ApplicationContainer applications;
	    Ptr<UniformRandomVariable> rv = CreateObject<UniformRandomVariable> ();
	    for (NodeContainer::Iterator i = nodes.Begin (); i != nodes.End (); ++i) {
	        Ptr<duty_cycle_application> app = CreateObject<duty_cycle_application> ();
	        //app->SetSize (m_packetSize);
	        //app->SetDelay (m_delay);
	       // std::cout<<"Here\n";
	       
	        (*i)->AddApplication (app);
	        app->setDelay(rv->GetInteger(0,3));
	        app->SetRemote (m_protocol);
	        app->set_receive(m_protocol);
	        applications.Add (app);
	      }
	    return applications;
}
	  /*
	  void
	  duty_cycle_application_helper::SetAttribute (std::string name, const AttributeValue &value){
	    m_factory.Set (name, value);
	  }*/

