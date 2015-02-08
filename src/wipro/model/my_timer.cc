/*
 * timer.cpp
 *
 *  Created on: 02/feb/2015
 *      Author: superjulietta
 */

#include "my_timer.h"


my_timer::my_timer() {
	// TODO Auto-generated constructor stub
	resetted = true;
	running = false;
	beg = 0;
	end = 0;
}

my_timer::~my_timer() {
	// TODO Auto-generated destructor stub
}



void my_timer::start() {
	if(! running) {
		if(resetted)
			beg = (unsigned long) clock();
		else
			beg -= end - (unsigned long) clock();
		running = true;
		resetted = false;
	}
}


void my_timer::stop() {
	if(running) {
		end = (unsigned long) clock();
		running = false;
	}
}


void my_timer::reset() {
	bool wereRunning = running;
	if(wereRunning)
		stop();
	resetted = true;
	beg = 0;
	end = 0;
	if(wereRunning)
		start();
}


bool my_timer::isRunning() {
	return running;
}


unsigned long my_timer::getTime() {
		if(running)
			return ((unsigned long) clock() - beg);
		else
			return end - beg;
}

unsigned long my_timer::getTimeInSeconds(){
		if( running )
			return ((unsigned long) clock() - beg) / CLOCKS_PER_SEC;

		return (end - beg) / CLOCKS_PER_SEC;
}

bool
my_timer::isOver(unsigned long seconds) {
	return seconds >= getTime();
}

