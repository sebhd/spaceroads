/*
 * LocalHumanRacer.h
 *
 *  Created on: 13.06.2012
 *      Author: sebastian
 */

#ifndef LOCALHUMANRACER_H_
#define LOCALHUMANRACER_H_

#include "Racer.h"
#include "../../util/Input/KeyboardEventListener.h"

class LocalHumanRacer: public Racer, public KeyboardEventListener {
public:
	LocalHumanRacer();
	virtual ~LocalHumanRacer();

	void handleKeyEvent(int, bool pressed);
};

#endif /* LOCALHUMANRACER_H_ */
