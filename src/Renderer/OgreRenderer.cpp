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
	mVehicleNode = NULL;
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

	Vehicle* ship = mpApp->mpPlayerVehicle;

	const cml::vector3d& pos = ship->getPosition();

	mVehicleNode->setPosition(pos[0], pos[1], pos[2]);

	/*
	 if (mVehicleOrientation != mDesiredVehicleOrientation) {

	 quat rotDiff = cml::quaternion_rotation_difference(mDesiredOrientation, mOrientation);



	 float diffAngle = 0;
	 cml::vector3f diffVec;
	 cml::quaternion_to_axis_angle(rotDiff, diffVec, diffAngle, (float) 0);

	 if (diffAngle > 0.01) {
	 quat rotQuat;
	 cml::quaternion_rotation_axis_angle(rotQuat, diffVec, -diffAngle * (float) 0.05);
	 setOrientation(mOrientation * rotQuat);
	 } else {
	 setOrientation(mDesiredOrientation);
	 }
	 }
	 */

	//Quaternion delta = Quaternion::Slerp(1, mVehicleOrientation, mDesiredVehicleOrientation, true);

	Ogre::Quaternion q;

	q.FromAngleAxis(Ogre::Radian(10), Ogre::Vector3(1, 0, 0));

	const quat& orientation = ship->getOrientation();

	q.w = orientation.as_vector()[0];
	q.x = orientation.as_vector()[1];
	q.y = orientation.as_vector()[2];
	q.z = orientation.as_vector()[3];

	mVehicleOrientation = q;

	mVehicleNode->setOrientation(mVehicleOrientation);

	return mpApp->handleFrameRenderingQueuedEvent();
}

Ogre::ManualObject* OgreRenderer::createBox(int x, int y, int z, int size_x, int size_y, int size_z,
		std::string material) {

// TODO 4: Memory management for this?
	Ogre::ManualObject* manual = mSceneMgr->createManualObject();

// Top:

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
	if (ta->meshName == "") {

		Ogre::String material = ta->mMaterial;

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

		movable = mSceneMgr->createEntity(id + "_" + ta->meshName, ta->meshName);
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
	mWindow = mRoot->initialise(true, "SpaceRoads");

	// Set default mipmap level (note: some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// initialise all resource groups
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Create the SceneManager, in this case a generic one:
	mSceneMgr = mRoot->createSceneManager("DefaultSceneManager");

	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	mSceneMgr->setSkyBox(true, mpApp->mpTrack->mSkybox);

	//################## BEGIN Add player ship to scene graph ####################

	Ogre::Entity* entVehicle = mSceneMgr->createEntity("Vehicle", "Vehicle.mesh");

	mVehicleNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mVehicleNode->attachObject(entVehicle);


	//################## END Add player ship to scene graph ####################

	mTrackAtomsRootNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

	// Build the track:
	buildTrackSubgraph();

	// Create the camera
	mCamera = mSceneMgr->createCamera("PlayerCam");

	// Position it at 80 in Z direction
	mCamera->setPosition(Ogre::Vector3(0, 13, 50));

	mCamera->setNearClipDistance(5);

	// Look at the player's vehicle:
	mCamera->lookAt(mVehicleNode->getPosition());

	mVehicleNode->attachObject(mCamera);

	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0.95, 0.95, 0.95));

	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	// Set ambient light
	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));

	// Create a light
	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setType(Ogre::Light::LT_POINT);
	l->setCastShadows(true);
	l->setPosition(0, 100, 0);
	mVehicleNode->attachObject(l);
	//l->setDirection(0, -1, -1);

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
