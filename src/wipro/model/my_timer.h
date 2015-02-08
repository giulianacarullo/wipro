/*
 * timer.h
 *
 *  Created on: 02/feb/2015
 *      Author: superjulietta
 */

#ifndef MY_TIMER_H_
#define MY_TIMER_H_

//#include "ns3/core-module.h"
#include <iostream>
//#include <conio>
#include <time.h>	// class needs this inclusion

using namespace std;
class my_timer {
public:
	my_timer();
	virtual ~my_timer();
	void           start();
	void           stop();
	void           reset();
	bool           isRunning();
	unsigned long  getTime();
	unsigned long getTimeInSeconds();
	bool           isOver(unsigned long seconds);
private:
	bool           resetted;
	bool           running;
	unsigned long  beg;
	unsigned long  end;
};




#endif /* TIMER_H_ */
