/*
 * LocalHumanPilot.h
 *
 *  Created on: 13.06.2012
 *      Author: sebastian
 */

#ifndef LOCALHUMANPILOT_H_
#define LOCALHUMANPILOT_H_

#include "Pilot.h"
#include "../../util/Input/KeyboardEventListener.h"

class KeyboardEventListener;

class LocalHumanPilot : public Pilot, public KeyboardEventListener {

public:

	LocalHumanPilot();
	virtual ~LocalHumanPilot();

	void command(Key, bool);

	virtual void handleKeyEvent(Key, bool pressed);
	virtual void step();
};

#endif /* LOCALHUMANPILOT_H_ */
