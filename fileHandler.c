#include "utils.h"


void fileWrite(const char *fileName, char * format,...){
   va_list arg;

   va_start (arg, format);
   vfprintf (stdout, format, arg);
   /*done = vfprintf (stdout, format, arg);*/
   va_end (arg); 
}
void deleteFile(const char *fileName){
}
 