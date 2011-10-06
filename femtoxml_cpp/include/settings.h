/* FemtoXML_CPP - A c++ wrapper for FemtoXML
 * FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* settings.h
 *
 * Class declaration for the static Settings-class. It contains
 * global settings for the C++ wrapper */ 

#ifndef SETTINGS_H
#define SETTINGS_H

#include <femtoxml.h>

namespace FemtoXML 
{
	class Settings 
	{
		public:
			// Use exceptions (default and recommended on)
			static bool exceptions;
			// A pointer to custom message callback to redirect error messages from stdout
			static fxml_callback_fn messageCallback;
			// Data to accompany the message callback
			static void* callbackData;
	};
}

#endif
