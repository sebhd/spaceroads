/*
 * BoundingBox.cpp
 *
 *  Created on: 27.04.2012
 *      Author: sebastian
 */

#include "AABB.h"

AABB::AABB() {

	mMin.set(0, 0, 0);
	mMax.set(0, 0, 0);
}

AABB::AABB(cml::vector3f a_min, cml::vector3f a_max) {

	mMin = a_min;
	mMax = a_max;
}

AABB::~AABB() {
	// TODO Auto-generated destructor stub
}


bool AABB::containsPoint(const cml::vector3f& v) {

	if (mMin[0] > v[0] || mMax[0] < v[0]) return false;
	if (mMin[1] > v[1] || mMax[1] < v[1]) return false;
	if (mMin[2] > v[2] || mMax[2] < v[2]) return false;

	return true;
}


bool AABB::intersectsWith(const AABB& other) {

	if (mMax[0] <= other.mMin[0]) return false;
	if (mMin[0] >= other.mMax[0]) return false;
	if (mMax[1] <= other.mMin[1]) return false;
	if (mMin[1] >= other.mMax[1]) return false;
	if (mMax[2] <= other.mMin[2]) return false;
	if (mMin[2] >= other.mMax[2]) return false;

	return true;
}

int AABB::getIntersectingAxis(const AABB& other, bool& x, bool& y, bool& z) {

	x = false;
	y = false;
	z = false;

	int count = 0;

	if (mMax[0] > other.mMin[0] && mMin[0] < other.mMax[0]) {
		x = true;
		count++;
	}

	if (mMax[1] > other.mMin[1] && mMin[1] < other.mMax[1]) {
		y = true;
		count++;
	}

	if (mMax[2] > other.mMin[2] && mMin[2] < other.mMax[2]) {
		z = true;
		count++;
	}

	return count;
}


