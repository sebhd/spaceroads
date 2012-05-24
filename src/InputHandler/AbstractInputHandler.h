/*
 * AbstractInputHandler.h
 *
 *  Created on: 29.04.2012
 *      Author: sebastian
 */

#ifndef ABSTRACTINPUTHANDLER_H_
#define ABSTRACTINPUTHANDLER_H_

#include "Application.h"

class Application;


class AbstractInputHandler {
public:
	AbstractInputHandler(Application* app);
	virtual ~AbstractInputHandler();

	virtual void processInput() = 0;

protected:
    Application* mpApp;

};

#endif /* ABSTRACTINPUTHANDLER_H_ */
