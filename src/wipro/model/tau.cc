
#include "tau.h"


tau::tau(){
			//tl = 1000; // default value (time in milliseconds) 1 s
			//tu = 3600000; //default value 1 h
			tl = 1; //1 s
			tu = 3600; //default 1 h
			tau_value = tl;
}
		
tau::tau(int tl_ms, int tu_ms){
		tl = tl_ms;
		tu = tu_ms;
		tau_value = tl;
}
tau::~tau(){

}
		/*
		 * When a mote hears a summary with newer data, it resets
		 * tau to its lower bound. This also happens when a mote 
		 * installs new software. 
		 * In terms of proximity this means that, I should reset tau (and
		 * therefore start announcing me more frequently) when it happens 
		 * either:
		 * 1) a peer in my neighbourhood change its neighbourhood;
		 * 2) I (as peer) change my neighbourhood.
		 * 
		 * Indeed, in both cases it may be the case of the discovery of 
		 * new peers. Indeed, in case 1, it may happen that a peer found
		 * a new peer, therefore I should announce myself more frequently 
		 * to be sure that the newcomer eventually senses my existence. 
		 * 
		 * The case 2 is almost the same, except the fact that I found a new
		 * peer and then I should notify other peers about a possibility of
		 * a newcomer in their networks, and also to notify the newcomer about
		 * my existence.
		 */
void
tau::networkChanged(){
		tau_value = tl;
}
		
		/*
		 * When tau expires it double up to its upper bound
		 */
void
	tau::tauExpired(){
		tau_value = (tau_value < tu)?tau_value*2: tau_value;
}
		
		
int
tau::getTauValue(){
		return tau_value;
}
		
int tau::getRandomInTheInterval(){
	return 0;
}

