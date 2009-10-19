/* FemtoXML - An easy to use XML library
 * 
 * See doc/license.txt for license information (GPLv3)
 * 
 * Authors: 
 * 	Fredrik Hultin, noname@ the domain two lines below, 2007
 *
 * http://nurd.se/~noname/femtoxml
 */

/* message.c
 *
 * This file conatins FemtoXMLs functions for sending messages to the application using it.
 * The messages can be anything from information to error messages */

#include "femtoxmli.h"

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

/* Default message callback, which is called whenever femtoxml sends a message. */
void fxml_messageCallback(fxml_msg* msg, void* data)
{
	printf("FXML %s (%d): %s\n", msg->typeName, msg->type, msg->msg);
}

/* Converts an error code to a string representing the value */
char* fxml_getErrorLevel(unsigned int level)
{
	static char levelNames[4][10] = {"Error", "Warning", "Info", "Debug"};
	return levelNames[level % 4];
}

/* Contains the pointer to the message callback. Used internally by fxml_setCallback and fxml_getCallback */
fxml_callbackSet* fxml_setGetCallback(fxml_callback_fn newCb, void* inData, int set)
{
	static fxml_callbackSet cbs;
	static int notFirst;

	if(!notFirst){
		notFirst = 1;
		cbs.data = NULL;
		cbs.cb = fxml_messageCallback;
	}

	if(set){
		cbs.data = inData;
		cbs.cb = newCb;
	}

	return &cbs;
}

/* Returns a pointer to the message callback function and the void*-data sent along with it */
fxml_callbackSet* fxml_getCallback()
{
	return fxml_setGetCallback(NULL, NULL, 0);
}

/* Sets a custom message callback. void* data is passed along to the callback when it's called */
void fxml_setCallback(fxml_callback_fn newCb, void* data)
{
	fxml_setGetCallback(newCb, data, 1);
}

/* Sends a given string to the message callback */
void fxml_sendMsgStr(int type, const char* typeName, const char* msg)
{
	fxml_msg myMsg;

	myMsg.type = type;
	myMsg.typeName = typeName;
	myMsg.msg = msg;


	if(fxml_getCallback()->cb != NULL){
		fxml_getCallback()->cb(&myMsg, fxml_getCallback()->data);
	}
}

/* Takes pintf-style arguments, converts them to a string and sends it to the message callback using fxml_sendMsgStr */
void fxml_ePrint(int type, const char *format, ...)
{
	va_list fmtargs;
	char buffer[FXML_MAX_MESSAGELEN];
	
	buffer[FXML_MAX_MESSAGELEN - 1] = '\0';

	va_start(fmtargs, format);
	
	#if __STDC_VERSION__ == 199901L | HAVE_VSNPRINTF
	/* If we're in C99 mode we can use the vsnprintf, which guarantees that the buffer won't be overrun */
	vsnprintf(buffer, FXML_MAX_MESSAGELEN - 1, format, fmtargs);
	#else
	/* In C89 mode we simple have to hope that the format doesn't expand the string beyond the size of the buffer */
	#warning vsnprintf unavailable, using UNSAFE vsprintf

	vsprintf(buffer, format, fmtargs);
	#endif

	va_end(fmtargs);
	
	fxml_sendMsgStr(type, fxml_getErrorLevel(type), buffer);
}

