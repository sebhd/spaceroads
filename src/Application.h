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
#include "GameModel/Racer/ReplayRacer.h"

class AbstractRenderer;

class Application : public KeyboardEventListener {

public:

	void run();

	Application();
	virtual ~Application();


	std::vector<CollisionInfo> findCollidingTrackAtoms(Racer*);
	bool handleFrameRenderingQueuedEvent();

	virtual void handleKeyEvent(int, bool pressed);

	void playTrackFile(std::string filename, bool replay);

	void handleKeyboardInput(Key, bool pressed);


	AbstractRenderer* getRenderer();

	bool quit;
	bool watchReplay;

	bool mWantReset;

	unsigned long mStopwatch;

	Track* mpTrack;

	Racer* mLocalPlayerRacer;

	std::vector<Racer::ReplayEntry> mReplay;

private:

	std::vector<Racer::ReplayEntry> loadReplayFromFile(std::string filename);

	void writeReplayToFile(std::vector<Racer::ReplayEntry>);

	AbstractRenderer* mpRenderer;
	InputHandler* mpInputHandler;

};



#endif /* APPLICATION_H_ */
