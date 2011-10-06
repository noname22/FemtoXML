/* FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* create.c
 *
 * Example showing how to create XML files using FemtoXML
 */

#include <femtoxml.h>
#include <stdlib.h> /* For NULL */

int main()
{
	/* The data structures we're going to use */
	fxml_document* document;
	fxml_element* element, *space;

	/* Creates a new, empty xml-document */
	document = fxml_newDocument();

	/* Adds an xml-declaration specifying that we're using xml version 1.0 and
 	   our character encoding is UTF-8 */
	element = fxml_addDeclaration(document->elements, "xml");
	fxml_addAttribute(element, "version", "1.0");
	fxml_addAttribute(element, "encoding", "UTF-8");

	/* Adds a comment to the file */
	fxml_addComment(document->elements, "This is an XML file generated using the FemtoXML XML library");	

	/* Adds an element with the name "space" */
	space = fxml_addElement(document->elements, "space");

	/* Add an attribute to the space element called "frontier" with the value "the final" */
	fxml_addAttribute(space, "frontier", "the final");

	/* Adds a new sub-element to the space-element called "klingons" */
	element = fxml_addElement(space, "klingons");

	/* Adds the text "K'PLAH!" to teh "klingons" element */
	fxml_addText(element, "K'PLAH!");

	/* Adds an element called "ocampa" to the "space"-element */
	element = fxml_addElement(space, "ocampa");
	
	/* Adds an attribute called "strange" with no value to the "ocampa"-element.
 	   Note that NULL is used here to omit the value. */
	fxml_addAttribute(element, "strange", NULL);

	/* Adds an attribute called "max_age" with value "15" to the "ocampa"-element */
	fxml_addAttribute(element, "max_age", "15");

	/* Prints out the XML document on the screen (stdout) */	
	fxml_print(document->elements);

	/* Saves the xml-document to an XML-file */
	fxml_save(document, "test.xml");

	/* Frees the memory used by the data structure */
	fxml_freeDocument(document);

	return 0;
}
