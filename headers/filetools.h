#if !defined(FILETOOLS_H)
#define FILETOOLS_H

#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "string.h"
#include "stdlib.h" 
#include "math.h"

#ifdef WINDOWS
//#include "windows.h"
//#include "tchar.h"
#endif

#include "fit_convert.h"
#include "fit_product.h"
#include "fit_example.h"
#include "fit_crc.h"

static FIT_UINT16 data_crc;


void WriteFileHeader(FILE *fp);
void WriteData(const void *data, FIT_UINT8 data_size, FILE *fp);
void WriteMessage(FIT_UINT8 local_mesg_number, const void *mesg_pointer, FIT_UINT8 mesg_size, FILE *fp);
void WriteMessageDefinition(FIT_UINT8 local_mesg_number, const void *mesg_def_pointer, FIT_UINT8 mesg_def_size, FILE *fp);
int done(FIT_CONVERT_RETURN convert_return, FILE *file);
void powerCfg(char* file) ;
double powerEst(double speed) ;
char*getFileName(char* file,char*buffer);
#endif
