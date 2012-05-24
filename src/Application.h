/*
 * Application.h
 *
 *  Created on: 28.04.2012
 *      Author: sebastian
 */

// TODO 4: Remove OIS dependency from Application class by introducing a self-made keyboard event message system

#ifndef APPLICATION_H_
#define APPLICATION_H_


#include <OISEvents.h>
#include <OISKeyboard.h>
#include "GameModel/GameModel.h"
#include "Renderer/AbstractRenderer.h"
#include "InputHandler/OISInputHandler.h"

class AbstractRenderer;
class AbstractInputHandler;


class Application : public OIS::KeyListener {
public:
	Application();
	virtual ~Application();

	bool handleFrameRenderingQueuedEvent();
	void run();

	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);


	bool quit;

	AbstractRenderer* getRenderer();

	GameModel* mpGameModel;

private:
	AbstractRenderer* mpRenderer;
	OISInputHandler* mpInputHandler;

};

#endif /* APPLICATION_H_ */
