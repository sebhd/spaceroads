/*
 * main.h
 *
 *  Created on: 24.04.2012
 *      Author: sebastian
 */

#ifndef OGRERENDERER_H_
#define OGRERENDERER_H_

// OGRE Includes:
#include "../AbstractRenderer.h"
#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>
#include "../../GameModel/Track/TrackAtom.h"
#include "OGRERendererRacer.h"
#include "Gorilla.h"

class OgreRenderer: public Ogre::WindowEventListener, public Ogre::FrameListener, public AbstractRenderer {
public:
	OgreRenderer(Application* app);
	virtual ~OgreRenderer(void);

	std::string getWindowSize();

	bool init();

	bool renderOneFrame();

	virtual void prepareForTrack();

	virtual void showKilledInfo(bool);
	virtual void showTrackCompletedInfo(bool);

protected:
	// Ogre::WindowEventListener
	virtual void windowResized(Ogre::RenderWindow* rw);
	virtual void windowClosed(Ogre::RenderWindow* rw);

	// Ogre::FrameListener
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	//Ogre::MovableObject* getTrackAtomGeometry(TrackAtom*);
	void createScene();

	Ogre::ManualObject* createBox(int x, int y, int z, int size_x, int size_y, int size_z, std::string material);

private:

	void cameraFollowRacer(OGRERendererRacer* racer);
	void buildTrackGeometry();

	bool mSidewardThrustRollCamera;

	Ogre::Light* mLight;

	// OGRE stuff:
	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::SceneNode* mCameraNode;

	std::vector<OGRERendererRacer*> m_vehicleRenderers;

	OGRERendererRacer* mLocalPlayerRacer;


	Ogre::StaticGeometry* mTrackStaticGeometry;

	// Gorilla stuff:
	Gorilla::Silverback* mSilverback;
	Gorilla::Screen* mScreen;
	Gorilla::Layer* mHUDLayer;
	Gorilla::Layer* mHintsLayer;

	Gorilla::Polygon* poly;
	Gorilla::LineList* list;
	Gorilla::Caption* mStopwatchLabel;
	Gorilla::Caption* mEnergyLabel;
	Gorilla::Caption* mPressToPlayLabel;
	Gorilla::Rectangle* rect;
	Gorilla::QuadList* quads;
	Gorilla::MarkupText* markup;
};

#endif /* OGRERENDERER_H_ */
