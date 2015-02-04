/*
 * wifireceiver.cpp
 *
 *  Created on: 02/feb/2015
 *      Author: superjulietta
 */

#include "wifi_receiver.h"

namespace std {

//wifi_receiver::wifi_receiver() {
	// TODO Auto-generated constructor stub

//}

wifi_receiver::~wifi_receiver() {
	// TODO Auto-generated destructor stub
}
class wifi_receiver {

	private:

	//private HashMap<String, ScanResult> map;
	//private HashMap<String, ScanResult> potentiallyLeft;
	map<string, string> mappina;
	trickle_time tt;

	//Note to my future me: tt passed via constructor is a stupid thing -.-'
	public:
	wifi_receiver(trickle_time t){
		//this.mainWifi = mainWifi;
		//this.map = new HashMap<String, ScanResult>();
		//this.potentiallyLeft = new HashMap<String, ScanResult>();
		tt = t;
	}

	void add_SSID(std::string SSID){

         //Considering only near peers
         if(SSID_formatter::checkFormat(SSID)) {
          		//Log.i("WifiRec", "wifi peer recognized"+SSID);
            	string printable = SSID_formatter::printableSSID(SSID);
	            mappina[printable] = printable;
	            //	if(potentiallyLeft.containsKey(printable))
	            //		potentiallyLeft.remove(printable);
	            //it is a peer, must check if it changed or not
	            tt = trickle_time::getCurrentTrickleTime();
            	if(!SSID_formatter::netChanged(SSID))
            		tt.sameMetadata();
            	else
            		tt.netChanged();
	            	//tt.netChanged();
            	}
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

};
} /* namespace std */



