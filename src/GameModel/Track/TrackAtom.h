/*
 * TrackAtom.h
 *
 *  Created on: 25.04.2012
 *      Author: sebastian
 */

#ifndef TRACKATOM_H_
#define TRACKATOM_H_

#include <string>
#include "../util/AABB.h"
#include "../Racer.h"

class Racer;

class TrackAtom {
public:

	enum HitSide {
		HIT_TOP, HIT_BOTTOM, HIT_LEFT, HIT_RIGHT, HIT_FRONT, HIT_BACK, HIT_NONE
	};

	TrackAtom(AABB bbox);
	virtual ~TrackAtom();

	virtual void applyContactEffects(Racer* ship, HitSide hs);
	virtual void applyCounterForces(Racer* ship, HitSide hs);

	AABB mBBox;

	std::string mName;
	std::string mRenderMaterial;
	std::string mRenderMeshName;

	bool mIsDeadly;
	bool mIsEnergyRefresher;
	bool mIsFinish;
	unsigned int mRotatorSteps;
	unsigned int mRotatorAxis;

	float mBounceThreshold;
	float mRebound;
	float mJumpForce;
	float mSlipOffset;
};

struct CollisionInfo {
	TrackAtom* ta;
	TrackAtom::HitSide hs;
};

#endif /* TRACKATOM_H_ */
