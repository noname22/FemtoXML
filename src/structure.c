/* FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* structure.c
 *
 * This file contains code modifying femtoxml's representation of
 * XML files in memory. */

#include "femtoxmli.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Returns the "error element", the element returned when a "get" failed */
fxml_element* fxml_errorElement()
{
	static fxml_element* error;
	static int notFirst;

	if(!notFirst){
		notFirst = TRUE;
		error = fxml_newElement(FXML_ERROR);
		error->next = NULL;
		error->children = NULL;
		error->val = (char*)malloc(0xf);
		strcpy(error->val, "(fxml error)");
	}

	return error;	
}

/* Sets the given element to it's own "next"-element if it has one
 * Returns true if there is a next element, and false if not.
 * Used to loop trough an element-structure "vertically" */
int fxml_getNextElement(fxml_element** element)
{
	if((*element)->next != NULL){
		*element = (*element)->next;
		return TRUE;
	}else{
		return FALSE;
	}
}

/* Counts the number of "vertical" elements in a list 
 * (ie. element->next of the elements children, not element->children) */
int fxml_count(fxml_element* element, const char* val, int type)
{
	int count=0;

	if(element->children != NULL){
		element = element->children;
		do{
			if((!strcmp(val, "") || !strcmp(val, element->val)) && (type == FXML_TYPE_ANY || type == element->type)){
				count++;
			}
		}while(fxml_getNextElement(&element));
	}
	return count;
}

/* Returns a pointer to an element with value val and/or type type and index index
 * If val is "", it is ignored
 * If type is FXML_TYPE_ANY it is ignored
 * If index is 0 it returns the first match (of course) 
 * Returns the error element if the get fails */
fxml_element* fxml_get(fxml_element* root, const char* val, int type, int index)
{
	fxml_element* cur;
	int sanityCounter;
	int counter;

	counter = sanityCounter = 0;

	if(root == NULL){
		fxml_ePrint(FXML_MSG_WARNING, "fxml_get was passed a NULL pointer");
		return fxml_errorElement();
	}

	if(root->children != NULL){
		cur = root->children;
	}else{
		fxml_ePrint(FXML_MSG_WARNING, "Couldn't get element with value '%s' and type %d from element '%s' (It doesn't have any sub-elements)", val, type, root->val);
		return fxml_errorElement();
	}

	do{
		if(sanityCounter++ >= FXML_MAXBREADTH){
			fxml_ePrint(FXML_MSG_ERROR, "The linked list seems to be looped. The parsing must have failed. (exceeded FXML_MAXBREADTH, %d)", FXML_MAXBREADTH);
			return fxml_errorElement();
		}

		if(val == NULL || !strcmp(val, "") || !strcmp(cur->val, val)){
			if(type == FXML_TYPE_ANY){
				if(counter++ >= index){
					return cur;
				}
			}else{
				if(cur->type == type && counter++ >= index){
					return cur;
				}
			}
		}
	}while(fxml_getNextElement(&cur));
	
	fxml_ePrint(FXML_MSG_WARNING, "Couldn't get element with value '%s', type %s and index %d from element '%s' (No such item found)", val, fxml_getTypeName(type), index, root->val);
	return fxml_errorElement();
}

/* Allocates a new document and returns a pointer to it */
fxml_document* fxml_newDocument()
{
	fxml_document* doc;
	fxml_element* element;
	
	if( (doc = (fxml_document*)malloc(sizeof(fxml_document))) ){
		element = fxml_newElement(FXML_TYPE_ROOT);
		fxml_setVal(element, "rootnode");
		doc->elements = element;
	}

	return doc;
}

/* Ask the user (through the onParse callback) if the element should be used or thrown away */
int fxml_addOrKillElementStruct(fxml_document* doc, fxml_element* parent, fxml_element* element)
{
	if(doc && (!doc->onParsed || doc->onParsed(element, doc->data))){
		/* Add */
		return fxml_addElementStruct(parent, element);
	}

	/* Or KILL */
	fxml_freeElement(element);

	return FALSE;
}

/* Adds an element as a child to another element.
 * Fails if any of the elements are NULL or if the child and the parent are
 * the same element. (It would create an infinite list) */
int fxml_addElementStruct(fxml_element* parent, fxml_element* element)
{
	fxml_element* children;

	if((!parent || !element) || (parent == element)){
		return FALSE;
	}

	element->parent = parent;

	children = fxml_getLastElement(parent->children);

	if(!children){
		parent->children = element;
	}else{
		children->next = element;
	}
	return TRUE;
}

/* Sets user defined custom data in the element */
void fxml_setCustomData(fxml_element* element, fxml_onFree_fn onFree, void* data)
{
	element->customData.data = data;
	element->customData.onFree = onFree;
}

/* Copies the specified value into an element */
void fxml_setVal(fxml_element* element, const char* val)
{
	unsigned int valLen = fxml_strlen(val);
	element->val = (char*)realloc(element->val, valLen + 1);
	element->val[valLen] = '\0';

	strncpy(element->val, val, valLen);
}

/* Allocates a new element and returns a pointer to it */
fxml_element* fxml_newElement(int type)
{
	fxml_element* element;

	if( !( element = (fxml_element*)malloc(sizeof(fxml_element)) ) ){
		return NULL;
	}

	element->children = NULL;
	element->next = NULL;
	element->parent = NULL;	

	element->val = NULL;

	element->type = type;

	element->customData.data = NULL;
	element->customData.onFree = NULL;

	return element;
}

/* Frees an element structure, started with fxml_freeElementRecurse */
void fxml_freeElementRecurse(fxml_element* element, int depth)
{
	fxml_element* tmp;
	int done=FALSE;

	if(depth > FXML_MAXDEPTH){
		fxml_ePrint(FXML_MSG_ERROR, "Couldn't free element, reached max recurse depth (fxml_freeElementRecurse as called by fxml_freeElement). Bug report me!");
		return;
	}

	do{
		tmp = element;
		if(element->children != NULL){
			fxml_freeElementRecurse(element->children, depth + 1);
		}
		
		done = !fxml_getNextElement(&element);

		/* printf("Freeing element with value: '%s'\n", tmp->val); */

		if(element->customData.onFree){
			element->customData.onFree(element, element->customData.data);
		}
	
		free(tmp->val);
		free(tmp);
	}while(!done);
}

/* Starter for fxml_freeElementRecurse  */
void fxml_freeElement(fxml_element* element)
{
	fxml_freeElementRecurse(element, 0);
}

/* Frees a document */
void fxml_freeDocument(fxml_document* document)
{
	fxml_freeElement(document->elements);
	free(document);
}

/* Loops through to the last of a given list of elements and returns a pointer to it */
fxml_element* fxml_getLastElement(fxml_element* element)
{
	fxml_element* step;
	int insane = 0;

	if(!element){
		return NULL;
	}
	
	step = element;

	while(step->next != NULL){
		step = step->next;
		/*fxml_ePrint(FXML_MSG_DEBUG, "fxml_getLastElement(), insane is: %u", insane);*/
		if(insane++ > FXML_MAXBREADTH){
			fxml_ePrint(
				FXML_MSG_ERROR, 
				"fxml_getLastElement() reached FXML_MAXBREADTH (%u). Is the linked list looped?", 
				FXML_MAXBREADTH
			);
			break;
		}
	}
 
	return step;
}

/* Counts the number of elements with a given type. 
 * If invert is true, it returns the number of elements NOT matching type instead. */
int fxml_countElements(fxml_element* element, int type, int invert)
{
	fxml_element* step;
	int count = 0;

	if(!element){
		return 0;
	}

	step = element;

	do{
		if(
			( invert && step->type != type) ||
			(!invert && step->type == type)
		){
			count++;
		}
		step = step->next;
	}while(step != NULL);
 
	return count;
}

/* Constructs a new element, adds it as a child to another given element and returns
 * a pointer to the new element (since you probably want to add something to it) */
fxml_element* fxml_addElement(fxml_element* element, const char* name)
{
	fxml_element* elem;
	
	if(!element || name == NULL || fxml_strlen(name) <= 0){
		return NULL;
	}

	elem = fxml_newElement(FXML_TYPE_ELEMENT);
	fxml_setVal(elem, name);
	fxml_addElementStruct(element, elem);
	return elem;
}

/* Cconstructs a new text element from the given text and adds it to the given element */
fxml_element* fxml_addSimple(fxml_element* element, int type, const char* text)
{
	fxml_element *textElem;

	if(!element){
		return NULL;
	}

	textElem = fxml_newElement(type);
	fxml_setVal(textElem, text);
	fxml_addElementStruct(element, textElem);
	
	return textElem;
}

/* Wrappers from text, comment, doctype and declaration */
fxml_element* fxml_addText(fxml_element* element, const char* text)
{
	return fxml_addSimple(element, FXML_TYPE_TEXT, text);
}

fxml_element* fxml_addDeclaration(fxml_element* element, const char* name)
{
	return fxml_addSimple(element, FXML_TYPE_DECLARATION, name);
}
fxml_element* fxml_addComment(fxml_element* element, const char* text)
{
	return fxml_addSimple(element, FXML_TYPE_COMMENT, text);
}
fxml_element* fxml_addDoctype(fxml_element* element, const char* name)
{
	return fxml_addSimple(element, FXML_TYPE_DOCTYPE, name);
}

/* Constructs an attribute from the given pair of value and name, and adds that to the given element */
fxml_element* fxml_addAttribute(fxml_element* element, const char* name, const char* val)
{
	fxml_element *attribute, *value;

	if(!element){
		return NULL;
	}

	attribute = fxml_newElement(FXML_TYPE_ATTRIBUTE);
	fxml_setVal(attribute, name);
	fxml_addElementStruct(element, attribute);

	if(val != NULL){
		value = fxml_newElement(FXML_TYPE_ATTRIBUTE_VAL);
		fxml_setVal(value, val);
		fxml_addElementStruct(attribute, value);
	}

	return attribute;
}

/* Sets the onParsed callback pointer in document, which is called every time the parser finishes parsing
 * a new element, but before it is added to the document. If the callback returns 1 the elemnt is added
 * otherwise it's discarded. */
void fxml_setOnParsed(fxml_document* doc, fxml_onParsed_fn onParsed, void* data)
{
	if(doc){
		doc->data = data;
		doc->onParsed = onParsed;
	}
}
