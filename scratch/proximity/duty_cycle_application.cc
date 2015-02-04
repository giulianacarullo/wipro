/*
 * duty_cycle_application.cpp
 *
 *  Created on: 03/feb/2015
 *      Author: superjulietta
 */

#include "duty_cycle_application.h"


duty_cycle_application::duty_cycle_application() {
	// TODO Auto-generated constructor stub

	beaconBroadcast = InetSocketAddress(Ipv4Address("255.255.255.255"), 80);
	local = InetSocketAddress (Ipv4Address::GetAny (), 80);
}

duty_cycle_application::~duty_cycle_application() {
	// TODO Auto-generated destructor stub
}
//class duty_cycle_application : public Application {

	void
	ReceivePacket (Ptr<Socket> socket) {

	    Ptr< Packet > pp = socket->Recv();
	    unsigned char *data = new unsigned char(pp->GetSize());
	    pp->CopyData (data, pp->GetSize());
		std::string s(data, data+pp->GetSize() );
		NS_LOG_UNCOND ("Received one packet: node "<< socket->GetNode()->GetId() <<" data "  << s <<  " at "<< Simulator::Now ().GetMicroSeconds () << " microseconds");
	}

	void
	duty_cycle_application::DoGenerate (void)	{
	 /*
	  m_next = Simulator::Schedule (Seconds (m_delay.GetValue ()),
	                &duty_cycle_application::DoGenerate, this);
	  Ptr<Packet> p = Create<Packet> (m_size.GetIntValue ());
	  m_socket->Send (p);*/
	}

	void
	duty_cycle_application::SetRemote (std::string socketType){
	  TypeId tid = TypeId::LookupByName (socketType);
	  m_socket = Socket::CreateSocket (GetNode (), tid);
	  m_socket->Bind ();
	  //mine
	  m_socket->SetAllowBroadcast(true);
	  //end mine
	  m_socket->ShutdownRecv();
	  m_socket->Connect (beaconBroadcast);
	}

	void
	duty_cycle_application::set_receive(std::string socketType){
		TypeId tid = TypeId::LookupByName (socketType);
		r_socket = Socket::CreateSocket (GetNode(), tid);
		r_socket->Bind (local);
		r_socket->SetRecvCallback (MakeCallback (&ReceivePacket));
	}
	void
	generate_traffic (Ptr<Socket> socket, uint32_t pktSize,
	                             uint32_t pktCount, Time pktInterval, bool repeat)
	{
	  if (pktCount > 0)  {
		  //Begin packet customization
		  std::stringstream ss;//create a stringstream
		  ss << socket->GetNode()->GetId();//add number to the stream
		  std::string s = "~" + ss.str(); //eventually check for pktSize
	      uint8_t *buffer = (uint8_t*)std::malloc(s.length() + 1);
	      std::memset(buffer, 0, s.length() + 1);
	      int offset = 2;
	      std::memcpy(buffer + offset, s.c_str(), s.length()+1);
	      //std::cout << (buffer+offset) << '\n';
		  pktSize = s.length() +2;

		  NS_LOG_UNCOND ("Sending packet whose content is: "<< buffer+offset);
		  //std::cout << "Sending packet whose content is: "<< buffer+offset<< " \n";
		  Packet *p = new Packet(buffer+offset, pktSize);
		  //End packet customization
	      //socket->Send (Create<Packet> (pktSize));
	      socket->Send (p);
	      Simulator::Schedule (pktInterval, &generate_traffic,
	                           socket, pktSize,pktCount-1, pktInterval, repeat);
	    }
	  else
		  if(repeat)
			  Simulator::Schedule (pktInterval, &generate_traffic,
	                     socket, pktSize,pktCount-1, pktInterval, repeat);
		  else {
			  socket->Close ();
		  }
	}
	/*
	TypeId
	GetTypeId (void)
	{
	  static TypeId tid = TypeId ("duty_cycle_application")
	    .SetParent<Application> ()
	    .AddConstructor<duty_cycle_application> ()
	    .AddAttribute ("Delay", "The delay between two packets (s)",
	           RandomVariableValue (ConstantVariable (1.0)),
	           MakeRandomVariableAccessor (&RandomGenerator::m_delay),
	           MakeRandomVariableChecker ())
	    .AddAttribute ("PacketSize", "The size of each packet (bytes)",
	           RandomVariableValue (ConstantVariable (2000)),
	           MakeRandomVariableAccessor (&RandomGenerator::m_size),
	           MakeRandomVariableChecker ())
	    ;
	  return tid;
	}*/
//};
/* namespace ns3 */
int main(){}
