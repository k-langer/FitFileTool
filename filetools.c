#define _CRT_SECURE_NO_WARNINGS

#include "filetools.h"

static int power_val = 0; 
static int power_order;
static double power_fit[5];

void WriteFileHeader(FILE *fp)
{
   FIT_FILE_HDR file_header;

   file_header.header_size = FIT_FILE_HDR_SIZE;
   file_header.profile_version = FIT_PROFILE_VERSION;
   file_header.protocol_version = FIT_PROTOCOL_VERSION_20;
   memcpy((FIT_UINT8 *)&file_header.data_type, ".FIT", 4);
   fseek (fp , 0 , SEEK_END);
   file_header.data_size = ftell(fp) - FIT_FILE_HDR_SIZE - sizeof(FIT_UINT16);
   file_header.crc = FitCRC_Calc16(&file_header, FIT_STRUCT_OFFSET(crc, FIT_FILE_HDR));

   fseek (fp , 0 , SEEK_SET);
   fwrite((void *)&file_header, 1, FIT_FILE_HDR_SIZE, fp);
}
void WriteData(const void *data, FIT_UINT8 data_size, FILE *fp)
{
   FIT_UINT8 offset;
   fwrite(data, 1, data_size, fp);

   for (offset = 0; offset < data_size; offset++)
      data_crc = FitCRC_Get16(data_crc, *((FIT_UINT8 *)data + offset));
}
void WriteMessage(FIT_UINT8 local_mesg_number, const void *mesg_pointer, FIT_UINT8 mesg_size, FILE *fp)
{
   WriteData(&local_mesg_number, FIT_HDR_SIZE, fp);
   WriteData(mesg_pointer, mesg_size, fp);
}
void WriteMessageDefinition(FIT_UINT8 local_mesg_number, const void *mesg_def_pointer, FIT_UINT8 mesg_def_size, FILE *fp)
{
   FIT_UINT8 header = local_mesg_number | FIT_HDR_TYPE_DEF_BIT;
   WriteData(&header, FIT_HDR_SIZE, fp);
   WriteData(mesg_def_pointer, mesg_def_size, fp);
}
int done(FIT_CONVERT_RETURN convert_return, FILE *file) {
   if (convert_return == FIT_CONVERT_ERROR)
   {   
      printf("Error decoding file.\n");
      fclose(file);
      return 1;
   }   
   if (convert_return == FIT_CONVERT_CONTINUE)
   {   
      printf("Unexpected end of file.\n");
      fclose(file);
      return 1;
   }   
   if (convert_return == FIT_CONVERT_DATA_TYPE_NOT_SUPPORTED)
   {   
      printf("File is not FIT.\n");
      fclose(file);
      return 1;
   }   
   if (convert_return == FIT_CONVERT_PROTOCOL_VERSION_NOT_SUPPORTED)
   {   
      printf("Protocol version not supported.\n");
      fclose(file);
      return 1;
   }   
   if (convert_return == FIT_CONVERT_END_OF_FILE)
      printf("File converted successfully.\n");

   fclose(file);
   return 0; 
}
void powerCfg(char* file) {
    FILE *input_file = fopen(file, "r");
    if( input_file == NULL ){ return; }
    power_val = 1;  
    char buffer[ 80 ]; 
    char *tok;
    while( fgets(buffer, 80, input_file) != NULL ){
        tok = strtok( buffer, " " );
        while( tok != NULL ){
            if (power_order >4) { break; }
            sscanf(tok, "%lf", &power_fit[power_order]); 
            power_order++; 
            tok = strtok( NULL, " ");
        }
    }
}
double powerEst(double speed) {
    double power = 0; 
    // speed is in MPH
    if (power_val == 1) {
    for (int i = 0; i < power_order; i++ ) 
        power += power_fit[power_order]*pow(speed,power_order-i); 
    }
    else {
         power = 0.0191*(speed*speed*speed)+0.0673*speed*speed + 4.2639*speed;  
    }
    return power;
}
