/* FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* test.c
 * 
 * Test file for femtoxml. Loads a given xml document, parses it, and prints it in two ways:
 * close to internal data representation and as re-generated XML (which hopefully 
 * should be pretty close to what the file looked like in the first place ;).
 */

#include <femtoxml.h>
#include <stdio.h>

int main(int argc, const char* const* argv)
{
	fxml_document* document;
	
	if(argc != 2){
		printf("Usage: %s <xml file>\n", argv[0]);
		return 1;
	}

	document = fxml_load(argv[1]);

	fxml_printSimple(document->elements);
	fxml_print(document->elements);

	fxml_freeDocument(document);

	return 0;
}
