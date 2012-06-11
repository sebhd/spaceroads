/*
 * XMLFileTrack.cpp
 *
 *  Created on: 09.06.2012
 *      Author: sebastian
 */

#include "XMLFileTrack.h"
#include "tinyxml.h"
#include <string>
#include "../TrackAtoms/SolidTrackAtom.h"

XMLFileTrack::XMLFileTrack(Application* a_app) :
		AbstractTrack(a_app) {
	// TODO Auto-generated constructor stub

	TiXmlDocument doc;
	doc.LoadFile("tracks/default_track.xml");

	TiXmlElement* trackElem = doc.FirstChildElement("Track");

	mSkybox = trackElem->Attribute("skybox");

	//#################### BEGIN Read <Atom> Elements ######################
	for (TiXmlElement* taElem = trackElem->FirstChildElement("Atom"); taElem != NULL;
			taElem = taElem->NextSiblingElement("Atom")) {

		float x = atof(taElem->Attribute("x"));
		float y = atof(taElem->Attribute("y"));
		float z = atof(taElem->Attribute("z"));

		float sizex = atof(taElem->Attribute("sizex"));
		float sizey = atof(taElem->Attribute("sizey"));
		float sizez = atof(taElem->Attribute("sizez"));

		SolidTrackAtom* ta = new SolidTrackAtom(
				game::BoundingBox(cml::vector3d(x, y, z), cml::vector3d(sizex, sizey, sizez)));

		if (taElem->Attribute("jumpForce") != NULL) {
			ta->mJumpForce = atof(taElem->Attribute("jumpForce"));
		}

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

		TrackMesh mesh;

		mesh.mPos.set(atof(taElem->Attribute("x")), atof(taElem->Attribute("y")),
				atof(taElem->Attribute("z")));
		mesh.mScale.set(atof(taElem->Attribute("scalex")), atof(taElem->Attribute("scaley")),
				atof(taElem->Attribute("scalez")));
		mesh.meshName = taElem->Attribute("mesh");

		mMeshes.push_back(mesh);

	}
	//#################### END Read <Mesh> Elements ######################
}

XMLFileTrack::~XMLFileTrack() {
	// TODO Auto-generated destructor stub
}

