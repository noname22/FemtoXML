/* FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* femtoxml.h
 *
 * This file contains all declarations needed to write an application using femtoxml 
 * */

#ifndef FEMTOXML_H
#define FEMTOXML_H

#ifndef TRUE
#	define TRUE 1
#endif

#ifndef FALSE
#	define FALSE 0
#endif

#define FXML_VERSION_MAJOR	0
#define	FXML_VERISON_MINOR	95
#define FXML_VERSION_STRING	"FemtoXML 0.95"

#ifdef __cplusplus
extern "C" {
#endif

/* Message types */
enum{
	FXML_MSG_ERROR,
	FXML_MSG_WARNING,
	FXML_MSG_INFO,
	FXML_MSG_DEBUG
};

/* Element types */
enum{
	FXML_TYPE_ROOT,
	FXML_TYPE_TEXT,
	FXML_TYPE_ELEMENT,
	FXML_TYPE_ATTRIBUTE,
	FXML_TYPE_ATTRIBUTE_VAL,
	FXML_TYPE_COMMENT,
	FXML_TYPE_DECLARATION,
	FXML_TYPE_DOCTYPE,
	FXML_TYPE_ANY,
	FXML_TYPE_NONE
};

struct _fxml_element;

/* Pointer to a callback called before an fxml_element is freed */
typedef void (*fxml_onFree_fn)(struct _fxml_element* element, void* data);

/* Element custom data structure */ 
typedef struct {
	void* data;
	fxml_onFree_fn onFree;
} fxml_customData;

/* Element data structure (single linked list in two dimensions) */
typedef struct _fxml_element{
	struct _fxml_element* children;
	struct _fxml_element* parent;

	struct _fxml_element* next;

	char* val;
	int type;

	/* Custom data */
	fxml_customData customData;
}fxml_element;

/* Callback called before a new element is added to the document */
typedef int (*fxml_onParsed_fn)(fxml_element* element, void* data);

/* The document structure */
typedef struct{
	fxml_element* elements;

	fxml_onParsed_fn onParsed;
	void* data;
}fxml_document;

/* Message structure */
typedef struct _fxml_msg
{
	int type;
	const char* typeName;
	const char* msg;
} fxml_msg;

/* Message callback function pointer type */
typedef void (*fxml_callback_fn)(fxml_msg* msg, void* data);

/* A "callback set" structure. A pointer to a message callback and a void 
 * pointer to user specified data to go along with the messages */
typedef struct {
	fxml_callback_fn cb;
	void* data;
} fxml_callbackSet;

/* Library functions */
/* Document */
fxml_document* fxml_newDocument();
void fxml_freeDocument(fxml_document* document);
fxml_document* fxml_load(const char* fileName);
fxml_document* fxml_loadInspect(const char* fileName, fxml_onParsed_fn onParsed, void* data);
 
fxml_document* fxml_loadBuffer(const unsigned char* buffer, long len);
fxml_document* fxml_loadBufferInspect(const unsigned char* buffer, long len, fxml_onParsed_fn onParsed, void* data);

void fxml_setOnParsed(fxml_document* document, fxml_onParsed_fn onParse, void* data);

/* Elements */
/* Free / create */
fxml_element* fxml_newElement(int type);
void fxml_freeElement(fxml_element* element);

/* Add data */
int fxml_addElementStruct(fxml_element* parent, fxml_element* element);

fxml_element* fxml_addAttribute(fxml_element* element, const char* name, const char* val);
fxml_element* fxml_addElement(fxml_element* element, const char* name);
fxml_element* fxml_addText(fxml_element* element, const char* text);
fxml_element* fxml_addDeclaration(fxml_element* element, const char* name);
fxml_element* fxml_addComment(fxml_element* element, const char* text);
fxml_element* fxml_addDoctype(fxml_element* doctype, const char* name);

/* Extract data */
fxml_element* fxml_get(fxml_element* root, const char* val, int type, int index);
int fxml_count(fxml_element* element, const char* val, int type);

/* Set data */
void fxml_setVal(fxml_element* element, const char* val);
void fxml_setCustomData(fxml_element* element, fxml_onFree_fn onFree, void* data);

/* Output data */
void fxml_print(fxml_element* element);
int fxml_save(fxml_document* document, const char* fileName);
void fxml_printSimple(fxml_element* element);

/* Help functions */
fxml_element* fxml_errorElement();
int fxml_getNextElement(fxml_element** element);
const char* fxml_getTypeName(unsigned int type);
void fxml_ePrint(int type, const char *format, ...);
void fxml_setCallback(fxml_callback_fn newCb, void* data);

#ifdef __cplusplus
}
#endif
#endif
