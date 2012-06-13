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
//#include "Renderer/AbstractRenderer.h"
#include "Input/InputHandler.h"
#include "Input/KeyboardEventListener.h"
#include "GameModel/Tracks/Track.h"
#include "GameModel/TrackAtoms/TrackAtom.h"
#include "GameModel/Pilots/LocalHumanPilot.h"

class AbstractRenderer;
class KeyboardEventListener;


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

	Vehicle* mpPlayerVehicle;

	Track* mpTrack;

	LocalHumanPilot mPilot;

private:
	AbstractRenderer* mpRenderer;
	InputHandler* mpInputHandler;

};



#endif /* APPLICATION_H_ */
