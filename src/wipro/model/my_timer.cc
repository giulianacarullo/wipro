/*
 * timer.cpp
 *
 *  Created on: 02/feb/2015
 *      Author: superjulietta
 */

#include "my_timer.h"


timer::timer() {
	// TODO Auto-generated constructor stub
	resetted = true;
	running = false;
	beg = 0;
	end = 0;
}

timer::~timer() {
	// TODO Auto-generated destructor stub
}



void timer::start() {
	if(! running) {
		if(resetted)
			beg = (unsigned long) clock();
		else
			beg -= end - (unsigned long) clock();
		running = true;
		resetted = false;
	}
}


void timer::stop() {
	if(running) {
		end = (unsigned long) clock();
		running = false;
	}
}


void timer::reset() {
	bool wereRunning = running;
	if(wereRunning)
		stop();
	resetted = true;
	beg = 0;
	end = 0;
	if(wereRunning)
		start();
}


bool timer::isRunning() {
	return running;
}


unsigned long timer::getTime() {
		if(running)
			return ((unsigned long) clock() - beg);
		else
			return end - beg;
}

unsigned long timer::getTimeInSeconds(){
		if(running)
			return ((unsigned long) clock() - beg) / std::CLOCKS_PER_SEC;
		else
			return (end - beg) / CLOCKS_PER_SEC;
}

bool
timer::isOver(unsigned long seconds) {
	return seconds >= getTime();
}

