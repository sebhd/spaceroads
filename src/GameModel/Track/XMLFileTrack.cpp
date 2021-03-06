/*
 * XMLFileTrack.cpp
 *
 *  Created on: 09.06.2012
 *      Author: sebastian
 */

#include <tinyxml.h>
#include "XMLFileTrack.h"
#include <iostream>
#include <string>
#include "TrackAtom.h"

XMLFileTrack::XMLFileTrack(std::string filename) {

	TiXmlDocument doc;
	doc.LoadFile(filename);

	TiXmlElement* trackElem = doc.FirstChildElement("Track");

	mSkybox = trackElem->Attribute("skybox");

	//#################### BEGIN Read Start position ######################
	TiXmlElement* startPosElem = trackElem->FirstChildElement("StartPos");

	if (startPosElem != NULL) {
		mStartPosition.set(atof(startPosElem->Attribute("x")), atof(startPosElem->Attribute("y")),
				atof(startPosElem->Attribute("z")));
	}
	//#################### END Read Start position ######################

	//#################### BEGIN Read Ambient light ######################
	TiXmlElement* ambientLightElem = trackElem->FirstChildElement("AmbientLight");

	if (ambientLightElem != NULL) {
		mAmbientLight.set(atof(ambientLightElem->Attribute("r")), atof(ambientLightElem->Attribute("g")),
				atof(ambientLightElem->Attribute("b")));
	}
	//#################### END Read Ambient light ######################

	//#################### BEGIN Read Directional light ######################
	TiXmlElement* directionalLightElem = trackElem->FirstChildElement("DirectionalLight");

	if (directionalLightElem != NULL) {
		mDirectionalLightDir.set(atof(directionalLightElem->Attribute("x")), atof(directionalLightElem->Attribute("y")),
				atof(directionalLightElem->Attribute("z")));
	}
	//#################### END Read Directional light ######################

	//#################### BEGIN Read <Atom> Elements ######################

	for (TiXmlElement* taElem = trackElem->FirstChildElement("Atom"); taElem != NULL;
			taElem = taElem->NextSiblingElement("Atom")) {

		float x = atof(taElem->Attribute("x"));
		float y = atof(taElem->Attribute("y"));
		float z = atof(taElem->Attribute("z"));

		float sizex = atof(taElem->Attribute("scalex"));
		float sizey = atof(taElem->Attribute("scaley"));
		float sizez = atof(taElem->Attribute("scalez"));

		std::string type = "solid";

		if (taElem->Attribute("type") != NULL) {
			type = taElem->Attribute("type");
		}

		AABB bbox(cml::vector3d(x, y, z), cml::vector3d(x + sizex, y + sizey, z + sizez));

		//################### BEGIN Instantiate track atom object depending on type ################
		TrackAtom* ta = new TrackAtom(bbox);

		if (type == "finish") {
			ta->mIsFinish = true;
		}

		if (type == "refresher") {
			ta->mIsEnergyRefresher = true;
		}

		if (type == "rotator") {

			unsigned int axis = atoi(taElem->Attribute("rotatorAxis"));
			unsigned int steps = atof(taElem->Attribute("rotatorSteps"));

			ta->mRotatorSteps = steps;
			ta->mRotatorAxis = axis;

		}

		if (taElem->Attribute("bounceThreshold") != NULL) {
			ta->mBounceThreshold = atof(taElem->Attribute("bounceThreshold"));
		}

		if (taElem->Attribute("slipOffset") != NULL) {
			ta->mSlipOffset = atof(taElem->Attribute("slipOffset"));
		}

		if (taElem->Attribute("jumpForce") != NULL) {
			ta->mJumpForce = atof(taElem->Attribute("jumpForce"));
		}

		if (taElem->Attribute("slipOffset") != NULL) {
			ta->mSlipOffset = atof(taElem->Attribute("slipOffset"));
		}

		if (taElem->Attribute("rebound") != NULL) {
			ta->mRebound = atof(taElem->Attribute("rebound"));
		}

		//################### END Instantiate track atom object depending on type ################

		if (taElem->Attribute("material") != NULL) {
			ta->mRenderMaterial = taElem->Attribute("material");
		}

		if (taElem->Attribute("name") != NULL) {
			ta->mName = taElem->Attribute("name");
		}

		mTrackAtoms.push_back(ta);

	}
	//#################### END Read <Atom> Elements ######################

	//#################### BEGIN Read <Mesh> Elements ######################
	for (TiXmlElement* taElem = trackElem->FirstChildElement("Mesh"); taElem != NULL;
			taElem = taElem->NextSiblingElement("Mesh")) {

		TrackDecorationMesh mesh;

		mesh.mPos.set(atof(taElem->Attribute("x")), atof(taElem->Attribute("y")), atof(taElem->Attribute("z")));

		mesh.mScale.set(atof(taElem->Attribute("scalex")), atof(taElem->Attribute("scaley")),
				atof(taElem->Attribute("scalez")));

		mesh.mMeshName = taElem->Attribute("mesh");

		if (taElem->Attribute("material") != NULL)
			mesh.mRenderMaterial = taElem->Attribute("material");

		mMeshes.push_back(mesh);
	}
	//#################### END Read <Mesh> Elements ######################

	computeExtent();
}

XMLFileTrack::~XMLFileTrack() {
	// TODO Auto-generated destructor stub
}

