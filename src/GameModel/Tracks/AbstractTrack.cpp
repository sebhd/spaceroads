/*
 * AbstractTrack.cpp
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

#include "AbstractTrack.h"

AbstractTrack::AbstractTrack(Application* a_app) {
	// TODO Auto-generated constructor stub

	mpApp = a_app;
}

AbstractTrack::~AbstractTrack() {
	// TODO Auto-generated destructor stub
}

void AbstractTrack::step() {
	// In derived classes, this is the place for code to modify the track at runtime
}

void AbstractTrack::reset() {

}