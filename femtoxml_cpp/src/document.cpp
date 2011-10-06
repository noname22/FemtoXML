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

/* document.cpp
 *
 * This file contains the function definitions for the wrapper type Document, which
 * logically wraps the type fxml_document from the C-library. */

#include "document.h"
#include "settings.h"
#include "exception.h"

using namespace FemtoXML;
			
// Constructor
// Sets up the exception callback and creates a new document
Document::Document()
{
	fxml_setCallback(fxml_exceptionCallback, Settings::callbackData);
	document = fxml_newDocument();
}

// Destructor
// Frees the C-structure
Document::~Document()
{
	if(document != NULL){
		fxml_freeDocument(document);
	}
}


// Parses a buffer with XML data
bool Document::loadBuffer(const unsigned char* buffer, long len)
{
	fxml_document* loadDoc;
	loadDoc = fxml_loadBuffer(buffer, len);
	
	if(loadDoc != NULL){
		fxml_freeDocument(document);
		document = loadDoc;
		return true;
	}
	
	return false;
}

// Parses a string with XML data
bool Document::loadBuffer(std::string& xml)
{
	return loadBuffer((const unsigned char*)xml.data(), xml.size());
}

// Loads a file
bool Document::load(std::string fileName)
{
	fxml_document* loadDoc;
	loadDoc = fxml_load(fileName.c_str());
	
	if(loadDoc != NULL){
		fxml_freeDocument(document);
		document = loadDoc;
		return true;
	}
	
	return false;
}

// Saves the data to a file
bool Document::save(std::string fileName)
{
	return fxml_save(document, fileName.c_str());
}

// Prints the document as xml to stdout
void Document::print()
{
	fxml_print(document->elements);
}

// Returns the root element of the document
Element Document::root()
{
	return Element(document->elements);
}
