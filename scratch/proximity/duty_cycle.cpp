/*
 * duty_cycle.cpp
 *
 *  Created on: 03/feb/2015
 *      Author: superjulietta
 */

#include "duty_cycle.h"

namespace ns3 {
/*
duty_cycle::duty_cycle() {
	// TODO Auto-generated constructor stub

}*/

duty_cycle::~duty_cycle() {
	// TODO Auto-generated destructor stub
}
class duty_cycle {
	private:
		std::map<Ptr<Node>, std::string> nodes; //node, status
	public:
		static std::string GOSSIP = 'gossip';
		static std::string SCAN = 'scan';
		static std::string SLEEP = 'sleep';

	//Constructor
		duty_cycle(NodeContainer nc){
			for (NodeContainer::Iterator j = nc.Begin (); j != nc.End (); ++j) {
			          Ptr<Node> object = *j;
			          //Initially all the nodes are in sleep mode
			          nodes[object]=SLEEP;
			   }
		}

		/*
		 * Nodes always gossip themselves in order to facilitate statistics
		 * but their state changes
		 */


};
} /* namespace std */
