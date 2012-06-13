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
//#include "Renderer/AbstractRenderer.h"
#include "Input/InputHandler.h"
#include "Input/KeyboardEventListener.h"
#include "GameModel/Tracks/Track.h"
#include "GameModel/TrackAtoms/TrackAtom.h"

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

	Racer* mpPlayerVehicle;

	Track* mpTrack;


private:
	AbstractRenderer* mpRenderer;
	InputHandler* mpInputHandler;

};



#endif /* APPLICATION_H_ */
