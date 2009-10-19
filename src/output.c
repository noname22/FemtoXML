/* FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* output.c
 *
 * This file contains code for outputting the information in the femtoxml data structure
 * to a file or to the screen. */

#include "femtoxmli.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* #define DEBUG_OUTPUT */

/* returns the length of a string, but max FXML_MAXSTRLEN */
int fxml_strlen(const char* str)
{
	int len = strlen(str);
	
	if(len > FXML_MAXSTRLEN){
		len = FXML_MAXSTRLEN;
	}
	return len;
}

/* Checks if a buffer contains 'needle' */
int fxml_contains(char* buffer, char needle, int len)
{
	int i;
	for(i=0; i < len; i++){
		if(buffer[i] == needle){
			return TRUE;
		}
	}
	return FALSE;
}



/* Returns a text representation of the given element type */
const char* fxml_getTypeName(unsigned int type)
{
	static char typeNames[11][20] = {
		"Rootnode", 
		"Text", 
		"Element", 
		"Attribute", 
		"AttributeValue", 
		"Comment", 
		"Declaration", 
		"Doctype", 
		"\"Any\"", 
		"\"None\"",
		"Unknown"
	};

	if(type >= 0 && type < 10){
		return typeNames[type % 9];
	}else{
		return typeNames[10];
	}
}

/* Generates a string with the given number of spaces */
char* fxml_genTabs(int num)
{
	int i;
	char* tabs;
	tabs = (char*)malloc(num + 1);

	for(i = 0; i < num; i++){
		tabs[i] = ' ';
	}

	tabs[num] = '\0';
	return tabs;
}

/* Prints out a simple representation of an element structure */
void fxml_printSimpleRecurse(fxml_element* element, int depth)
{
	if(depth > FXML_MAXDEPTH){
		return;
	}
	do{
		if(element->val != NULL){
			char* tabs = fxml_genTabs(depth * 2);
			#ifdef DEBUG_OUTPUT
			fxml_ePrint(FXML_MSG_INFO, "%s'%s' type: '%s' (%d) depth: %d this: %p next: %p", tabs, element->val, fxml_getTypeName(element->type) ,element->type, depth, (void*)element, (void*)element->next);
			#else
			fxml_ePrint(FXML_MSG_INFO, "%s'%s' type: '%s' (%d)", tabs, element->val, fxml_getTypeName(element->type), element->type);
			#endif
			free(tabs);
		}

		if(element->children != NULL){
			fxml_printSimpleRecurse(element->children, depth + 1);
		}
	}while(fxml_getNextElement(&element));
}


void fxml_printSimple(fxml_element* element)
{
	fxml_printSimpleRecurse(element, 0);
}

/* Prints out a document structure as XML to the given buffer */
/* TODO: rewrite with fxml_getNextElement(). Recurse in two directions is unnecessary */
void fxml_printRecurse(FILE* buffer, fxml_element* element, int printAttrib, int depth, int breadth)
{
	int i;

	if(element == fxml_errorElement()){
		fxml_ePrint(FXML_MSG_ERROR, "The structure contains an \"error element\".");
		return;
	}

	if(breadth > FXML_MAXBREADTH){
		fxml_ePrint(FXML_MSG_ERROR, "Error during recurse, at max breadth in elements (%d)!", FXML_MAXBREADTH);
		return;
	}

	if(depth > FXML_MAXDEPTH){
		fxml_ePrint(FXML_MSG_ERROR, "Error during recurse, at max depth in elements (%d)!", FXML_MAXDEPTH);
		return;
	}

	if(printAttrib){
		if(element->type == FXML_TYPE_ATTRIBUTE){
			fprintf(buffer, " %s", element->val);
		}
		if(element->children != NULL && element->children->type == FXML_TYPE_ATTRIBUTE_VAL){
			if(fxml_contains(element->children->val, '"', fxml_strlen(element->children->val))){
				fprintf(buffer, "=\'%s\'", element->children->val);
			}else{
				fprintf(buffer, "=\"%s\"", element->children->val);
			}
		}
	}else if(element->type != FXML_TYPE_ATTRIBUTE){
		for(i = 0; i < depth - 1; i++){
			fprintf(buffer, "\t");
		}

		switch(element->type){
			case FXML_TYPE_ROOT:
				if(element->children != NULL){
					fxml_printRecurse(buffer, element->children, FALSE, depth + 1, breadth);
				}
				break;

			case FXML_TYPE_TEXT:
				fprintf(buffer, "%s\n", element->val);
				break;

			case FXML_TYPE_COMMENT:
				fprintf(buffer, "<!--%s-->\n", element->val);
				break;
			
			case FXML_TYPE_DOCTYPE:
				fprintf(buffer, "<!DOCTYPE %s>\n", element->val);
				break;

			case FXML_TYPE_ELEMENT:
			case FXML_TYPE_DECLARATION:
				if(element->type == FXML_TYPE_DECLARATION){
					fprintf(buffer, "<?%s", element->val);
				}else{
					fprintf(buffer, "<%s", element->val);
				}
				if(element->children != NULL){

					fxml_printRecurse(buffer, element->children, TRUE, depth + 1, breadth);

					if(fxml_countElements(element->children, FXML_TYPE_ATTRIBUTE, TRUE) > 0){
						if(element->type == FXML_TYPE_DECLARATION){
							fprintf(buffer, "?>\n");
						}else{
							fprintf(buffer, ">\n");
						}

						fxml_printRecurse(buffer, element->children, FALSE, depth + 1, breadth);

						for(i = 0; i < depth - 1; i++){
							fprintf(buffer, "\t");
						}
						if(element->type == FXML_TYPE_DECLARATION){
							fxml_ePrint(FXML_MSG_ERROR, "Internal error (FXML_TYPE_DECLARATION but looks like FMXL_TYPE_ELEMENT)");
						}
						fprintf(buffer, "</%s>\n", element->val);
					}else{
						if(element->type == FXML_TYPE_DECLARATION){
							fprintf(buffer, " ?>\n");
						}else{
							fprintf(buffer, " />\n");
						}
					}
				}else{
					if(element->type == FXML_TYPE_DECLARATION){
						fprintf(buffer, " ?>\n");
					}else{
						fprintf(buffer, " />\n");
					}
				}

				break;
		}
	}

	if(element->next != NULL){
		fxml_printRecurse(buffer, element->next, printAttrib, depth, breadth + 1);
	}
}

/* Prints a given element structure as an XML-file to stdout callback */
void fxml_print(fxml_element* element)
{
	if(element == NULL){
		fxml_ePrint(FXML_MSG_ERROR, "NULL pointer supplied (fxml_print)");
		return;
	}

	if(element->type == FXML_ERROR){
		fxml_ePrint(FXML_MSG_ERROR, "Invalid Structure");
		return;
	}

	fxml_printRecurse(stdout, element, FALSE, 0, 0);
}

int fxml_save(fxml_document* document, const char* fileName)
{
	FILE* file;

	if(!document){
		fxml_ePrint(FXML_MSG_ERROR, "NULL pointer supplied (fxml_save)");
		return FALSE;
	}

 	file = fopen(fileName, "w");
	
	if(!file){
		fxml_ePrint(FXML_MSG_ERROR, "Couldn't open file '%s' for saving.", fileName);
		return FALSE;
	}

	fxml_printRecurse(file, document->elements, FALSE, 0, 0);

	fclose(file);
	return TRUE;
}
