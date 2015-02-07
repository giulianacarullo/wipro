/*
 * timer.h
 *
 *  Created on: 02/feb/2015
 *      Author: superjulietta
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "ns3/core-module.h"
#include <iostream>
//#include <conio>
#include <time.h>	// class needs this inclusion

using namespace ns3;

class timer {
public:
	timer();
	virtual ~timer();
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
