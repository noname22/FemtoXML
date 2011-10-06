/* FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* music.c
 *
 * Music collection example for the FemtoXML XML parser.
 * To be used with the provided 'music.xml' (or any other file with the same structure).
 */

#include <stdio.h>
#include <stdlib.h>
#include <femtoxml.h>

/* Example message handler */
void message(fxml_msg* msg, void* data)
{
	/* Check what kind of message we got */
	if(msg->type == FXML_MSG_ERROR){
		/* FemtoXML ran into some kind of trouble. Print what it was and die. */
		fprintf(stderr, "There was an error while parsing the document: %s\n", msg->msg);
		exit(1);
	}else{
		printf("Message from FemtoXML: %s (%d): %s\n", msg->typeName, msg->type, msg->msg);
	}
}

int main(int argc, const char* const* argv)
{
	fxml_document* document;
	fxml_element *collection, *album, *track, *tmp;
	int i, i2, i3;
	
	if(argc != 2){
		printf("Usage: %s <music database file (eg. music.xml)>\n", argv[0]);
		return 1;
	}

	/* Set up a local callback for error and message handling */
	fxml_setCallback(message, NULL);

	/* Open our document */
	document = fxml_load(argv[1]);

	/* If the document is NULL, loading failed */
	if(!document){
		return 1;
	}

	/* This is the code for reading an XML-file with the structure of the provided 'music.xml'. */

	/* If the file doesn't contain any 'collection' elements, die */
	if(fxml_count(document->elements, "collection", FXML_TYPE_ELEMENT) <= 0){
		printf("The specified file apperas to be valid XML (or plain text), but contains no 'collection'-elements.\n");
		return 1;
	}

	/* Loop through all the collections in the file */
	for(i = 0; i < fxml_count(document->elements, "collection", FXML_TYPE_ELEMENT); i++){
		collection = fxml_get(document->elements, "collection", FXML_TYPE_ELEMENT, i);

		/* Checks if the collection has a description and print it if it indeed does */
		if(fxml_count(collection, "description", FXML_TYPE_ATTRIBUTE) > 0){
			tmp = fxml_get(collection, "description", FXML_TYPE_ATTRIBUTE, 0);
			tmp = fxml_get(tmp, "", FXML_TYPE_ATTRIBUTE_VAL, 0);
			printf("Collection with name: '%s'\n\n", tmp->val);
		}else{
			printf("Collection with no name\n\n");
		}
		
		/* Loop through all the albums in the collection */
		for(i2 = 0; i2 < fxml_count(collection, "album", FXML_TYPE_ELEMENT); i2++){
			album = fxml_get(collection, "album", FXML_TYPE_ELEMENT, i2);
			tmp = fxml_get(album, "title", FXML_TYPE_ELEMENT, 0);
			tmp = fxml_get(tmp, "", FXML_TYPE_TEXT, 0);
			
			printf("Title:\t%s\n", tmp->val);
			
			tmp = fxml_get(album, "artist", FXML_TYPE_ELEMENT, 0);
			tmp = fxml_get(tmp, "", FXML_TYPE_TEXT, 0);

			printf("Artist:\t%s\n", tmp->val);
			
			tmp = fxml_get(album, "year", FXML_TYPE_ELEMENT, 0);
			tmp = fxml_get(tmp, "", FXML_TYPE_TEXT, 0);

			printf("Year:\t%s\n", tmp->val);

			printf("Track listing:\n");
		
			/* Loop through all the tracks in the album */
			for(i3 = 0; i3 < fxml_count(album, "track", FXML_TYPE_ELEMENT); i3++){
				track = fxml_get(album, "track", FXML_TYPE_ELEMENT, i3);
				tmp = fxml_get(track, "number", FXML_TYPE_ATTRIBUTE, 0);
				tmp = fxml_get(tmp, "", FXML_TYPE_ATTRIBUTE_VAL, 0);
				printf("\t%s", tmp->val);

				tmp = fxml_get(track, "", FXML_TYPE_TEXT, 0);
				printf(": %s ", tmp->val);
				
				tmp = fxml_get(track, "length", FXML_TYPE_ATTRIBUTE, 0);
				tmp = fxml_get(tmp, "", FXML_TYPE_ATTRIBUTE_VAL, 0);
				printf("(%s)\n", tmp->val);
			}

			printf("\n");
		}
	}

	fxml_freeDocument(document);

	return 0;
}
