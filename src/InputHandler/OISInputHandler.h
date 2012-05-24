/*
 * OISInputHandler.h
 *
 *  Created on: 28.04.2012
 *      Author: sebastian
 */

#ifndef OISINPUTHANDLER_H_
#define OISINPUTHANDLER_H_

// OIS Includes:
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <vector>

class OISInputHandler :  public OIS::KeyListener {
public:
	OISInputHandler(std::string windowSize);
	virtual ~OISInputHandler();

	void handleWindowResizeEvent(int width, int height);
	void handleWindowClosedEvent();
	void processInput();

	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);

	void addKeyListener(OIS::KeyListener* listener);


private:
    // OIS Input devices
    OIS::InputManager* mpInputManager;
    OIS::Mouse*    mpMouse;
    OIS::Keyboard* mpKeyboard;

    std::vector<OIS::KeyListener*> mKeyListeners;
};


#endif /* OISINPUTHANDLER_H_ */
