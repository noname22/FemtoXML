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

/* document.h
 *
 * This file contains the class declaration for the wrapper type Document which
 * logically wraps the type fxml_document from the C-library. */

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <femtoxml.h>
#include "element.h"

#include <string>

namespace FemtoXML
{
	class Document
	{
		public:
			// Constructor and destructor
			Document();
			~Document();

			// loading and saving
			bool load(std::string fileName);
			bool loadBuffer(std::string& xml);
			bool loadBuffer(const unsigned char* buffer, long len);
			bool save(std::string fileName);

			// Returns the root element of the document
			Element root();

			// Prints out the entire document as XML to stdout
			void print();
		private:
			// The wrapped data structure
			fxml_document* document;
	};
}

#endif
