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

/* exception.cpp
 *
 * This file contains the function definition for the exception-function
 * that generates C++-exceptions from femtoxml errors. */

#include "exception.h"
#include "settings.h"
#include <iostream>
#include <stdexcept>

using namespace FemtoXML;

/* Message callback that throws exceptions if an error is encountered. */
void fxml_exceptionCallback(fxml_msg* msg, void* data)
{
	if(msg->type == FXML_MSG_ERROR && Settings::exceptions){
		std::string error = "FemtoXML: ";
		error.append(msg->msg);
		
		throw std::runtime_error(error.c_str());
	}
	if(Settings::messageCallback != NULL){
		(Settings::messageCallback)(msg, data);
	}else{
		std::cout << "FXML " << msg->typeName << "(" << msg->type << "): " << msg->msg << std::endl;
	}
}


