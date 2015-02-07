#ifndef SSID_FORMATTER_H_
#define SSID_FORMATTER_H_

#include "ns3/core-module.h"

#include <iostream>
#include <string>
#include <stdexcept>

using namespace ns3;

class SSID_formatter {
	private:
		SSID_formatter();
		virtual ~SSID_formatter();
	public:
		static std::string format(std::string SSID);
		static bool checkFormat(std::string SSID);
		static bool netChanged(std::string SSID);
		static std::string printableSSID(std::string SSID);
};

#endif /* SSID_FORMATTER_H_ */

