/*
 * statistics.cc
 *
 *  Created on: 10/feb/2015
 *      Author: superjulietta
 */

#include "statistics.h"


statistics::statistics() {
	dkp = 0;
	dukp = 0;
	//undropped = 0;
	dropped = std::set<std::string>();
	dropped_but_recognized = std::map<std::string, Time>();
}

statistics::statistics(int id_peer, int numNodes, int simLen) {
	id = id_peer;
	num_nodes = numNodes;
	sim_len = simLen;
	dkp = 0;
	dukp = 0;
	//recognized = 0;
	//undropped = 0;
	recognized = std::set<std::string>();
	dropped = std::set<std::string>();
	dropped_times = std::map<std::string, Time>();
	dropped_but_recognized = std::map<std::string, Time>();
}

statistics::~statistics() {
	// TODO Auto-generated destructor stub
}
void
statistics::addDroppedKnownPacket(){
		dkp++;
}
void
statistics::addDroppedUnknownPacket(std::string ssid){
	dukp++;
	if(dropped.count(ssid) == 0){
		//if(id == 5)
		//	NS_LOG_UNCOND("Dropped : " <<ssid);
		dropped.insert(ssid);
		dropped_times[ssid] =  Simulator::Now();
		//dropped_but_recognized[ssid]= Simulator::Now();
	}
}
void
statistics::addRecognized(std::string ssid){
	if(recognized.count(ssid)==0)
		recognized.insert(ssid);
}
void
statistics::removeIfDropped(std::string ssid){
	if(dropped.count(ssid) != 0){
		//if(id == 5)
		//	NS_LOG_UNCOND("Removing " <<ssid << "from dropped nodes");
		//std::set<std::string>::iterator pos = dropped.find(ssid);
		dropped.erase(ssid); //required to properly manage dropped_but_recognized
		//storing the delay between first receive and current time.
		//computing the single peer discovery time.
		Time now = Simulator::Now();
		//Time delay = Time(now.GetMilliSeconds() - dropped_times[ssid].GetMilliSeconds());
		Time delay = Time::FromDouble(now.GetMilliSeconds() - dropped_times[ssid].GetMilliSeconds(), Time::MS);
		dropped_but_recognized[ssid] = delay;
	}
	//else manage undropped packets PROBABLY THERE IS A BUG!
	//simply undropped = num nodes - not recognized - dropped_but_rec
	else {
		//undropped++;
	}
}

void
statistics::printDropped(){
		std::stringstream convert_id; // stringstream used for the conversion
		convert_id << id;
		std::string ssid = convert_id.str();

		std::string drop= "Node "+ ssid + " - dropped known: ";
		std::stringstream convert;
		convert << dkp;
		drop = drop + convert.str() + " dropped unknown: " ;
		std::stringstream convert_uk;
		convert_uk << dukp;
		drop = drop + convert_uk.str() + " - not discovered peer cause dropped: " ;
		std::stringstream convert_not_recognized;
		convert_not_recognized << dropped.size();
		drop = drop + convert_not_recognized.str() + " - dropped but recognized:  " ;

		std::stringstream convert_drop_rec;
		convert_drop_rec << dropped_but_recognized.size();
				drop = drop + convert_drop_rec.str() + "; " ;
		NS_LOG_UNCOND("STATS - "<< drop);
		printSinglePeerDiscoveryTime();
}

void
statistics::printSinglePeerDiscoveryTime(){
	NS_LOG_UNCOND("STATS - Single Peer Discovery Time for node "<<id);
	for (std::map<std::string, Time>::iterator it=dropped_but_recognized.begin(); it!=dropped_but_recognized.end(); ++it)
		NS_LOG_UNCOND (it->first << " => " << it->second.GetSeconds());
		//NS_LOG_UNCOND (it->first << " => " << it->second.GetMilliSeconds());
}

void
statistics::saveDroppedVaryingNodes(){
	  std::stringstream convert; // stringstream used for the conversion
	  convert << num_nodes;
	  std::string nn = convert.str();

      //FILE *file;
	  std::ofstream outfile;
	  std::string filename("./src/wipro/results/base_protocol/nodes/20seconds_sim/"+nn+"nodes.txt");
	  //outfile.open(filename.c_str(), std::ios::out);

	  //outfile.open(filename.c_str(), std::ios_base::app);
	  outfile.open(filename.c_str(), std::ios::app);

	  //std::ios::app
	  // id, #total recognized #not recognized cause dropped # dropped recognized #rec immediately
	  outfile << id<< " "<<recognized.size()<<" "<<dropped.size()<<" "<<dropped_but_recognized.size()<<" "<<recognized.size()-dropped_but_recognized.size()<<"\n";
	  outfile.close();
}

//values on the main diagonal should be ignored
void
statistics::saveTimings(){
	 	  std::stringstream convert; // stringstream used for the conversion
		  convert << num_nodes;
		  std::string nn = convert.str();

	      //FILE *file;
		  std::ofstream outfile;
		  std::string filename("./src/wipro/results/base_protocol/nodes/20seconds_sim/"+nn+"nodes_timing.txt");
		  //outfile.open(filename.c_str(), std::ios::out);

		  //outfile.open(filename.c_str(), std::ios_base::app);
		  outfile.open(filename.c_str(), std::ios::app);

		  //std::ios::app
		  // id, #total recognized #not recognized cause dropped # dropped recognized #rec immediately
		  std::stringstream ss;//create a stringstream
		  //ss << i;
		  outfile << id <<" ";
		  for (int i = 0; i < num_nodes; i++){
			  ss << i;
			  std::string ssid = ss.str();
			  std::map<std::string, Time>::iterator it= dropped_but_recognized.find(ssid);
			  //If it has not ben dropped but recognized
			  if(it == dropped_but_recognized.end()){
				  //and it is not even ever dropped
				  if(dropped.count(ssid) != 0){
					  outfile << "-1" << " "; // -1 not found cause dropped
				  }
				  else {
					  //and not immediately recognized
					  if(recognized.count(ssid) !=0)
						  outfile << "0" << " "; // 0 for immediately discovered
					  //it means that the peer has been not found at all
					  else // -2 for not found at all
						  outfile << "-2" << " ";
				  }
			  }
			  else { // storing dropped node's timing
				  outfile << it->second.GetSeconds() << " ";
			  }
			  ss.str("");
			  ss.clear();
			  if(i == num_nodes-1)
				  outfile<<"\n";

		  }
		  //outfile << id<< " "<<recognized<<" "<<dropped.size()<<" "<<dropped_but_recognized.size()<<" "<<recognized-dropped_but_recognized.size()<<"\n";
		  outfile.close();
}
