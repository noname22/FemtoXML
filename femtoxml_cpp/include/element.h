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

/* element.h
 *
 * This file contains the class declaration for the wrapper type Element which
 * logically wraps the type fxml_element from the C-library. */

#ifndef ELEMENT_H
#define ELEMENT_H

#include "femtoxml.h"

#include <string>
#include <iostream>

namespace FemtoXML
{
	class Element
	{
		public:
			// Constructors
			Element();
			Element(fxml_element* in);

			// Prints the element (and it's sub-elements) to the message handler
			void print();

			// Count sub-elements by value and/or type
			int count(int type);
			int count(std::string value, int type = FXML_TYPE_ANY);
			
			// Get sub-element by index
			Element get(int index = 0);

			// Get sub-element by value
			Element get(std::string value, int index = 0);

			// Get sub-element by type
			Element get(int type, int index);
			Element get(int type, std::string value, int index = 0);

			// Modify element
			void set(std::string value);
			void setType(int type);			

			// Add/create sub-elements
			Element addElement(std::string name);
			Element addText(std::string text);
			Element addDeclaration(std::string name);
			Element addComment(std::string comment);
			Element addDoctype(std::string text);
			Element addAttribute(std::string attribute);
			Element addAttribute(std::string attribute, std::string value);

			// Returns the value of this element
			std::string value();
			std::string asStr();
			double asFloat();
			int asInt();

			// Returns the first child of a given element (like the value of an attribute)
			Element valueOf(std::string eValue, int index = 0, int type = FXML_TYPE_ANY);

			// Conversions

			operator int();
			operator float();
			operator double();
			operator std::string();

		private:
			fxml_element* element;
	};
}

#endif
