/*
 * InputEventHandler.h
 *
 *  Created on: 13.06.2012
 *      Author: sebastian
 */

#ifndef INPUTEVENTHANDLER_H_
#define INPUTEVENTHANDLER_H_


class KeyboardEventListener {
public:
	enum Key {
			KEY_ESCAPE,
			KEY_DOWN,
			KEY_LEFT,
			KEY_RIGHT,
			KEY_SPACE,
			KEY_UP
		};

	KeyboardEventListener();
	virtual ~KeyboardEventListener();

	virtual void handleKeyEvent(Key, bool pressed) = 0;
};

#endif /* INPUTEVENTHANDLER_H_ */
