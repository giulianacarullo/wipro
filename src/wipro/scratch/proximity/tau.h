
#ifndef TAU_H_
#define TAU_H_
	/**
	 * Sync assumption holds
	 * @author giucar
	 *
	 */

#include <iostream>
#include <string>

class tau{
public:
	tau();
	virtual ~tau();
	void networkChanged();
	int getTauValue();
	void tauExpired();
	virtual int getRandomInTheInterval();
};

#endif /* TAU_H_ */
