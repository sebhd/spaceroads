/*
 * AbstractInputHandler.h
 *
 *  Created on: 29.04.2012
 *      Author: sebastian
 */

#ifndef ABSTRACTINPUTHANDLER_H_
#define ABSTRACTINPUTHANDLER_H_

#include "KeyboardEventListener.h"
#include <vector>


class InputHandler {
public:

	InputHandler();
	virtual ~InputHandler();
	virtual void getInput() = 0;

	std::vector<KeyboardEventListener*> mListeners;
};

#endif /* ABSTRACTINPUTHANDLER_H_ */
