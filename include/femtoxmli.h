/* FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* femtoxmli.h
 *
 * This file contains all declarations used internally in femtoxml between the
 * different source files. It isn't needed by an application using femtoxml.
 *
 * If you wish to increase the limits of femtoxml, do so here, but note that 
 * the library has to be recompiled for the changes to take effect.
 * */

#ifndef FEMTOXMLI_H
#define FEMTOXMLI_H

#include <stdio.h>

#include "femtoxml.h"

/* Limits */
#define FXML_MAXSTRLEN 0xffff			/* Maximum length of a string in FemtoXML (tagnames, etc.) */
#define FXML_MAXDEPTH 0xffff			/* Maximum document depth (ie. <tag depth="1"><tag depth="2">At depth 3</tag></tag> ) */
#define FXML_MAXBREADTH 0xffff			/* Maximum document breadth (ie. <tag breadth="1"></tag><tag breadth="2"></tag>At breadth 3*/
#define FXML_MAXATTRIBS 0xffff			/* Maximum number of attributes accompanying an element (ie. <tag attr="1" attr="2">) */
#define FXML_MAX_MESSAGELEN 512			/* Maximum length of messages (error messages etc.) */

/* Constants */
#define FXML_ERROR -1

/* vFile structure */
typedef struct{
	long pos;
	long len;
	const unsigned char* buffer;
	unsigned char* rwBuffer;
}fxml_vFile;

/* Position in file struct (used for errors) */
typedef struct{
	long line;
	long col;
}fxml_filePos;

/* Helper functions */
int fxml_strlen(const char* str);
int fxml_contains(char* buffer, char needle, int len);

fxml_filePos fxml_getFilePos(fxml_vFile* file);
int fxml_countElements(fxml_element* element, int type, int invert);
fxml_element* fxml_getLastElement(fxml_element* element);

int fxml_addOrKillElementStruct(fxml_document* doc, fxml_element* parent, fxml_element* element);

/* vFile, "virtual" file buffer */

#define FXML_SEEK_SET 0
#define FXML_SEEK_CUR 1
#define FXML_SEEK_END 2

#define FXML_EOF 512

/* Like the corresponding standard c library functions*/

#define FXML_WITHIN (file && file->pos < file->len && file->pos >= 0)
#define fxml_vftell(file) file->pos

int fxml_vfseek(fxml_vFile* file, long offset, int origin);
int fxml_vfgetc(fxml_vFile* file);
int fxml_vfread(void* buffer, long size, long num, fxml_vFile* file);

/* Read a file from disk into the buffer */
fxml_vFile* fxml_vfopen(const char* fname);

/* Close the buffer */
void fxml_vfclose(fxml_vFile* file);

/* Create a virtual file from a memory instance */
fxml_vFile* fxml_vffromBuffer(const unsigned char* buffer, long len);

/* Debug macros */
#ifdef FXML_DEBUG
#	define FXML_PRINTFUNCTION fxml_ePrint(FXML_MSG_DEBUG, "%s", __func__);
#	define FXML_DPRINT(string) fxml_ePrint(FXML_MSG_DEBUG, "%s: %s", __func__, string);
#else
#	define FXML_PRINTFUNCTION
#	define FXML_DPRINT(string)
#endif


#endif
