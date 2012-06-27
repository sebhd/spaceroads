/*
 * OgreApplication.cpp
 *
 *  Created on: 24.04.2012
 *      Author: sebastian
 */

// TODO 4: Implement "classic" camera that doesn't translate left/right ...
// with the vehicle but stays in the centerline of the track instead (problems with some concepts?)
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
		buildTrackGeometry();
		mpApp->mpTrack->mHasChanged = false;
	}

	if (mWindow->isClosed()) {
		return false;
	}

	// Update vehicle renderers:

	for (unsigned int ii = 0; ii < m_vehicleRenderers.size(); ii++) {
		m_vehicleRenderers[ii]->update();
	}

	return mpApp->handleFrameRenderingQueuedEvent();
}

Ogre::ManualObject* OgreRenderer::createBox(int x, int y, int z, int size_x, int size_y, int size_z,
		std::string material) {

	Ogre::ManualObject* manual = mSceneMgr->createManualObject();

	//#################### BEGIN Front #####################
	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	manual->normal(0, 0, 1);
	manual->position(x, y + size_y, z); // left  front top 		- vertex1
	manual->position(x, y, z); // left front bottom 	- vertex2
	manual->position(x + size_x, y, z); // right front bottom 	- vertex3
	manual->position(x + size_x, y + size_y, z); // right front top 		- vertex4
	manual->position(x, y, z + size_z); // left back bottom 	- vertex5
	manual->position(x + size_x, y, z + size_z); // right back bottom 	- vertex6
	manual->position(x + size_x, y + size_y, z + size_z); //  right back top 		- vertex7
	manual->position(x, y + size_y, z + size_z); // left  back top 		- vertex8

	manual->index(1);
	manual->index(0);
	manual->index(2);
	manual->index(3);
	manual->index(2);
	manual->index(0);

	manual->end();
	//#################### END Front #####################

	//#################### BEGIN Back #####################
	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	manual->normal(0, 0, -1);
	manual->position(x, y + size_y, z); // left  front top 		- vertex1
	manual->position(x, y, z); // left front bottom 	- vertex2
	manual->position(x + size_x, y, z); // right front bottom 	- vertex3
	manual->position(x + size_x, y + size_y, z); // right front top 		- vertex4
	manual->position(x, y, z + size_z); // left back bottom 	- vertex5
	manual->position(x + size_x, y, z + size_z); // right back bottom 	- vertex6
	manual->position(x + size_x, y + size_y, z + size_z); //  right back top 		- vertex7
	manual->position(x, y + size_y, z + size_z); // left  back top 		- vertex8

	manual->index(7);
	manual->index(4);
	manual->index(6);
	manual->index(5);
	manual->index(6);
	manual->index(4);

	manual->end();
	//#################### END Back #####################

	//#################### BEGIN Top #####################
	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	manual->normal(0, 1, 0);
	manual->position(x, y + size_y, z); // left  front top 		- vertex1
	manual->position(x, y, z); // left front bottom 	- vertex2
	manual->position(x + size_x, y, z); // right front bottom 	- vertex3
	manual->position(x + size_x, y + size_y, z); // right front top 		- vertex4
	manual->position(x, y, z + size_z); // left back bottom 	- vertex5
	manual->position(x + size_x, y, z + size_z); // right back bottom 	- vertex6
	manual->position(x + size_x, y + size_y, z + size_z); //  right back top 		- vertex7
	manual->position(x, y + size_y, z + size_z); // left  back top 		- vertex8

	manual->index(0);
	manual->index(7);
	manual->index(3);
	manual->index(6);
	manual->index(3);
	manual->index(7);

	manual->end();
	//#################### END Top #####################

	//#################### BEGIN Bottom #####################
	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	manual->normal(0, -1, 0);
	manual->position(x, y + size_y, z); // left  front top 		- vertex1
	manual->position(x, y, z); // left front bottom 	- vertex2
	manual->position(x + size_x, y, z); // right front bottom 	- vertex3
	manual->position(x + size_x, y + size_y, z); // right front top 		- vertex4
	manual->position(x, y, z + size_z); // left back bottom 	- vertex5
	manual->position(x + size_x, y, z + size_z); // right back bottom 	- vertex6
	manual->position(x + size_x, y + size_y, z + size_z); //  right back top 		- vertex7
	manual->position(x, y + size_y, z + size_z); // left  back top 		- vertex8

	manual->index(4);
	manual->index(1);
	manual->index(5);
	manual->index(2);
	manual->index(5);
	manual->index(1);

	manual->end();
	//#################### END Bottom #####################

	//#################### BEGIN Right #####################
	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	manual->normal(1, 0, 0);
	manual->position(x, y + size_y, z); // left  front top 		- vertex1
	manual->position(x, y, z); // left front bottom 	- vertex2
	manual->position(x + size_x, y, z); // right front bottom 	- vertex3
	manual->position(x + size_x, y + size_y, z); // right front top 		- vertex4
	manual->position(x, y, z + size_z); // left back bottom 	- vertex5
	manual->position(x + size_x, y, z + size_z); // right back bottom 	- vertex6
	manual->position(x + size_x, y + size_y, z + size_z); //  right back top 		- vertex7
	manual->position(x, y + size_y, z + size_z); // left  back top 		- vertex8

	manual->index(3);
	manual->index(6);
	manual->index(2);
	manual->index(5);
	manual->index(2);
	manual->index(6);

	manual->end();
	//#################### END Right #####################

	//#################### BEGIN Left #####################
	manual->begin(material, Ogre::RenderOperation::OT_TRIANGLE_LIST);
	//manual->normal(0, 0, 1);
	manual->position(x, y + size_y, z); // left  front top 		- vertex1
	manual->position(x, y, z); // left front bottom 	- vertex2
	manual->position(x + size_x, y, z); // right front bottom 	- vertex3
	manual->position(x + size_x, y + size_y, z); // right front top 		- vertex4
	manual->position(x, y, z + size_z); // left back bottom 	- vertex5
	manual->position(x + size_x, y, z + size_z); // right back bottom 	- vertex6
	manual->position(x + size_x, y + size_y, z + size_z); //  right back top 		- vertex7
	manual->position(x, y + size_y, z + size_z); // left  back top 		- vertex8

	manual->index(0);
	manual->index(1);
	manual->index(7);
	manual->index(4);
	manual->index(7);
	manual->index(1);

	manual->end();
	//#################### END Left #####################

	return manual;
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
	time_t timestamp;
	tm *nun;
	timestamp = time(0);
	nun = localtime(&timestamp);

	std::stringstream windowTitle;

	windowTitle << "SpaceRoads - built " << nun->tm_year + 1900 << "-" << nun->tm_mon + 1 << "-" << nun->tm_mday << " "
			<< nun->tm_hour << ":" << nun->tm_min << ":" << nun->tm_sec;

	mWindow = mRoot->initialise(true, windowTitle.str());

	// Set default mipmap level (note: some APIs ignore this)
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// initialize all resource groups
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Create scene manager:
	mSceneMgr = mRoot->createSceneManager("OctreeSceneManager");

	// Set shadow technique:
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);

	// Instantiate vehicle renderers:

	mCameraNode = new Ogre::SceneNode(mSceneMgr);

	for (unsigned int ii = 0; ii < mpApp->m_racers.size(); ii++) {
		//mpVehicleRenderer = new OGRERendererVehicle(mSceneMgr, mpApp->m_racers[ii]);
		m_vehicleRenderers.push_back(new OGRERendererVehicle(mSceneMgr, mpApp->m_racers[ii]));
	}

	//############### BEGIN Set up camera ##############
	mCamera = mSceneMgr->createCamera("PlayerCam");

	mCamera->setNearClipDistance(5);
	mCamera->setFarClipDistance(2000);

	mCameraNode->attachObject(mCamera);

	cameraFollowRacer(m_vehicleRenderers[0]);

	// Create one viewport, entire window
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);
	vp->setBackgroundColour(Ogre::ColourValue(0.95, 0.95, 0.95));
	// Alter the camera aspect ratio to match the viewport
	mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));

	//############### END Set up camera ##############

	//######################## BEGIN Create some entities ############################

	// ####### BEGIN Set up Unit cube mesh/entity that is used to represent track atoms #######
	Ogre::ManualObject* unitCube = createBox(0, 0, 0, 1, 1, 1, "SpaceRoads/Track/White");
	Ogre::MeshPtr meshPtr = unitCube->convertToMesh("UnitCube");
	meshPtr.get()->buildEdgeList();

	mSceneMgr->createEntity("UnitCube", "UnitCube");
	// ####### END Set up Unit cube mesh/entity that is used to represent track atoms #######

	// "Track completed" sign:
	Ogre::Entity* entTrackCompleted = mSceneMgr->createEntity("TrackCompleted", "TrackCompleted.mesh");
	entTrackCompleted->setVisible(false);

	// "Ooops" sign:
	Ogre::Entity* entOoops = mSceneMgr->createEntity("Ooops", "Ooops.mesh");
	entOoops->setVisible(false);

	//######################## END Create some entities ############################

	// ############# BEGIN Set up game state info node ############
	Ogre::SceneNode* gameStateInfoNode = mCameraNode->createChildSceneNode("TrackCompletedNode");
	gameStateInfoNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Radian(M_PI / 2));
	gameStateInfoNode->setPosition(0, 5, 0);
	gameStateInfoNode->scale(0.7, 0.7, 0.7);

	gameStateInfoNode->attachObject(entTrackCompleted);
	gameStateInfoNode->attachObject(entOoops);
	// ############# END Set up game state info node ############

	// Create static geometry object for the track:
	mTrackStaticGeometry = mSceneMgr->createStaticGeometry("TrackAtoms");

	//Set initial mouse clipping size
	windowResized(mWindow);

	//Register as a Window listener
	Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

	mRoot->addFrameListener(this);

	return true;
}

void OgreRenderer::cameraFollowRacer(OGRERendererVehicle* racer) {

	mCamera->setPosition(Ogre::Vector3(0, 10, 40));
	mCamera->lookAt(racer->mVehicleNode->getPosition());

	racer->mVehicleNode->addChild(mCameraNode);

}

void OgreRenderer::prepareForTrack() {

	// ############### BEGIN Set up track / environment rendering ################

	// Set up skybox:
	mSceneMgr->setSkyBox(true, mpApp->mpTrack->mSkybox, 100);

	// Set up ambient light
	mSceneMgr->setAmbientLight(
			Ogre::ColourValue(mpApp->mpTrack->mAmbientLight[0], mpApp->mpTrack->mAmbientLight[1],
					mpApp->mpTrack->mAmbientLight[2]));

	// Set up directional light:
	// TODO 3: Read directional light color from Track class

	Ogre::Light* l = mSceneMgr->createLight("MainLight");
	l->setType(Ogre::Light::LT_DIRECTIONAL);
	l->setCastShadows(true);

	l->setDirection(mpApp->mpTrack->mDirectionalLightDir[0], mpApp->mpTrack->mDirectionalLightDir[1],
			mpApp->mpTrack->mDirectionalLightDir[2]);
	l->setDiffuseColour(0.7, 0.7, 0.7);
	l->setSpecularColour(0.7, 0.7, 0.7);

	std::cout << l->getDirection() << std::endl;

	// Build track geometry:
	buildTrackGeometry();

	// ############### END Set up track / environment rendering ################

}

void OgreRenderer::buildTrackGeometry() {

	Track* track = mpApp->mpTrack;

	mTrackStaticGeometry->destroy();

	mTrackStaticGeometry->setRenderingDistance(1000);

	std::vector<TrackAtom*> trackAtoms = track->getTrackAtomsAround(cml::vector3f(0, 0, 0));

	Ogre::Quaternion orientation;
	orientation.FromAngleAxis(Ogre::Radian(0), Ogre::Vector3::UNIT_Y);

	//############### BEGIN Add track atoms to track geometry #################
	for (unsigned int ii = 0; ii < trackAtoms.size(); ii++) {

		TrackAtom* ta = trackAtoms[ii];

		if (ta->mRenderMaterial != "none") {

			Ogre::Vector3 pos(ta->mBBox.mMin[0], ta->mBBox.mMin[1], ta->mBBox.mMin[2]);
			Ogre::Vector3 scale(ta->mBBox.mMax[0] - ta->mBBox.mMin[0], ta->mBBox.mMax[1] - ta->mBBox.mMin[1],
					ta->mBBox.mMax[2] - ta->mBBox.mMin[2]);

			Ogre::Entity* unitcube = mSceneMgr->getEntity("UnitCube");
			unitcube->setMaterialName(Ogre::String(ta->mRenderMaterial));
			mTrackStaticGeometry->addEntity(unitcube, pos, orientation, scale);
		}
	}
	//############### END Add track atoms to track geometry #################

	//############### BEGIN Add decorative meshes to track geometry #################
	for (unsigned int ii = 0; ii < mpApp->mpTrack->mMeshes.size(); ii++) {

		TrackDecorationMesh* tm = &mpApp->mpTrack->mMeshes[ii];

		if (!mSceneMgr->hasEntity(tm->mMeshName)) {
			mSceneMgr->createEntity(tm->mMeshName, tm->mMeshName + ".mesh");
		}

		Ogre::Entity* entity = mSceneMgr->getEntity(tm->mMeshName);

		entity->setMaterialName(tm->mRenderMaterial);

		mTrackStaticGeometry->addEntity(entity, Ogre::Vector3(tm->mPos[0], tm->mPos[1], tm->mPos[2]), orientation,
				Ogre::Vector3(tm->mScale[0], tm->mScale[1], tm->mScale[2]));
	}
	//############### END Add decorative meshes to track geometry #################

	mTrackStaticGeometry->setCastShadows(true);
	mTrackStaticGeometry->build();

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

void OgreRenderer::showKilledInfo(bool enable) {

	mSceneMgr->getEntity("Ooops")->setVisible(enable);
}

void OgreRenderer::showTrackCompletedInfo(bool enable) {
	mSceneMgr->getEntity("TrackCompleted")->setVisible(enable);
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
