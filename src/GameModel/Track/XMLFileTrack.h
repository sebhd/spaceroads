/*
 * XMLFileTrack.h
 *
 *  Created on: 09.06.2012
 *      Author: sebastian
 */

#ifndef XMLFILETRACK_H_
#define XMLFILETRACK_H_

#include "Track.h"
#include <string>

class XMLFileTrack: public Track {
public:
	XMLFileTrack(std::string filename);
	virtual ~XMLFileTrack();
};

#endif /* XMLFILETRACK_H_ */
