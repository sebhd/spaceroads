/*
 * XMLFileTrack.h
 *
 *  Created on: 09.06.2012
 *      Author: sebastian
 */

#ifndef XMLFILETRACK_H_
#define XMLFILETRACK_H_

#include "AbstractTrack.h"

class XMLFileTrack: public AbstractTrack {
public:
	XMLFileTrack(Application* a_app);
	virtual ~XMLFileTrack();
};

#endif /* XMLFILETRACK_H_ */
