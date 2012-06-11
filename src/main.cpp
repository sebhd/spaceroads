/*
 * main.cpp
 *
 *  Created on: 24.04.2012
 *      Author: sebastian
 */

#include "Application.h"

/*
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
int main(int argc, char *argv[])
#endif
		{
	// Create application object
	OgreApplication app;

	try {
		app.go();
	} catch (Ogre::Exception& e) {
		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
				MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
		#else
				std::cerr << "An exception has occured: " << e.getFullDescription().c_str() << std::endl;
		#endif
	}

	return 0;
}

#ifdef __cplusplus
}
#endif
*/

int main(int argc, char *argv[]) {

	Application app;

	app.init();

	std::string trackFilePath = "tracks/default_track.xml";

	if (argc == 2) {
		trackFilePath = argv[1];
	}

	std::cout << "Now playing track " << trackFilePath << std::endl;

	app.playTrackFile(trackFilePath);

	std::cout << "Bye!" << std::endl;
}
