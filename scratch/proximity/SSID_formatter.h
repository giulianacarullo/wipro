#ifndef SSIDFORMATTER_H_
#define SSIDFORMATTER_H_

#include <iostream>
#include <string>
#include <stdexcept>

namespace std {
	class SSID_formatter {
	public:
		SSID_formatter();
		virtual ~SSID_formatter();
		static string format(string SSID);
		static bool checkFormat(string SSID);
		static bool netChanged(string SSID);
		static string printableSSID(string SSID);
	};
}
#endif /* SSIDFORMATTER_H_ */

