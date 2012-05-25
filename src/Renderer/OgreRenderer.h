/*
 * main.h
 *
 *  Created on: 24.04.2012
 *      Author: sebastian
 */

#ifndef OGRERENDERER_H_
#define OGRERENDERER_H_


// OGRE Includes:
#include <OgreRoot.h>
#include <OgreWindowEventUtilities.h>

#include "AbstractRenderer.h"
#include "../GameModel/TrackAtoms/TrackAtom.h"

class Application;

class OgreRenderer : public Ogre::WindowEventListener, public Ogre::FrameListener, public AbstractRenderer
{
public:
    OgreRenderer(Application* app);
    virtual ~OgreRenderer(void);

    std::string getWindowSize();

    bool init();

    bool renderOneFrame();

protected:
    // Ogre::WindowEventListener
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);

    // Ogre::FrameListener
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    void createTrackAtomGeometry(TrackAtom*);
    void createScene();

    int taCount;

    Ogre::ManualObject* createBox(int x, int y, int z, int size_x, int size_y, int size_z, std::string material);


private:
    // OGRE stuff:
    Ogre::Root* mRoot;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    Ogre::RenderWindow* mWindow;
    Ogre::SceneManager* mSceneMgr;
    Ogre::Camera* mCamera;
    Ogre::SceneNode* mPlayerVehicleNode;
};

#endif /* OGRERENDERER_H_ */
