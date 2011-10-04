/* FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* parse.c
 *
 * This file contains code for parsing XML-files and storing the data in  
 * femtoxml's internal data structures. */

/* #define FXML_DEBUG */

#include "femtoxmli.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

/* Used internally for identifying tags etc. */
enum{
	FXML_PARSE_START,
	FXML_PARSE_DECLARATION,
	FXML_PARSE_TAG,
	FXML_PARSE_EMPTYTAG,
	FXML_PARSE_ENDTAG,
	FXML_PARSE_EMPTYATTRIBUTE,
	FXML_PARSE_ATTRIBUTE,
	FXML_PARSE_ATTRIBUTE_VAL,
	FXML_PARSE_TEXT,
	FXML_PARSE_COMMENT,
	FXML_PARSE_DOCTYPE,
	FXML_PARSE_FOE,
	FXML_PARSE_ERROR
};


/* Sets the value in a given fxml_element from a given buffer */
void fxml_fsetVal(fxml_element* element, fxml_vFile* buffer, int len)
{
	int read, i;
	FXML_PRINTFUNCTION

	element->val = (char*)realloc(element->val, len + 1);
	/* fxml_vfread(element->val, len, 1, buffer); */

	for(i=0; i < len; i++){
		if((read = fxml_vfgetc(buffer)) > 31){
			element->val[i] = read;
		}else{
			element->val[i] = ' ';
		}
	}
	
	element->val[len] = '\0';
}

/* Jumps to the next word in a buffer */
/* TODO: May have UTF-8 compatibility issues.
 * A second, third or fourth byte in a UTF-8 encoded character may have a value of 
 * less than 32 without being a special/function character? */
int fxml_nextWord(fxml_vFile* buffer)
{
	int parse;
	FXML_PRINTFUNCTION
	do{
		parse = fxml_vfgetc(buffer);
		#ifdef FXML_DEBUG
		fxml_ePrint(FXML_MSG_DEBUG, "got chr: %i", (int)parse);
		#endif
	}while(parse <= 32 && parse != FXML_EOF);

	if(parse == FXML_EOF){
		return FALSE;
	}

	fxml_vfseek(buffer, -1, SEEK_CUR);

	return TRUE;
}

/* Returns the length the first element in the buffer. *
 * The length is determined differently depending on what status
 * the parser is in (which type of element it is parsing). */

int fxml_getLen(fxml_vFile* buffer, int status){
	long pos, initPos;
	fxml_filePos position;
	int parse, tmp, count = 0;

	FXML_PRINTFUNCTION
	
	tmp = -1;

	initPos = fxml_vftell(buffer);

	/* printf("Getlen sees: "); */

	while( (parse = fxml_vfgetc(buffer)) != FXML_EOF){
		
		#ifdef FXML_DEBUG
		fxml_ePrint(FXML_MSG_DEBUG, "%c, %d, %d\n", parse, parse, count);
		#endif

		/* printf("%c", parse); */
		pos = fxml_vftell(buffer);

		switch(status){
			case FXML_PARSE_DECLARATION:
				if(parse == '?' && fxml_vfgetc(buffer) == '>'){
					fxml_vfseek(buffer, initPos, SEEK_SET);
					/* printf("\n"); */
					return count;
				}else{
					fxml_vfseek(buffer, pos, SEEK_SET);
				}
				break;

			case FXML_PARSE_COMMENT:
				if(parse == '-' && fxml_vfgetc(buffer) == '-' && fxml_vfgetc(buffer) == '>'){
					fxml_vfseek(buffer, initPos, SEEK_SET);
					/* printf("\n"); */
					return count;
				}else{
					fxml_vfseek(buffer, pos, SEEK_SET);
				}
				break;

			case FXML_PARSE_TEXT:
				if(parse == '<'){
					fxml_vfseek(buffer, initPos, SEEK_SET);
					/* printf("\n"); */
					return count;
				}
				break;
			
			case FXML_PARSE_TAG:
			case FXML_PARSE_EMPTYTAG:
			case FXML_PARSE_ENDTAG:
			case FXML_PARSE_DOCTYPE:
				if(parse == '>'){
					fxml_vfseek(buffer, initPos, SEEK_SET);
					/* printf("\n"); */
					return count;
				}
				
				if(parse == '/' && fxml_vfgetc(buffer) == '>'){
					fxml_vfseek(buffer, initPos, SEEK_SET);
					/* printf("\n"); */
					return count;
				}else{
					fxml_vfseek(buffer, pos, SEEK_SET);
				}
				break;

			case FXML_PARSE_ATTRIBUTE:
			case FXML_PARSE_EMPTYATTRIBUTE:
				if(parse < 32 || parse == '=' || parse == '>' || parse == '/' || parse == ' '){
					fxml_vfseek(buffer, initPos, SEEK_SET);
					/* printf("\n"); */

					#ifdef FXML_DEBUG
					fxml_ePrint(FXML_MSG_DEBUG, "Returning length of attribute, %i", count);
					fxml_ePrint(FXML_MSG_DEBUG, "We're at position %i in the file", fxml_vftell(buffer));
					#endif
					return count;
				}
				break;

			case FXML_PARSE_ATTRIBUTE_VAL:
				if(tmp == -1){
					if(parse == '\'' || parse == '"'){
						tmp = parse;
					}else{
						/* ERROR */
						/* printf("Error: expected citation, not text\n"); */
						position = fxml_getFilePos(buffer);
						fxml_ePrint(FXML_MSG_ERROR, "Expected citation, not text, in attribute value (should be <tag attribute=\"value\") (line: %d column: %d)", position.line, position.col);
						fxml_vfseek(buffer, initPos, SEEK_SET);
						/* printf("\n"); */
						return FXML_ERROR;
					}
				}else{
					if(parse == tmp){
						fxml_vfseek(buffer, initPos, SEEK_SET);
						/* printf("\n"); */
						return count - 1;
					}
				}
				break;

			default:
				if(parse < 32){
					fxml_vfseek(buffer, initPos, SEEK_SET);
					/* printf("\n"); */
					return count;
				}
				break;
		}
		count++;
	}

	/* printf("\n"); */

	if(parse != FXML_EOF){
		fxml_vfseek(buffer, initPos, SEEK_SET);
	}
	/*else{
		fxml_ePrint(FXML_MSG_ERROR, "Unexpected end of file");
	}*/

	return count;
}

/* All the fxml_is-functions work like the fxml_isTag function, but for their respective target type.
 *
 * If the given buffer starts with a tag (<hello>), fxml_isTag steps into that tag and returns true.
 * If it isn't a tag, it resets the buffer pointer and returns false */

int fxml_isTag(fxml_vFile* buffer)
{
	long pos, pos2;
	
	FXML_PRINTFUNCTION

	pos = fxml_vftell(buffer);

	fxml_nextWord(buffer);
	pos2 = fxml_vftell(buffer) + 1;
	
	if(fxml_vfgetc(buffer) == '<' && !(fxml_vfgetc(buffer) == '!' && fxml_vfgetc(buffer) == '-' && fxml_vfgetc(buffer) == '-')){
		fxml_vfseek(buffer, pos2, SEEK_SET);
		if(fxml_vfgetc(buffer) != '/'){
			fxml_vfseek(buffer, pos2, SEEK_SET);
			return TRUE;
		}
	}

	fxml_vfseek(buffer, pos, SEEK_SET);
	return FALSE;
}

/* Checks whether the current buffer starts with a declaration, ie. <? hello ?> (behaves like fxml_isTag)*/
int fxml_isDeclaration(fxml_vFile* buffer)
{
	long pos;
	
	FXML_PRINTFUNCTION

	pos = fxml_vftell(buffer);

	fxml_nextWord(buffer);

	if(fxml_vfgetc(buffer) == '<' && fxml_vfgetc(buffer) == '?'){
		return TRUE;
	}

	fxml_vfseek(buffer, pos, SEEK_SET);
	return FALSE;
}

/* Checks whether the current buffer starts with a doctype, ie. <!hello> (behaves like fxml_isTag)*/
int fxml_isDoctype(fxml_vFile* buffer)
{
	long pos;

	FXML_PRINTFUNCTION

	pos = fxml_vftell(buffer);

	fxml_nextWord(buffer);

	if(
		fxml_vfgetc(buffer) == '<' && 
		fxml_vfgetc(buffer) == '!' && 
		fxml_vfgetc(buffer) == 'D' &&
		fxml_vfgetc(buffer) == 'O' &&
		fxml_vfgetc(buffer) == 'C' &&
		fxml_vfgetc(buffer) == 'T' &&
		fxml_vfgetc(buffer) == 'Y' &&
		fxml_vfgetc(buffer) == 'P' &&
		fxml_vfgetc(buffer) == 'E' &&
		fxml_vfgetc(buffer) == ' ' 
	){
		return TRUE;
	}

	fxml_vfseek(buffer, pos, SEEK_SET);
	return FALSE;
}

/* Checks whether the current buffer starts with an empty tag, ie. <hello /> (behaves like fxml_isTag)*/
int fxml_isEmptyTag(fxml_vFile* buffer)
{
	int pos, pos2, len, get;

	FXML_PRINTFUNCTION

	pos2 = fxml_vftell(buffer);

	if(fxml_isTag(buffer)){
		pos = fxml_vftell(buffer);

		len = fxml_getLen(buffer, FXML_PARSE_TAG);

		fxml_vfseek(buffer, len, SEEK_CUR);

		get = fxml_vfgetc(buffer);
		
		if(get == '/'){
			fxml_vfseek(buffer, pos, SEEK_SET);
			return TRUE;
		}else{
			fxml_vfseek(buffer, pos2, SEEK_SET);
		}
	}
	

	return FALSE;
}

/* Checks whether the current buffer starts with an endtag, ie. </hello> (behaves like fxml_isTag)*/
int fxml_isEndTag(fxml_vFile* buffer)
{
	long pos, pos2;

	FXML_PRINTFUNCTION

	pos = fxml_vftell(buffer);

	fxml_nextWord(buffer);
	pos2 = fxml_vftell(buffer) + 2;
	
	if(fxml_vfgetc(buffer) == '<' && fxml_vfgetc(buffer) == '/'){
		fxml_vfseek(buffer, pos2, SEEK_SET);
		return TRUE;
	}

	fxml_vfseek(buffer, pos, SEEK_SET);
	return FALSE;
}

/* Checks whether the current buffer starts with a comment, ie. <!-- hello --> (behaves like fxml_isTag)*/
int fxml_isComment(fxml_vFile* buffer)
{
	long pos;

	FXML_PRINTFUNCTION

	pos = fxml_vftell(buffer);

	fxml_nextWord(buffer);

	if(fxml_vfgetc(buffer) == '<' && fxml_vfgetc(buffer) == '!' && fxml_vfgetc(buffer) == '-' && fxml_vfgetc(buffer) == '-'){
		return TRUE;
	}

	fxml_vfseek(buffer, pos, SEEK_SET);
	return FALSE;
}

/* Checks if we've reached the end of a buffer */
int fxml_isEndOfBuffer(fxml_vFile* buffer)
{

	FXML_PRINTFUNCTION

	if(fxml_vfgetc(buffer) != FXML_EOF){
		fxml_vfseek(buffer, -1, SEEK_CUR);
		return FALSE;
	}
	return TRUE;
}

/* Compares two strings. Case INsensitive. TRUE if they're the same. TODO UTF-8 weirdness */
int fxml_strncmp(unsigned int n, const char* a, const char* b)
{
	int i;

	for(i = 0; i < n; i++){
		if(tolower(a[i]) != tolower(b[i])){
			return FALSE;
		}

		if(a[i] == '\0' || b[i] == '\0'){
			return TRUE;
		}
	}

	return TRUE;
}

/* Checks if the endtag matches the starttag */
int fxml_checkEndTag(char* tagName, fxml_vFile* buffer, int len)
{

	char* name = (char*)malloc(len + 1);
	
	FXML_PRINTFUNCTION
	
	name[len] = '\0';
	fxml_vfread(name, len, 1, buffer);
	if(fxml_strncmp(FXML_MAXSTRLEN, name, tagName)){
		free(name);
		return TRUE;
	}
	free(name);
	return FALSE;
}

/* Checks if a given buffer contains contains 'needle', and the buffer starts with the type 'type' */
/* Jumps to the 'needle' if it exists. Position reset if not. */
int fxml_fcontains(fxml_vFile* buffer, int type, int needle){
	long pos;
	int i, len, ret;

	FXML_PRINTFUNCTION
	#ifdef FXML_DEBUG
	fxml_ePrint(FXML_MSG_DEBUG, "type is: '%s' (%i), needle is '%c' (%i)", fxml_getTypeName(type), type, (char)needle, needle);
	#endif

	pos = fxml_vftell(buffer);

	len = fxml_getLen(buffer, type);

	ret = FALSE;

	for(i=0; i < len; i++){
		if(fxml_vfgetc(buffer) == needle){
			ret = TRUE;
			FXML_DPRINT("Returning true");
		}
	}

	fxml_vfseek(buffer, pos, SEEK_SET);
	return ret;
}

/* Uses the fxml_is...-functions to identify which type of element a buffer starts with */
int fxml_identify(fxml_vFile* buffer, int status)
{

	FXML_PRINTFUNCTION

	switch(status){
		case FXML_PARSE_START:
			FXML_DPRINT("status is FXML_PARSE_START");
			if(fxml_isDeclaration(buffer)){
				FXML_DPRINT("Identified declaration");
				return FXML_PARSE_DECLARATION;
			}else

			if(fxml_isEmptyTag(buffer)){
				FXML_DPRINT("Identified an empty tag");
				return FXML_PARSE_EMPTYTAG;
			}else

			if(fxml_isComment(buffer)){
				FXML_DPRINT("Identified a comment");
				return FXML_PARSE_COMMENT;
			}else
			
			if(fxml_isDoctype(buffer)){
				FXML_DPRINT("Identified a doctype");
				return FXML_PARSE_DOCTYPE;
			}else

			if(fxml_isTag(buffer)){
				FXML_DPRINT("Identified a tag");
				return FXML_PARSE_TAG;
			}else

			if(fxml_isEndTag(buffer)){
				FXML_DPRINT("Identified an endtag");
				return FXML_PARSE_ENDTAG;
			}

			else if(!fxml_isEndOfBuffer(buffer)){
				fxml_nextWord(buffer);
				FXML_DPRINT("Identified text");
				return FXML_PARSE_TEXT;
			}

			return FXML_PARSE_FOE;

			break;
		case FXML_PARSE_TAG:
			FXML_DPRINT("status is FXML_PARSE_TAG");

			if(fxml_fcontains(buffer, FXML_PARSE_ATTRIBUTE, '=')){
				FXML_DPRINT("returning FXML_PARSE_EMPTYATTRIBUTE");
				return FXML_PARSE_EMPTYATTRIBUTE;
			}
	
			FXML_DPRINT("returning FXML_PARSE_ATTRIBUTE");

			return FXML_PARSE_ATTRIBUTE;
			break;
	}

	FXML_DPRINT("Couldn't identify anything");
	return FXML_PARSE_ERROR;
}

/* Constructs an fxml_element from the given buffer in the given length with the given type */
fxml_element* fxml_parseTag(fxml_vFile* buffer, int len, int addType)
{
	fxml_element* element, *add, *val;
	int parseLen, type, aValLen;
	long pos;
	int safe;
	
	FXML_PRINTFUNCTION

	pos = fxml_vftell(buffer);

	element = fxml_newElement(addType);

	#ifdef FXML_DEBUG
	fxml_ePrint(FXML_MSG_DEBUG, "Parsing an element declaration"); 
	#endif
	
	parseLen = fxml_getLen(buffer, FXML_PARSE_ATTRIBUTE);
	fxml_fsetVal(element, buffer, FXML_MIN(len, parseLen));

	#ifdef FXML_DEBUG
	fxml_ePrint(FXML_MSG_DEBUG, "found the tag name: '%s' with length '%i'", element->val, parseLen);
	#endif

	safe = 0;
	while(safe < FXML_MAXATTRIBS){
		/* printf("Safe is %d\n", safe); */
		safe++;
		
		#ifdef FXML_DEBUG
		fxml_ePrint(FXML_MSG_DEBUG, "Fetching next word");
		#endif

		fxml_nextWord(buffer);

		if(fxml_vftell(buffer) - pos >= len){
			#ifdef FXML_DEBUG
			fxml_ePrint(FXML_MSG_DEBUG, "Done parsing tag");
			#endif
			return element;
		}

		add = fxml_newElement(FXML_TYPE_ATTRIBUTE);

		type = fxml_identify(buffer, FXML_PARSE_TAG);
		/* printf("type: %d\n", type); */
		if((parseLen = fxml_getLen(buffer, type)) == 0){
			fxml_filePos position = fxml_getFilePos(buffer);
			fxml_ePrint(FXML_MSG_ERROR, "Syntax error (line: %d column: %d)", position.line, position.col);
			return element;
		};

		if((int)(fxml_vftell(buffer) - pos) + parseLen > len){
			/* printf("\tFound the last attribute "); */
			fxml_fsetVal(add, buffer, len - (int)(fxml_vftell(buffer) - pos));
		} else {
			/* printf("\tFound an attribute "); */
			fxml_fsetVal(add, buffer, parseLen);
		}

		/* printf("'%s'\n", add->val); */
		fxml_nextWord(buffer);

		if(fxml_vfgetc(buffer) == '='){
			fxml_nextWord(buffer);
			
			if((aValLen = fxml_getLen(buffer, FXML_PARSE_ATTRIBUTE_VAL)) != FXML_ERROR){
				fxml_vfseek(buffer, 1, SEEK_CUR);
				val = fxml_newElement(FXML_TYPE_ATTRIBUTE_VAL);
				fxml_fsetValEntityDecode(val, buffer, aValLen);
				fxml_addElementStruct(add, val);
				/* printf("\tFound an attribute value: '%s'\n", val->val); */
				fxml_vfseek(buffer, 1, SEEK_CUR);
			}
			
		}else{
			fxml_vfseek(buffer, -1, SEEK_CUR);
		}

	
		fxml_addElementStruct(element, add);
	}
	return element;
}
/* Calculates what line and column a given file is currently at */
fxml_filePos fxml_getFilePos(fxml_vFile* file)
{
	long counter, pos;
	fxml_filePos ret;
	
	FXML_PRINTFUNCTION
	
	pos = fxml_vftell(file);

	ret.line = ret.col = 1;

	fxml_vfseek(file, 0, SEEK_SET);

	for(counter = 0; counter < pos; counter++){
		if(fxml_vfgetc(file) == '\n'){
			ret.col = 1;
			ret.line++;
		}
		ret.col++;
	}
	
	fxml_vfseek(file, pos, SEEK_SET);
	return ret;
}

/* Constructs an fxml_document from a given buffer.
 * element is used internally by the function (it's recursive) 
 * should be initiated like in fxml_load with fxml_fparse(buffer, document, document->elements)*/
void fxml_fparse(fxml_vFile* buffer, fxml_document* document, fxml_element* element)
{
	int len, parse, status, type, seekAdd;
	fxml_element* add;
	fxml_filePos position;

	FXML_PRINTFUNCTION

	status = FXML_PARSE_START;

	while((parse = fxml_identify(buffer, status)) != FXML_PARSE_ERROR && parse != FXML_PARSE_FOE ){
		switch(parse){
			case FXML_PARSE_COMMENT:
			case FXML_PARSE_DOCTYPE:
				if(parse == FXML_PARSE_COMMENT){
					type = FXML_TYPE_COMMENT;
					seekAdd = 3;
				}else{
					type = FXML_TYPE_DOCTYPE;
					seekAdd = 1;
				}
				len = fxml_getLen(buffer, parse);
				/* printf("Identified a comment with length: %d\n\n", len); */

				add = fxml_newElement(type);
				fxml_fsetVal(add, buffer, len);
				fxml_addOrKillElementStruct(document, element, add);
				fxml_vfseek(buffer, seekAdd, SEEK_CUR);
				break;

			case FXML_PARSE_EMPTYTAG:
			case FXML_PARSE_TAG:
			case FXML_PARSE_DECLARATION:

				len = fxml_getLen(buffer, parse);

				if(parse == FXML_PARSE_DECLARATION){
					/* printf("Identified a declaration with length %d\n\n", len); */
					add = fxml_parseTag(buffer, len, FXML_TYPE_DECLARATION);
				}else{
					/* printf("Identified a tag with length %d\n\n", len); */
					add = fxml_parseTag(buffer, len, FXML_TYPE_ELEMENT);
				}

				if(!add){
					position = fxml_getFilePos(buffer);
					/* printf("Error!\n"); */
					fxml_ePrint(FXML_MSG_ERROR, "Couldn't parse tag (line: %d column: %d)", position.line, position.col);
					/* ERROR */
					break;
				}

				fxml_addOrKillElementStruct(document, element, add);

				if(parse == FXML_PARSE_TAG){
					fxml_vfseek(buffer, 1, SEEK_CUR);
					fxml_fparse(buffer, document, add);
				}else{
					fxml_vfseek(buffer, 2, SEEK_CUR);
				}

				break;

			case FXML_PARSE_ENDTAG:
				len = fxml_getLen(buffer, FXML_PARSE_ENDTAG);
				/* printf("Identified an endtag with length %d\ndeath\n\n", len); */

				if(!fxml_checkEndTag(element->val, buffer, len)){
					position = fxml_getFilePos(buffer);
					fxml_ePrint(FXML_MSG_ERROR, "Tag / end tag mismatch, should be '</%s>' (line: %d column: %d)", element->val, position.line, position.col);
				}else{
					/* printf("End tag matches tag\n"); */
				}

				fxml_vfseek(buffer, 1, SEEK_CUR);

				return;

				break;

			case FXML_PARSE_TEXT:
				len = fxml_getLen(buffer, FXML_PARSE_TEXT);
				/* printf("Identified text with length %d, inserting into element '%s'\n\n", len, element->val); */
				
				add = fxml_newElement(FXML_TYPE_TEXT);
				fxml_fsetValEntityDecode(add, buffer, len);
				fxml_addOrKillElementStruct(document, element, add);

				/* printf("\tvalue: '%s'\n", add->val); */
				break;
		}
	}
}

fxml_document* fxml_loadBufferInspect(const unsigned char* buffer, long len, fxml_onParsed_fn onParsed, void* data)
{
	fxml_document* document;
	fxml_vFile* file = fxml_vffromBuffer(buffer, len);

	FXML_PRINTFUNCTION

	if(!file){
		return NULL;
	}

	document = fxml_newDocument();

	fxml_setOnParsed(document, onParsed, data);

	fxml_fparse(file, document, document->elements);
	
	fxml_vfclose(file);	

	return document;
}

fxml_document* fxml_loadBuffer(const unsigned char* buffer, long len)
{
	return fxml_loadBufferInspect(buffer, len, NULL, NULL);
}

fxml_document* fxml_loadInspect(const char* fileName, fxml_onParsed_fn onParsed, void* data)
{
	fxml_document* document;
	fxml_vFile* file = fxml_vfopen(fileName);

	FXML_PRINTFUNCTION

	if(!file){
		fxml_ePrint(FXML_MSG_ERROR, "Couldn't open file '%s'.", fileName);
		return NULL;
	}

	document = fxml_newDocument();

	fxml_setOnParsed(document, onParsed, data);

	fxml_fparse(file, document, document->elements);
	
	fxml_vfclose(file);	

	return document;
}

fxml_document* fxml_load(const char* fileName)
{
	return fxml_loadInspect(fileName, NULL, NULL);
}
