/*
 * wifireceiver.cpp
 *
 *  Created on: 02/feb/2015
 *      Author: superjulietta
 */

#include "wifi_receiver.h"


wifi_receiver::wifi_receiver() {
	// TODO Auto-generated constructor stub

}

wifi_receiver::~wifi_receiver() {
	// TODO Auto-generated destructor stub
}


	//Note to my future me: tt passed via constructor is a stupid thing -.-'
wifi_receiver::wifi_receiver(trickle_time t){
	//this.mainWifi = mainWifi;
	//this.map = new HashMap<String, ScanResult>();
	//this.potentiallyLeft = new HashMap<String, ScanResult>();
	mappina = std::map<std::string,std::string>();
	settino = std::set<std::string> ();
	tt = t;
}

bool
wifi_receiver::add_SSID(std::string SSID){
     //Considering only near peers
     //if(SSID_formatter::checkFormat(SSID)) { //no need for checkformat! it is just a simulation!
    	 //Log.i("WifiRec", "wifi peer recognized"+SSID);
		//std::cout<<"WR- received SSID" <<SSID;
    	 //std::string printable = SSID_formatter::printableSSID(SSID);
	     //std::string printable = SSID;
	    //mappina.insert(std::pair<std::string,std::string>(SSID,SSID) );
		bool new_insert = false;
		if(settino.count(SSID) == 0){
			new_insert = true;
			settino.insert(SSID);
		}
    	 //mappina[SSID] = SSID;
    	 //std::cout<<"WR "<< SSID <<" - " << printable <<" azz "<<std::endl;
    	 //	if(potentiallyLeft.containsKey(printable))
    	 //		potentiallyLeft.remove(printable);
    	 //it is a peer, must check if it changed or not
    	 tt = tt.getCurrentTrickleTime();
         if(!SSID_formatter::netChanged(SSID)) {
          		tt.sameMetadata();
         }
         else {
          		tt.netChanged();
         }

     //}

     //Log.i("WifiRec", "wifi recognized nets "+SSID);
     //checking for no more available networks
     //adjust it to remove the SSID after n seconds it has not been discovered again
     /*
            for (String SSID:map.keySet()){
            	        	ScanResult net = map.get(SSID);
            	            	if(!wifiList.contains(net)){
            	            		map.remove(net);
            	            		potentiallyLeft.put(SSID, net);
            	            	}
            	 }*/
         return new_insert;
        }
bool
wifi_receiver::contains(std::string SSID){
	if(settino.count(SSID)==0)
		return false;
	return true;
}

void
wifi_receiver::printResults(){
	//std::cout << "Nodes discovered so far: ";
	std::string discovered = "";
	//for(std::map<std::string,std::string>::iterator iter = mappina.begin(); iter != mappina.end(); ++iter)
	for(std::set<std::string>::iterator iter = settino.begin(); iter != settino.end(); ++iter)
	{
		discovered = discovered + iter->data() + " - ";
		//std::cout << iter->first <<" ";
	//ignore value
	//Value v = iter->second;
	}
	NS_LOG_UNCOND( discovered);
	//std::cout << discovered << std::endl;
}
/*
	ArrayList<String> getResults(){
		//check this
		ArrayList<String> list = new ArrayList<String>();
		for(String s:map.keySet()) {
    		String printable = SSIDFormatter.printableSSID(s);
			list.add(printable);
		}
		return list;
	}*/




