/*
 * AbstractRenderer.h
 *
 *  Created on: 29.04.2012
 *      Author: sebastian
 */

#ifndef ABSTRACTRENDERER_H_
#define ABSTRACTRENDERER_H_

#include "Application.h"
#include <string>

class Application;

class AbstractRenderer {
public:
	AbstractRenderer(Application* app);
	virtual ~AbstractRenderer();

	virtual bool renderOneFrame() = 0;
	virtual bool init() = 0;
	virtual std::string getWindowSize() = 0;
	virtual void prepareForTrack() = 0;

protected:
    Application* mpApp;

};


#endif /* ABSTRACTRENDERER_H_ */
