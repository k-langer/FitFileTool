#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "string.h"
#include "math.h"

#include "fit_convert.h"
#include "fit_product.h"
#include "fit_example.h"
#include "fit_crc.h"
#include "filetools.h"

int main(int argc, char* argv[])
{
   FILE *file, *fp;
   FIT_UINT8 buf[8];
   FIT_CONVERT_RETURN convert_return = FIT_CONVERT_CONTINUE;
   FIT_UINT32 buf_size;
   int timeOffset = 0; 
   double speedMph; 
   double powerEstm; 
  
   powerCfg("pwr.cfg");
    
   data_crc = 0;
   fp = fopen("PowerAdded.fit", "w+b");
   WriteFileHeader(fp);

   char * fitInput; 

   #ifdef WINDOWS
        char buffer[80]; 
        fitInput = getFileName("pwr.cfg",buffer);
        printf("Converting fit file: %s\n",fitInput);
   #else
   fitInput = argv[1]; 
   if (argc < 2)
   {
      printf("usage: decode.exe <fit file>\n");
   }
   #endif
   printf("Testing file conversion using %s file...\n", fitInput);
   FitConvert_Init(FIT_TRUE);

   if((file = fopen(fitInput, "rb")) == NULL)
   {
      printf("Error opening file %s.\n", fitInput);
      #ifdef WINDOWS 
      getchar();
      #endif
      return FIT_FALSE;
   } 
   while(!feof(file) && (convert_return == FIT_CONVERT_CONTINUE))
   {
      for(buf_size=0;(buf_size < sizeof(buf)) && !feof(file); buf_size++)
      {
         buf[buf_size] = (FIT_UINT8)getc(file);
      }
      do
      {
         convert_return = FitConvert_Read(buf, buf_size);
         switch (convert_return)
         {
            case FIT_CONVERT_MESSAGE_AVAILABLE:
            {
	  	        const FIT_UINT8 *mesg = FitConvert_GetMessageData();
	            FIT_UINT16 mesg_num = FitConvert_GetMessageNumber();
               //printf("Mesg %d %d- \n", mesg_num);
               switch(mesg_num)
               {
                  case FIT_MESG_NUM_FILE_ID: 
                     break;
                  case FIT_MESG_NUM_USER_PROFILE:
                     break;
                  case FIT_MESG_NUM_ACTIVITY:
                     break;
                  case FIT_MESG_NUM_SESSION:
                     break;
                  case FIT_MESG_NUM_LAP:
                     break;
                  case FIT_MESG_NUM_RECORD:
                  {
                     FIT_RECORD_MESG *record = (FIT_RECORD_MESG *) mesg;
		             if(timeOffset == 0 ) { timeOffset = record->timestamp; }	
		             speedMph = record->speed*0.00223694; 
                     powerEstm = powerEst(speedMph);
                     //powerEstm = 0.0191*(speedMph*speedMph*speedMph)+0.0673*speedMph*speedMph + 4.2639*speedMph; 
                    #ifdef DEBUG
        		     printf("%d, %d, %.2f, %.2f",
            		 	record->timestamp-timeOffset,record->heart_rate, speedMph,powerEst(speedMph));
                     printf("\n");
                    #endif
                     record->power = (FIT_UINT16) powerEstm;
                     WriteMessageDefinition(mesg_num, fit_mesg_defs[FIT_MESG_RECORD], FIT_RECORD_MESG_DEF_SIZE,fp);
                     WriteMessage(mesg_num,mesg,FIT_RECORD_MESG_SIZE,fp);
                     break;
                  }
                  case FIT_MESG_NUM_EVENT:
                     break;
                  case FIT_MESG_NUM_DEVICE_INFO:
                     break;
                  default:
                    break;
               }
               break;
            }
            default:
               break;
         }
      } while (convert_return == FIT_CONVERT_MESSAGE_AVAILABLE);
   }
   
   fwrite(&data_crc, 1, sizeof(FIT_UINT16), fp);
   WriteFileHeader(fp);
   fclose(fp);
   #ifdef WINDOWS
   getchar();
   printf("All Done, Press any key to close");
   #endif
   return done(convert_return,file);
}
