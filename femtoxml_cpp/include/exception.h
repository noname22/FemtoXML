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

/* exception.h
 *
 * This file contains the function declaration for the exception-function
 * that generates C++-exceptions from femtoxml errors. */

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <femtoxml.h>
#include <exception>
#include <string>

void fxml_exceptionCallback(fxml_msg* msg, void* data);

#endif
