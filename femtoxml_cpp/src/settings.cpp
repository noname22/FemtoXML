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

/* settings.cpp
 *
 * Class definition for the static Settings-class. It contains
 * global settings for the C++ wrapper */ 

#include "settings.h"
#include <iostream> // for NULL

using namespace FemtoXML;

bool Settings::exceptions = true;
fxml_callback_fn Settings::messageCallback = NULL;
void* Settings::callbackData = NULL;
