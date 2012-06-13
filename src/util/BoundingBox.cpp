/*
 * BoundingBox.cpp
 *
 *  Created on: 27.04.2012
 *      Author: sebastian
 */

#include "BoundingBox.h"



BoundingBox::BoundingBox() {
	// TODO Auto-generated constructor stub
	mPos.set(0, 0, 0);
	mSize.set(0, 0, 0);
}

BoundingBox::BoundingBox(cml::vector3f pos, cml::vector3f size) {
	// TODO Auto-generated constructor stub
	mPos = pos;
	mSize = size;
}

BoundingBox::~BoundingBox() {
	// TODO Auto-generated destructor stub
}


bool BoundingBox::containsPoint(const cml::vector3f& v) {

	if (mPos[0] > v[0] || mPos[0] + mSize[0] < v[0]) return false;
	if (mPos[1] > v[1] || mPos[1] + mSize[1] < v[1]) return false;
	if (mPos[2] > v[2] || mPos[2] + mSize[2] < v[2]) return false;

	return true;
}


bool BoundingBox::intersectsWith(const BoundingBox& other) {

	cml::vector3d own_max = mPos + mSize;
	cml::vector3d other_max = other.mPos + other.mSize;

	if (own_max[0] < other.mPos[0]) return false;
	if (mPos[0] > other_max[0]) return false;
	if (own_max[1] < other.mPos[1]) return false;
	if (mPos[1] > other_max[1]) return false;
	if (own_max[2] < other.mPos[2]) return false;
	if (mPos[2] > other_max[2]) return false;

	return true;
}

int BoundingBox::getIntersectingAxis(const BoundingBox& other, bool& x, bool& y, bool& z) {

	cml::vector3d own_max = mPos + mSize;
	cml::vector3d other_max = other.mPos + other.mSize;

	x = false;
	y = false;
	z = false;

	int count = 0;

	if (own_max[0] > other.mPos[0] && mPos[0] < other_max[0]) {
		x = true;
		count++;
	}

	if (own_max[1] > other.mPos[1] && mPos[1] < other_max[1]) {
		y = true;
		count++;
	}

	if (own_max[2] > other.mPos[2] && mPos[2] < other_max[2]) {
		z = true;
		count++;
	}

	return count;
}


