#include "utils.h"

void printErr(char * format,...){
    va_list arg;    

    va_start (arg, format);
    vfprintf (stdout, format, arg);
    /*done = vfprintf (stdout, format, arg);*/
    va_end (arg); 
}