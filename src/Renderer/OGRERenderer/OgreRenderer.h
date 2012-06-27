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
#include "../../GameModel/Track/TrackAtom/TrackAtom.h"
#include "OGRERendererVehicle.h"


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

	void cameraFollowRacer(OGRERendererVehicle* racer);
	void buildTrackGeometry();

	bool mSidewardThrustRollCamera;

	// OGRE stuff:
	Ogre::Root* mRoot;
	Ogre::String mResourcesCfg;
	Ogre::String mPluginsCfg;
	Ogre::RenderWindow* mWindow;
	Ogre::SceneManager* mSceneMgr;
	Ogre::Camera* mCamera;
	Ogre::SceneNode* mCameraNode;

	std::vector<OGRERendererVehicle*> m_vehicleRenderers;

	Ogre::StaticGeometry* mTrackStaticGeometry;
};

#endif /* OGRERENDERER_H_ */
