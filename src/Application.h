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
#include "Renderer/AbstractRenderer.h"
#include "InputHandler/OISInputHandler.h"
#include "GameModel/Tracks/AbstractTrack.h"
#include "GameModel/TrackAtoms/TrackAtom.h"



class AbstractTrack;
class AbstractRenderer;
class AbstractInputHandler;
class Vehicle;
class TrackAtom;







class Application : public OIS::KeyListener {
public:



	Application();
	virtual ~Application();


	//std::vector<CollisionInfo> getCollidingTAs();
	bool handleFrameRenderingQueuedEvent();
	void init();

	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);

	void playTrackFile(std::string filename);

	bool quit;

	AbstractRenderer* getRenderer();


	Vehicle* mpPlayerVehicle;

	AbstractTrack* mpTrack;



private:
	AbstractRenderer* mpRenderer;
	OISInputHandler* mpInputHandler;

};



#endif /* APPLICATION_H_ */
