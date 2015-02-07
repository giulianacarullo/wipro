#include "SSID_formatter.h"

SSID_formatter::SSID_formatter(){}
SSID_formatter::~SSID_formatter(){}

std::string SSID_formatter::format(std::string SSID){
	if(SSID.size()*2 > 30) //2 bytes needed for the extra char
		throw std::runtime_error("The specified SSID is too long");
	return "~"+SSID;
}
	
  
bool SSID_formatter::checkFormat(std::string SSID){
		if(SSID.size()*2 > 30 || SSID.compare(0,2,"~*")!=0) 
			return false;
		return true;
}

//The last bit indicates if the net is changed or not
bool SSID_formatter::netChanged(std::string SSID){
	  char net_changed =  SSID[SSID.size()];
	  return net_changed=='0'? false:true;
  		//bs = BitSet.valueOf(("~"+SSID).getBytes());
  		//return bs.get(bs.size());
}
std::string printableSSID(std::string SSID){
  		if(SSID_formatter::checkFormat(SSID))
  			return SSID.substr(1, SSID.size()-1);
  		return "";
}

