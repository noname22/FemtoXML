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

/* element.cpp
 *
 * This file contains the function definitions for the wrapper type Element, which
 * logically wraps the type fxml_element from the C-library. */

#include "element.h"

#include <string>
#include <sstream>
#include <cstdlib>

using namespace FemtoXML;

// Default constructor: creates an element
Element::Element()
{
	element = fxml_newElement(FXML_TYPE_ELEMENT);
}

// Creates a C++ Element-object from a C-style fxml_element
Element::Element(fxml_element* in)
{
	if(in == NULL){
		element = fxml_errorElement();
		fxml_ePrint(FXML_MSG_ERROR, "Couldn't create element from pointer");
	}else{
		element = in;
	}
}

// Prints out the given element to the message handler
void Element::print()
{
	fxml_printSimple(element);
}

// Count sub-elements by value and/or type
int Element::count(std::string value, int type)
{
	return fxml_count(element, value.c_str(), type);
}


int Element::count(int type)
{
	return count("", type);
}

// Get sub-elements by type and/or value and/or index
Element Element::get(int type, std::string value, int index)
{
	return Element(fxml_get(element, value.c_str(), type, index));
}

Element Element::get(int index)
{
	return get(FXML_TYPE_ANY, "", index);
}

Element Element::get(std::string value, int index)
{
	return get(FXML_TYPE_ANY, value, index);
}

Element Element::get(int type, int index)
{
	return get(type, "", index);
}

// Returns the value of this element
std::string Element::value()
{
	if(element->val){
		return std::string(element->val);
	}else{
		fxml_ePrint(FXML_MSG_ERROR, "The element value is NULL in  Element::value()");
		return "";
	}
}

std::string Element::asStr()
{
	return value();
}

double Element::asFloat()
{
	std::stringstream tmp(value());
	double ret;
	tmp >> ret;
	return ret;
}

int Element::asInt()
{
	std::stringstream tmp(value());
	int ret;
	tmp >> ret;
	return ret;
}


// Add/create sub-elements
Element Element::addElement(std::string name)
{
	fxml_element* newElement = fxml_addElement(element, name.c_str());
	return Element(newElement);
}

Element Element::addText(std::string text)
{
	fxml_element* newElement = fxml_addText(element, text.c_str());
	return Element(newElement);
}

Element Element::addDeclaration(std::string name)
{
	fxml_element* newElement = fxml_addDeclaration(element, name.c_str());
	return Element(newElement);
}

Element Element::addComment(std::string comment)
{
	fxml_element* newElement = fxml_addComment(element, comment.c_str());
	return Element(newElement);
}

Element Element::addDoctype(std::string text)
{
	fxml_element* newElement = fxml_addDoctype(element, text.c_str());
	return Element(newElement);
}

Element Element::addAttribute(std::string attribute)
{
	fxml_element* newElement = fxml_addAttribute(element, attribute.c_str(), NULL);
	return Element(newElement);
}

Element Element::addAttribute(std::string attribute, std::string value)
{
	fxml_element* newElement = fxml_addAttribute(element, attribute.c_str(), value.c_str());
	return Element(newElement);
}

void Element::set(std::string value)
{
	fxml_setVal(element, value.c_str());
}

void Element::setType(int type)
{
	element->type = type;
}

Element Element::valueOf(std::string eValue, int index, int type)
{
	return get(type, eValue, index).get();
}

Element::operator int()
{
	return asInt();
}

Element::operator float()
{
	return asFloat();
}

Element::operator double()
{
	return asFloat();
}

Element::operator std::string()
{
	return value();
}

