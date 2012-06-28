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

	void playTrackFile(std::string filename);

	void handleKeyboardInput(Key, bool pressed);


	AbstractRenderer* getRenderer();

	bool quit;
	bool stopPlayingTrack;
	bool watchReplay;


	std::vector<Racer*> m_racers;

	Track* mpTrack;

	Racer* mLocalPlayerRacer;
	ReplayRacer* mReplayRacer;

	std::vector<Racer::ReplayEntry> mReplay;

private:
	void doRacerStep(Racer*);

	std::vector<Racer::ReplayEntry> loadReplayFromFile(std::string filename);

	void writeReplayToFile(std::vector<Racer::ReplayEntry>);

	AbstractRenderer* mpRenderer;
	InputHandler* mpInputHandler;

};



#endif /* APPLICATION_H_ */
