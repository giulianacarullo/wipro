#include <SSID_formatter.h>

using namespace std;

class SSID_formatter {
  public:
  static std::string format(string SSID){
		if(SSID.size()*2 > 30) //2 bytes needed for the extra char
			throw std::runtime_error("The specified SSID is too long"); 
		return "~"+SSID;
	};
	
  
  static bool checkFormat(string SSID){
		if(SSID.size()*2 > 30 || SSID.compare(0,2,"~*")!=0) 
			return false;
		return true;
	};

  //The last bit indicates if the net is changed or not
  static bool netChanged(string SSID){
	  char net_changed =  SSID[SSID.size()];
	  return net_changed=='0'? false:true;
  		//bs = BitSet.valueOf(("~"+SSID).getBytes());
  		//return bs.get(bs.size());
  	}
  static string printableSSID(string SSID){
  		if(checkFormat(SSID))
  			return SSID.substr(1, SSID.size()-1);
  		return "";
  	}
};

//int main(){}
 

