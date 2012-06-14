/*
 * BoundingBox.h
 *
 *  Created on: 27.04.2012
 *      Author: sebastian
 */

#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include "cml/cml.h"

class AABB {
public:
	AABB();
	AABB(cml::vector3f, cml::vector3f);
	virtual ~AABB();

	cml::vector3f mMin;
	cml::vector3f mMax;

	bool containsPoint(const cml::vector3f&);
	bool intersectsWith(const AABB& other);
	int getIntersectingAxis(const AABB& other, bool& x, bool& y, bool& z);
};


#endif /* BOUNDINGBOX_H_ */
