/*
 * Application.h
 *
 *  Created on: 28.04.2012
 *      Author: sebastian
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <OISEvents.h>
#include <OISKeyboard.h>
#include "Input/InputHandler.h"
#include "GameModel/Track/Track.h"
#include "GameModel/Racer/LocalHumanRacer.h"

class AbstractRenderer;

class Application : public KeyboardEventListener {

public:

	Application();
	virtual ~Application();


	std::vector<CollisionInfo> findCollidingTrackAtoms();
	bool handleFrameRenderingQueuedEvent();

	virtual void handleKeyEvent(Key, bool pressed);

	void playTrackFile(std::string filename);

	void handleKeyboardInput(Key, bool pressed);


	AbstractRenderer* getRenderer();

	bool quit;

	LocalHumanRacer* mpPlayerVehicle;

	Track* mpTrack;


private:
	AbstractRenderer* mpRenderer;
	InputHandler* mpInputHandler;

};



#endif /* APPLICATION_H_ */
