/*
 * RandomTrack.h
 *
 *  Created on: 25.05.2012
 *      Author: sebastian
 */

#ifndef RANDOMTRACK_H_
#define RANDOMTRACK_H_

#include "AbstractTrack.h"

class RandomTrack: public AbstractTrack {
public:
	RandomTrack(Application* a_app);
	virtual ~RandomTrack();

	virtual void step();
	virtual void reset();

protected:

	int nextX, nextY, nextZ;
};

#endif /* RANDOMTRACK_H_ */
