/*
 * OgreApplication.cpp
 *
 *  Created on: 24.04.2012
 *      Author: sebastian
 */

#include "OgreRenderer.h"

#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreCamera.h>
#include <OgreManualObject.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>

// Constructor:
OgreRenderer::OgreRenderer(Application* app) :
		AbstractRenderer(app), mRoot(0), mResourcesCfg(Ogre::StringUtil::BLANK), mPluginsCfg(Ogre::StringUtil::BLANK) {

	mSidewardThrustRollCamera = false;

	mpVehicle = NULL;
}

// Destructor:
OgreRenderer::~OgreRenderer(void) {

	//Remove ourself as a Window listener
	Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
	windowClosed(mWindow);
	delete mRoot;
}

// Handler for frameRenderingQueued event:
bool OgreRenderer::frameRenderingQueued(const Ogre::FrameEvent& evt) {

	if (mpApp->mpTrack->mHasChanged) {
		buildTrackSubgraph();
		mpApp->mpTrack->mHasChanged = false;
	}

	if (mWindow->isClosed()) {
		return false;
	}

	if (mpVehicle != NULL) {
		mpVehicle->update();
	}

	return mpApp->handleFrameRenderingQueuedEvent();
}

Ogre::ManualObject* OgreRenderer::createBox(int x, int y, int z, int size_x, int size_y, int size_z,
		std::string material) {

// TODO 4: Memory management for this?
	Ogre::ManualObject* manual = mSceneMgr->createManualObject();

// Top:

// TODO 2: Generate cube meshes that support shadows

	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);

	manual->position(x, y + size_y, z + size_z); // left  back top
	manual->normal(0, 1, 0);
	manual->position(x + size_x, y + size_y, z); // right front top
	manual->normal(0, 1, 0);
	manual->position(x, y + size_y, z); // left  front top
	manual->normal(0, 1, 0);

	manual->position(x, y + size_y, z + size_z); // left  back top
	manual->normal(0, 1, 0);
	manual->position(x + size_x, y + size_y, z + size_z); //  right back top
	manual->normal(0, 1, 0);
	manual->position(x + size_x, y + size_y, z); // right front top
	manual->normal(0, 1, 0);

	manual->end();

	// Front:
	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	manual->position(x, y + size_y, z); //  right back  bottom
	manual->normal(0, 0, 1);
	manual->position(x + size_x, y + size_y, z); //  right back  bottom
	manual->normal(0, 0, 1);
	manual->position(x, y, z); // right front bottom
	manual->normal(0, 0, 1);

	manual->position(x + size_x, y + size_y, z); //  right back  bottom
	manual->normal(0, 0, 1);
	manual->position(x + size_x, y, z); //  right back  bottom
	manual->normal(0, 0, 1);
	manual->position(x, y, z); // right front bottom
	manual->normal(0, 0, 1);

	manual->end();

	// Left:
	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	manual->position(x, y + size_y, z + size_z); // left back top
	manual->normal(-1, 0, 0);
	manual->position(x, y + size_y, z); // left front top
	manual->normal(-1, 0, 0);
	manual->position(x, y, z + size_z); // left  back  bottom
	manual->normal(-1, 0, 0);

	manual->position(x, y + size_y, z); // left front top
	manual->normal(-1, 0, 0);
	manual->position(x, y, z); // left front bottom
	manual->normal(-1, 0, 0);
	manual->position(x, y, z + size_z); // left  back  bottom
	manual->normal(-1, 0, 0);

	manual->end();

	// Right:
	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);

	manual->position(x + size_x, y + size_y, z); // right front top
	manual->normal(1, 0, 0);
	manual->position(x + size_x, y + size_y, z + size_z); //  right back top
	manual->normal(1, 0, 0);
	manual->position(x + size_x, y, z); // right front bottom
	manual->normal(1, 0, 0);

	manual->position(x + size_x, y + size_y, z + size_z); //  right back top
	manual->normal(1, 0, 0);
	manual->position(x + size_x, y, z + size_z); // right back bottom
	manual->normal(1, 0, 0);
	manual->position(x + size_x, y, z); // right front bottom
	manual->normal(1, 0, 0);

	manual->end();

	// Back:
	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	manual->position(x + size_x, y + size_y, z + size_z); //  right back top
	manual->normal(0, 0, 1);
	manual->position(x, y + size_y, z + size_z); // left back top
	manual->normal(0, 0, 1);
	manual->position(x + size_x, y, z + size_z); // right back bottom
	manual->normal(0, 0, 1);

	manual->position(x, y + size_y, z + size_z); // left back top
	manual->normal(0, 0, 1);
	manual->position(x, y, z + size_z); // left back bottom
	manual->normal(0, 0, 1);
	manual->position(x + size_x, y, z + size_z); // right back bottom
	manual->normal(0, 0, 1);
	manual->end();

	// Bottom:
	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	manual->position(x + size_x, y, z); // right front bottom
	manual->normal(0, -1, 0);
	manual->position(x + size_x, y, z + size_z); // right back bottom
	manual->normal(0, -1, 0);
	manual->position(x, y, z); // left front bottom
	manual->normal(0, -1, 0);

	manual->position(x + size_x, y, z + size_z); // right back bottom
	manual->normal(0, -1, 0);
	manual->position(x, y, z + size_z); // left  back bottom
	manual->normal(0, -1, 0);
	manual->position(x, y, z); // left front bottom
	manual->normal(0, -1, 0);

	manual->end();

	/*
	 // Neue box mit indices:
	 // Front:
	 manual->begin("BaseWhite", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	 manual->position(x, y, z + size_z);
	 manual->position(x, y + size_y, z + size_z);
	 manual->position(x + size_x, y + size_y, z + size_z);
	 manual->position(x + size_x, y, z + size_z);

	 manual->index(0);
	 manual->index(2);
	 manual->index(1);
	 manual->index(0);
	 manual->index(3);
	 manual->index(2);
	 manual->end();

	 manual->position(x, y, z);
	 manual->position(x, y + size_y, z);
	 manual->position(x + size_x, y + size_y, z);
	 manual->position(x + size_x, y, z);

	 // Top:

	 manual->index(1);
	 manual->index(6);
	 manual->index(5);
	 manual->index(1);
	 manual->index(2);
	 manual->index(6);

	 // Right:
	 manual->index(3);
	 manual->index(7);
	 manual->index(6);
	 manual->index(3);
	 manual->index(6);
	 manual->index(2);
	 */
	return manual;
}

Ogre::MovableObject* OgreRenderer::getTrackAtomGeometry(TrackAtom* ta) {

	Ogre::MovableObject* movable;

	// If the TrackAtom has no mesh name assigned, create default geometry for it.
	// This will be a simple block with the same size and shape as the TrackAtom's collision bounding box:
	if (ta->mRenderMeshName == "") {

		Ogre::String material = ta->mRenderMaterial;

		Ogre::ManualObject* manual = createBox(0, 0, 0, ta->mBBox.mSize[0], ta->mBBox.mSize[1], ta->mBBox.mSize[2],
				material);

		manual->setMaterialName(0, material);

		movable = (Ogre::MovableObject*) manual;

	}
	// However, if a mesh is defined, we try to load this mesh from file:
	else {

		std::ostringstream s;
		std::string id;
		s << taCount;
		id = s.str();
		taCount++;

		movable = mSceneMgr->createEntity(id + "_" + ta->mRenderMeshName, ta->mRenderMeshName);
	}

	return movable;
}

bool OgreRenderer::init() {
#ifdef _DEBUG
	mResourcesCfg = "resources_d.cfg";
	mPluginsCfg = "plugins_d.cfg";

#else
	mResourcesCfg = "resources.cfg";
	mPluginsCfg = "plugins.cfg";
#endif

	// construct Ogre::Root
	mRoot = new Ogre::Root(mPluginsCfg);

	// set up resources
	// Load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(mResourcesCfg);

	// Go through all sections & settings in the file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String secName, typeName, archName;
	while (seci.hasMoreElements()) {
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i) {
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	// configure

	// Show the configuration dialog and initialise the system
	if (!(mRoot->restoreConfig() || mRoot->showConfigDialog())) {
		return false;
	}

	// Create render window:
	mWindow = mRoot->initialise(true, "SpaceRoads 2012-06-08");

	// Set default mipmap level (note: some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// initialise all resource groups
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Create the SceneManager, in this case a generic one:
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	// ############### BEGIN Set up track / environment rendering ################
	mTrackAtomsRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	// Build the track:
	buildTrackSubgraph();

	// Set up skybox:
	mSceneMgr->setSkyBox(true, mpApp->mpTrack->mSkybox);

	// Set up ambient light
	// TODO 2: Define ambient light in track
	//mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));

	// Set up directional light:
	// TODO 2: Read light settings from Track class

	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	l->setCastShadows(true);
	//l->setDirection(1, -1, 1);
	l->setDirection(0, -1, 0);
	l->setDiffuseColour(1, 1, 1);
	l->setSpecularColour(1, 1, 1);

	//mSceneMgr->getRootSceneNode()->attachObject(l);

	// ############### END Set up track / environment rendering ################

	mpVehicle = new OGREVehicle(mSceneMgr, mpApp->mpPlayerVehicle);

	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");
	mCamera->setNearClipDistance(5);

	// Position the camera behind the player's vehicle:
	mpVehicle->mVehicleNode->attachObject(mCamera);
	mCamera->setPosition(Ogre::Vector3(0, 15, 40));
	mCamera->lookAt(mpVehicle->mVehicleNode->getPosition());
	//mCamera->lookAt(mpVehicle->mVehicleNode);
	//mCamera->setAutoTracking(true, mpVehicle->mVehicleNode, Ogre::Vector3(0,0,0));

	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0.95, 0.95, 0.95));
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);

	return true;
}

void OgreRenderer::buildTrackSubgraph() {

	std::vector<TrackAtom*> trackAtoms = mpApp->mpTrack->getTrackAtomsAround(cml::vector3f(0, 0, 0));

	taCount = 0;

	mTrackAtomsRootNode->removeAllChildren();

	for (unsigned int ii = 0; ii < trackAtoms.size(); ii++) {

		TrackAtom* ta = trackAtoms[ii];

		Ogre::MovableObject* movable = getTrackAtomGeometry(ta);

		//Ogre::SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		Ogre::SceneNode* node = mTrackAtomsRootNode->createChildSceneNode();

		node->setPosition(ta->mBBox.mPos[0], ta->mBBox.mPos[1], ta->mBBox.mPos[2]);
		node->attachObject(movable);
	}
}

bool OgreRenderer::renderOneFrame() {
	Ogre::WindowEventUtilities::messagePump();

	// Render a frame
	if (!mRoot->renderOneFrame()) {
		return false;
	}

	return true;
}

std::string OgreRenderer::getWindowSize() {
	size_t windowHnd = 0;
	std::ostringstream windowHndStr;

	mWindow->getCustomAttribute("WINDOW", &windowHnd);
	windowHndStr << windowHnd;

	return windowHndStr.str();
}

//Adjust mouse clipping area
void OgreRenderer::windowResized(Ogre::RenderWindow* rw) {

	// TODO 4: Reimplement!
	/*
	 unsigned int width, height, depth;
	 int left, top;
	 rw->getMetrics(width, height, depth, left, top);

	 const OIS::MouseState &ms = mMouse->getMouseState();
	 ms.width = width;
	 ms.height = height;
	 */
}

//Unattach OIS before window shutdown (very important under Linux)
void OgreRenderer::windowClosed(Ogre::RenderWindow* rw) {

}
