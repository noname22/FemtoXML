/* FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* vfile.c
 *
 * This file contains code for the vFile structure, which handles
 * a memory buffer like a C stream. */

/* #define FXML_DEBUG */

#include "femtoxml.h"
#include "femtoxmli.h"

#include <stdlib.h>

void fxml_vfprint(fxml_vFile* file)
{
	long i;
	FXML_PRINTFUNCTION
	#ifdef FXML_DEBUG
	fxml_ePrint(FXML_MSG_DEBUG, "File length is: %li", file->len);
	#endif
	for(i = 0; i < file->len; i++){
		putchar(file->buffer[i]);
	}
}

int fxml_vfseek(fxml_vFile* file, long offset, int origin)
{
	FXML_PRINTFUNCTION

	if(!file){
		return -1;
	}

	switch(origin){
		case FXML_SEEK_SET:
			if(offset >= 0){
				file->pos = offset;
			}
			break;

		case FXML_SEEK_CUR:
			if(offset + file->pos >= 0){
				file->pos += offset;
			}
			break;

		case FXML_SEEK_END:
			if(file->len + offset >= 0){
				file->pos = file->len + offset;
			}
			break;
	}

	return 0;
}

int fxml_vfgetc(fxml_vFile* file)
{
	FXML_PRINTFUNCTION

	#ifdef FXML_DEBUG
	fxml_ePrint(FXML_MSG_DEBUG, "Position is %li", file->pos);
	#endif

	if(FXML_WITHIN){
		return file->buffer[file->pos++];
	}
	file->pos++;
	return FXML_EOF;
}

#if 0
int fxml_vfputc(fxml_vFile* file, unsigned char val)
{
	FXML_PRINTFUNCTION

	#ifdef FXML_DEBUG
	fxml_ePrint(FXML_MSG_DEBUG, "Gets: %i (%c)", val, val);
	#endif

	if(FXML_WITHIN){
		file->buffer[file->pos++] = val;
		return val;
	}
	return FXML_EOF;
}

/* Set a value in the buffer */
int fxml_vfset(fxml_vFile* file, unsigned char val)
{
	FXML_PRINTFUNCTION
	if(FXML_WITHIN){
		file->buffer[file->pos] = val;
		return TRUE;
	}
	return FALSE;
}
#endif

fxml_vFile* fxml_vffromBuffer(const unsigned char* buffer, long len)
{
	fxml_vFile* ret;
	
	FXML_PRINTFUNCTION
	
	ret = (fxml_vFile*)malloc(sizeof(fxml_vFile));
	if(ret){
		ret->len = len;
		ret->pos = 0;
		ret->buffer = buffer;
		ret->rwBuffer = NULL;

		return ret;
	}else{
		fxml_ePrint(FXML_MSG_ERROR, "Couldn't allocate memory for parse buffer container");
	}
	return NULL;
}

fxml_vFile* fxml_vffromBufferRw(unsigned char* buffer, long len)
{
	fxml_vFile* ret;

	FXML_PRINTFUNCTION

	ret = fxml_vffromBuffer(buffer, len);
	ret->rwBuffer = buffer;
	
	return ret;
}


fxml_vFile* fxml_vfnew(long size)
{
	fxml_vFile* ret;
	FXML_PRINTFUNCTION
	ret = (fxml_vFile*)malloc(sizeof(fxml_vFile));
	if(ret){
		ret->rwBuffer = (unsigned char*)malloc(size);
		if(ret->rwBuffer){
			ret->buffer = ret->rwBuffer;
			ret->len = size;
			ret->pos = 0;

			return ret;
		}else{
			fxml_ePrint(FXML_MSG_ERROR, "Couldn't allocate file buffer");
			free(ret);
		}
	}else{
		fxml_ePrint(FXML_MSG_ERROR, "Couldn't allocate memory fore parse buffer container");
	}
	return NULL;
}

/* Get the length of a REAL open file */
long fxml_getFileLen(FILE* file)
{
	long tmp, ret;
	FXML_PRINTFUNCTION

	tmp = ftell(file);
	fseek(file, 0, SEEK_END);
	
	#ifdef FXML_DEBUG
	fxml_ePrint(FXML_MSG_DEBUG, "File len is %li", ftell(file));
	#endif

	ret = ftell(file);
	fseek(file, tmp, SEEK_SET);

	return ret;
}

/* Read a file from disk into the buffer */
fxml_vFile* fxml_vfopen(const char* fname)
{
	fxml_vFile* ret;
	long i, len;
	FILE* file = fopen(fname, "r");
	unsigned char* buffer;
	
	FXML_PRINTFUNCTION

	if(!file){
		return NULL;
	}

	len = fxml_getFileLen(file);
	
	buffer = (unsigned char*)malloc(len);

	if(!buffer){
		fxml_ePrint(FXML_MSG_ERROR, "Allocate memory for file.", fname);
		return NULL;
	}

	for(i = 0; i < len; i++){
		int tmp;
		if((tmp = fgetc(file)) == EOF){
			fxml_ePrint(FXML_MSG_ERROR, "Error reading from file");
			fclose(file);
			return NULL;
		}else{
			buffer[i] = tmp;
		}
	}

	fclose(file);
	
	ret = fxml_vffromBufferRw(buffer, len);

	#ifdef FXML_DEBUG
	fxml_vfprint(ret);
	#endif

	return ret;
}

int fxml_vfread(void* buffer, long size, long num, fxml_vFile* file)
{
	int count, i, i2;
	FXML_PRINTFUNCTION
	count = 0;

	for(i = 0; i < num; i++){
		for(i2 = 0; i2 < size; i2++){
			((unsigned char*)buffer)[count++] = fxml_vfgetc(file);
		}
	}

	return count;
}

/* Close the buffer */
void fxml_vfclose(fxml_vFile* file)
{
	FXML_PRINTFUNCTION
	if(file){
		if(file->rwBuffer){
			free(file->rwBuffer);
		}
		free(file);
	}
}

