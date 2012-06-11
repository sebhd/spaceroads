/*
 * TrackMesh.h
 *
 *  Created on: 11.06.2012
 *      Author: sebastian
 */

#ifndef TRACKMESH_H_
#define TRACKMESH_H_
#include "cml/cml.h"

class TrackDecorationMesh {
public:
	TrackDecorationMesh();
	virtual ~TrackDecorationMesh();

	cml::vector3f mPos;
	cml::vector3f mScale;
	std::string meshName;

};

#endif /* TRACKMESH_H_ */
