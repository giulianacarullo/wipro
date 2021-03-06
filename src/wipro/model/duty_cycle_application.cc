/*
 * duty_cycle_application.cpp
 *
 *  Created on: 03/feb/2015
 *      Author: superjulietta
 */

#include "duty_cycle_application.h"

duty_cycle_application::duty_cycle_application(int numNodes, int simLen) :
    beaconBroadcast(Ipv4Address("255.255.255.255"), 80),
    local(Ipv4Address::GetAny (), 80) {
	firstExecution = true;
	num_nodes = numNodes;
	sim_len = simLen;
	scanning = false;
	packetSize = 1000; // bytes
	numPackets = 1;
    interPacketInterval = Seconds (1.0);
    tt = trickle_time(true);
    receiver_wifi = wifi_receiver(tt);
}

duty_cycle_application::~duty_cycle_application() {
	// TODO Auto-generated destructor stub
}
//class duty_cycle_application : public Application {

void
duty_cycle_application::checkRestartRequired(){
	//result should be updated as soon as they are available
	//Log.i("WifiScanner", " Wifi Updating results");
	//if(mainWifi.isWifiEnabled()==false)
	//    mainWifi.setWifiEnabled(true);
	//mainWifi.startScan();
	//updateResults(receiverWifi.getResults());
	//if i find a new peer I restart with a more intense
	//broadcasting
	if(tt.isNetChanged()) {
		resultsTimer.reset();
		//resultsTimer.purge();
		//restarting everything
		firstExecution = true;
	}

}

void
generate_traffic (Ptr<Socket> socket, duty_cycle_application *dca,
		uint32_t pktCount, Time pktInterval, bool netchanged)	{

	  if (pktCount > 0)  {
		  if(!dca->isScanning()){
			  //Begin packet custom http://pastebin.com/WHnAgaakmization
			  std::stringstream ss;//create a stringstream
			  ss << socket->GetNode()->GetId();//add number to the stream
			  //std::string s = "~" + ss.str(); //eventually check for pktSize
			  std::string s = ss.str();
			  uint8_t *buffer = (uint8_t*)std::malloc(s.length() + 2);
			  std::memset(buffer, 0, s.length() + 2);
			  int offset = 2;
			  std::memcpy(buffer + offset, s.c_str(), s.length()+1);
			  if(netchanged)
				  std::memcpy(buffer + offset+1, "0", sizeof(netchanged));
			  else
				  std::memcpy(buffer + offset+1, "1", sizeof(netchanged));
			  int pktSize = s.length() +1;
			  //NS_LOG_UNCOND ("Sending packet whose content is: "<< buffer+offset);
			  Packet *p = new Packet(buffer+offset, pktSize);
			  //End packet customization
			  //socket->Send (Create<Packet> (pktSize));
			  socket->Send (p);
			  Simulator::Schedule (pktInterval, &generate_traffic,
								   socket, dca, pktCount-1, pktInterval, netchanged);
		  }
	    }
	  /*
	  else
		  if(repeat) {
			  if(!dca->isScanning()){
				  //NS_LOG_UNCOND ("still entering?");
				  Simulator::Schedule (pktInterval, &generate_traffic,
	                     socket, dca,pktCount-1, pktInterval, repeat);
			  }
		  }
		  else {
			  socket->Close ();
		  }
	*/
}
void
duty_cycle_application::flipScanning(){
	//NS_LOG_UNCOND (GetNode( )->GetId()<<" is FLIP: "<<scanning);
	scanning = ! scanning;
}
void
duty_cycle_application::HandleMessage (Ptr<Socket> r_socket) {
	Ptr< Packet > pp = r_socket->Recv();
	unsigned char *data = new unsigned char(pp->GetSize());
	pp->CopyData (data, pp->GetSize());
	std::string s(data, data+pp->GetSize());
	std::string ssid = s.substr(0, s.size()-1);
	std::string netchanged = s.substr(s.size()-1, s.size());
	if(netchanged=="1")
		tt.netChanged();
	if(scanning){
		//if(GetNode()->GetId() == 5)
		//	NS_LOG_UNCOND(GetNode()->GetId()<<" recognized a new peer: "<< ssid);
		if(receiver_wifi.add_SSID(ssid))
			stats.addRecognized(ssid);
		stats.removeIfDropped(ssid);
		checkRestartRequired();
	}
	else {
		//Adding statistics on dropped packets! (including end peers but dropped (bad) or old peer dropped (good)
		if(receiver_wifi.contains(ssid)){
			stats.addDroppedKnownPacket();
			//NS_LOG_DEBUG(GetNode()->GetId()<<" Dropped packet cause of scanning - already known");
		}
		else{
			stats.addDroppedUnknownPacket(ssid);
			//if(GetNode()->GetId() == 5)
			//	NS_LOG_UNCOND(GetNode()->GetId()<<" Dropped packet " << ssid <<" cause of scanning - not known");
			//NS_LOG_DEBUG(GetNode()->GetId()<<" Dropped packet cause of scanning - not known");
		}
	}
}
	
/*
void
duty_cycle_application::generate_traffic (Ptr<Socket> socket,
		uint32_t pktCount, Time pktInterval, bool repeat)	{
	  //if(!scanning){
	  if (pktCount > 0)  {
		  //Begin packet custohttp://pastebin.com/WHnAgaakmization
		  std::stringstream ss;//create a stringstream
		  ss << socket->GetNode()->GetId();//add number to the stream
		  std::string s = "~" + ss.str(); //eventually check for pktSize
	      uint8_t *buffer = (uint8_t*)std::malloc(s.length() + 1);
	      std::memset(buffer, 0, s.length() + 1);
	      int offset = 2;
	      std::memcpy(buffer + offset, s.c_str(), s.length()+1);
		  int pktSize = s.length() +2;
		  NS_LOG_UNCOND ("Sending packet whose content is: "<< buffer+offset);
		  Packet *p = new Packet(buffer+offset, pktSize);
		  //End packet customization
	      //socket->Send (Create<Packet> (pktSize));
	      socket->Send (p);
	      Simulator::Schedule (pktInterval, &duty_cycle_application::generate_traffic,
	                           socket, pktCount-1, pktInterval, repeat);

	    }
	  else
		  if(repeat) {
			  NS_LOG_UNCOND ("still entering?");
			  Simulator::Schedule (pktInterval, &duty_cycle_application::generate_traffic,
	                     socket, pktCount-1, pktInterval, repeat);
		  }
		  else {
			  socket->Close ();
		  }
	 // }
}*/



void
duty_cycle_application::doInback() {

	//Simulator::Schedule (Seconds(2.0), &duty_cycle_application::gen_traffic, m_socket);
		//scanning = true; //when the interval starts, the peer is in scan state
    	tt = tt.getCurrentTrickleTime();
    	tt.intervalCompleted(); //check if it is correct to call this method here
    	interval = tt.getIntervalLength();
    	//managing only-listening period

      //  handler.postDelayed(new Runnable() {
      //     public void run() {
         	if(!firstExecution) {
            		//Log.i("WifiScanner", "wifi firstExecution "+firstExecution);
	           		tt.intervalCompleted();
	         }
	         else {
	         		resultsTimer = my_timer();
	          		firstExecution = false;
	         }
	         int onlyListeningTime = tt.getOnlyListeningTime();
	         //rate is the delay in milliseconds before task is to be executed.
	         int rate = 1;
	         interval = tt.getIntervalLength();
	         //int rate = (onlyListeningTime>6)?6:onlyListeningTime;//min between 6000 and onlyListeningTime
	         //NS_LOG_UNCOND(GetNode( )->GetId()<<" - Interval size: "<< interval <<" scanning for "<<onlyListeningTime <<" seconds");
	         //Flipping scanning after rate seconds, which means we are entering the scan state
	         Simulator::Schedule(Seconds(rate), &duty_cycle_application::flipScanning,this);
	         // exiting the scan state after onlyLusteningTime
	         Simulator::Schedule(Seconds(onlyListeningTime), &duty_cycle_application::flipScanning,this);
	        //Checking if should I broadcast myself or not at this time
	        //tt = tt.getCurrentTrickleTime();
	        if(tt.shouldIBroadcast()){
	        	//NS_LOG_UNCOND("Should I broadcast!");
	        	numPackets = 3; //sending 4 packets, one for every second
	        	Simulator::ScheduleNow(&generate_traffic, m_socket,this, numPackets, interPacketInterval, tt.isNetChanged());
	         }
				////////////////////OLD code
				//tt = tt.getCurrentTrickleTime();
				//interval = tt.getIntervalLength();
				// NS_LOG_UNCOND("Interval size: "<< interval);
				//////////////////end OLD code
	       // doInback();//???? not sure

	        //}, interval);
			//substituting postDelay, interval
	        //Simulator::ScheduleNow(&generate_traffic, m_socket,this, interval, interPacketInterval, false);
			//Sleeping for the remaining part of the interval
	        Simulator::Schedule(Seconds(interval), &duty_cycle_application::doInback, this);
		}
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
		r_socket->SetRecvCallback (MakeCallback (&duty_cycle_application::HandleMessage, this));

	}

void
duty_cycle_application::StartApplication(){
	stats = statistics(GetNode()->GetId(), num_nodes, sim_len);

	Simulator::Schedule(Seconds(m_delay), &duty_cycle_application::doInback, this);
	//Simulator::Schedule(Seconds (90.0), &duty_cycle_application::StopApplication,this);
}

void
duty_cycle_application::setDelay(unsigned int val){
	m_delay = val; //initial delay, otherwise nodes' packets collide
}
void
duty_cycle_application::StopApplication () {
	//NS_LOG_UNCOND("\nNode "<<GetNode()->GetId() << " recognized: ");
	receiver_wifi.printResults();
	stats.printDropped();
	stats.saveDroppedVaryingNodes();
	stats.saveTimings();
}
