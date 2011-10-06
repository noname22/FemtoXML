#include <femtoxml.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#ifndef timersub
#define timersub(a, b, result)                  \
   do {                             \
      (result)->tv_sec = (a)->tv_sec - (b)->tv_sec;     \
      (result)->tv_usec = (a)->tv_usec - (b)->tv_usec;      \
      if ((result)->tv_usec < 0) {              \
     --(result)->tv_sec;                    \
     (result)->tv_usec += 1000000;              \
      }                             \
   } while (0)
#endif

int main(int argc, char** argv)
{
/*	fxml_document* document;
	unsigned char tmp[] = "<xml>test<test></test><monkey ass=\"2\"/></xml>";

	document = fxml_loadBuffer(tmp, (long)strlen((char*)tmp));
	
	fxml_print(document->elements);

	return 0;*/

	if(argc == 2 || argc == 3){
		struct timeval tv, tv2, res;
		fxml_document* document;

		gettimeofday(&tv, NULL); 

		/*if(argc != 3){
			fxml_setCallback(NULL, NULL);
		}*/

		document = fxml_load(argv[1]);

		gettimeofday(&tv2, NULL); 

		if(document){
			timersub(&tv2, &tv, &res);
						
			printf("load took %f seconds\n", (double)res.tv_sec + (double)res.tv_usec / 1000000);
			if(argc == 3){
				fxml_print(document->elements);
			}
		}else{
			printf("Load failed.\n");
		}

	}else{
		printf("NO MEANS NO!\n");
	}
	return 0;
}
