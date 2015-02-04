/*
 * wifi_scanner.cpp
 *
 *  Created on: 02/feb/2015
 *      Author: superjulietta
 */

#include "wifi_scanner.h"

namespace std {
/*
wifi_scanner::wifi_scanner() {
	// TODO Auto-generated constructor stub

}*/

wifi_scanner::~wifi_scanner() {
	// TODO Auto-generated destructor stub
}
class wifi_scanner {

	private:
		string SSID = "";
		//private WifiManager mainWifi;
		wifi_receiver receiver_wifi;
		//private BeaconStuffing bs;
		//private final Handler handler = new Handler();
		trickle_time tt;
		timer resultsTimer;
		int interval;
		bool firstExecution = true;


	public:
        void handleMessage() {
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
        }

	/*
    void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        inputSSID();
        mainWifi = (WifiManager) getSystemService(Context.WIFI_SERVICE);

        receiverWifi = new WifiReceiver(mainWifi,tt);
        //receiverWifi.goAsync();
        registerReceiver(receiverWifi, new IntentFilter(
                WifiManager.SCAN_RESULTS_AVAILABLE_ACTION)); //try new IntentFilter(Intent.ACTION_TIME_TICK)
        if(mainWifi.isWifiEnabled()==false)
            mainWifi.setWifiEnabled(true);
        //Initiate a scan
        mainWifi.startScan(); //new doInBack()
        bs = new BeaconStuffing(this, mainWifi);
       	doInback();
       	//updateResults(receiverWifi.getResults()); //new doInBack()

    }*/



    void doInback() {

    	tt = trickle_time::getCurrentTrickleTime();
    	interval = tt.getIntervalLength();
    	//managing only-listening period

      //  handler.postDelayed(new Runnable() {

       //     public void run() {
            	if(!firstExecution) {
            		std::cout << "WifiScanner:wifi firstExecution "<<firstExecution<< std::endl;
            		//Log.i("WifiScanner", "wifi firstExecution "+firstExecution);
            		tt.intervalCompleted();
            	}
            	else {
            		resultsTimer = new timer();
            		firstExecution = false;
            	}
                int onlyListeningTime = tt.getOnlyListeningTime();
                int rate = (onlyListeningTime>6000)?6000:onlyListeningTime;
                //Log.i("WifiScanner", "Wifi Scanning and Updating");
                /*************************** UNCOMMENT
               	resultsTimer.scheduleAtFixedRate(new TimerTask() {

				}, rate, onlyListeningTime); //min between 6000 and onlyListeningTime
				*****************/
                //Checking if should I broadcast myself or not at this time
                tt = trickle_time::getCurrentTrickleTime();
                if(tt.shouldIBroadcast()){
                    //Log.i("WifiScan", "wifi broadcasting SSID "+SSID);
                    //bs.beaconStuffing(mainWifi, SSID);
                	//send message
                }
				//Log.i("WifiScanner", "Wifi Interval "+interval);
				tt = trickle_time::getCurrentTrickleTime();
                interval = tt.getIntervalLength();
                doInback();
            }
        //}, interval);
    //}

/*
    void updateResults(ArrayList<string> connections){
    	//Log.i("wifiscanner", "connections "+connections.size());
    	ListView list = (ListView)findViewById(R.id.netList);
        list.setAdapter(new ArrayAdapter<String>(getApplicationContext(),
        android.R.layout.simple_list_item_1,connections));

        ((ArrayAdapter) list.getAdapter()).notifyDataSetChanged();
        //Avoids crashes but does not work xD
        list.requestLayout();
    }*/



};
} /* namespace std */
int main(){}
