/*
 * duty_cycle_application.cpp
 *
 *  Created on: 03/feb/2015
 *      Author: superjulietta
 */

#include "duty_cycle_application.h"

duty_cycle_application::duty_cycle_application() :
    beaconBroadcast(Ipv4Address("255.255.255.255"), 80),
    local(Ipv4Address::GetAny (), 80) {

	scanning = false;
}
duty_cycle_application::~duty_cycle_application() {
	// TODO Auto-generated destructor stub
}
//class duty_cycle_application : public Application {

bool
duty_cycle_application::checkRestartRequired(){
	//result should be updated as soon as they are available
	//Log.i("WifiScanner", " Wifi Updating results");
	//if(mainWifi.isWifiEnabled()==false)
	//    mainWifi.setWifiEnabled(true);
	//mainWifi.startScan();
	//updateResults(receiverWifi.getResults());
	//if i find a new peer I restart with a more intense
	//broadcasting
	if(tt.isNetChanged())
		resultsTimer.reset();
	//resultsTimer.purge();
	//restarting everything
	firstExecution = true;
	//IDEA: add statistics on dropped packets!
}

void
duty_cycle_application::HandleMessage (Ptr<Socket> socket) {
	//just to try until complete code is produced... remove the following line
	scanning = true;
	if(scanning){
	    Ptr< Packet > pp = socket->Recv();
	    unsigned char *data = new unsigned char(pp->GetSize());
	    pp->CopyData (data, pp->GetSize());
		std::string s(data, data+pp->GetSize() );
		NS_LOG_UNCOND ("Received one packet: node "<< socket->GetNode()->GetId() <<" data "  << s <<  " at "<< Simulator::Now ().GetMicroSeconds () << " microseconds");
		checkRestartRequired();
	}

}
	
void
generate_traffic (Ptr<Socket> socket, uint32_t pktSize,
	              uint32_t pktCount, Time pktInterval, bool repeat)	{
	  if (pktCount > 0)  {
		  //Begin packet customization
		  std::stringstream ss;//create a stringstream
		  ss << socket->GetNode()->GetId();//add number to the stream
		  std::string s = "~" + ss.str(); //eventually check for pktSize
	      uint8_t *buffer = (uint8_t*)std::malloc(s.length() + 1);
	      std::memset(buffer, 0, s.length() + 1);
	      int offset = 2;
	      std::memcpy(buffer + offset, s.c_str(), s.length()+1);
		  pktSize = s.length() +2;
		  NS_LOG_UNCOND ("Sending packet whose content is: "<< buffer+offset);
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

	void
	duty_cycle_application::DoGenerate (void)	{

		 //initial delay is considered to avoid colliding nodes
		 Simulator::Schedule(Seconds(m_delay), &generate_traffic, m_socket,packetSize, numPackets, interPacketInterval, true);
		//Simulator::Schedule (Seconds (10.0), &generate_traffic,
        //             m_socket, packetSize, numPackets, interPacketInterval, true);

/*
	  Simulator::Schedule (Seconds (m_delay.GetValue ()),
	                &duty_cycle_application::DoGenerate, this);
	  Ptr<Packet> p = Create<Packet> (1000);
	  m_socket->Send (p);*/
	}

void
duty_cycle_application::doInback() {
    	tt = tt.getCurrentTrickleTime();
    	interval = tt.getIntervalLength();
    	//managing only-listening period

      //  handler.postDelayed(new Runnable() {
      //     public void run() {
         	if(!firstExecution) {
            		//std::cout << "WifiScanner:wifi firstExecution "<<firstExecution<< std::endl;
            		//Log.i("WifiScanner", "wifi firstExecution "+firstExecution);
	           		tt.intervalCompleted();
	         }
	         else {
	         		resultsTimer = new std::timer();
	          		firstExecution = false;
	         	}
	         int onlyListeningTime = tt.getOnlyListeningTime();
	         int rate = (onlyListeningTime>6000)?6000:onlyListeningTime;

	        // Simulator::Schedule(rate, &setScanning);
	         /*************************** UNCOMMENT
	            	resultsTimer.scheduleAtFixedRate(new TimerTask() {

			 }, rate, onlyListeningTime); //min between 6000 and onlyListeningTime
			*****************/
	        //Checking if should I broadcast myself or not at this time
	        tt = tt.getCurrentTrickleTime();
	        if(tt.shouldIBroadcast()){
	            //Log.i("WifiScan", "wifi broadcasting SSID "+SSID);
	            //bs.beaconStuffing(mainWifi, SSID);
	        	Simulator::ScheduleNow(&generate_traffic, m_socket,packetSize, numPackets, interPacketInterval, false);
	         }
			//Log.i("WifiScanner", "Wifi Interval "+interval);
			tt = tt.getCurrentTrickleTime();
	        interval = tt.getIntervalLength();
	        doInback();
	        }
	        //}, interval);
	    //}

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
		r_socket->Listen();
		r_socket->SetRecvCallback (MakeCallback (&HandleMessage));

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
void
duty_cycle_application::StartApplication(){
	DoGenerate();
}

void
duty_cycle_application::setDelay(unsigned int val){
	m_delay = val; //initial delay, otherwise nodes' packets collide
}
void
duty_cycle_application::StopApplication () {

}
