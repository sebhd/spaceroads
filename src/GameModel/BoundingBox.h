/*
 * BoundingBox.h
 *
 *  Created on: 27.04.2012
 *      Author: sebastian
 */

#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include "cml/cml.h"

namespace game {

class BoundingBox {
public:
	BoundingBox();
	BoundingBox(cml::vector3f, cml::vector3f);
	virtual ~BoundingBox();

	cml::vector3f mPos;
	cml::vector3f mSize;

	bool containsPoint(cml::vector3f&);
	bool intersectsWith(BoundingBox* other);
	int getIntersectingAxis(const BoundingBox& other, bool& x, bool& y, bool& z);
};

} /* namespace game */
#endif /* BOUNDINGBOX_H_ */
