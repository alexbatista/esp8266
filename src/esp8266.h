/*
Library commands for ESP8266
*/
#include <stdio.h>
#include <stdarg.h>

/*Base AT commands*/
#define AT = "AT" //Startup test
#define RST = "AT+RST" //restart esp
#define INFO_VERSION = "AT+GMR" //See information about the version of the esp
#define ALT_ECHO = "ATE" //Alternate echo
#define ENABLE_ECHO = "ATE1" //enable echo
#define DISABLE_ECHO = "ATE0" //disable echo
#define RESTORE = "AT+RESTORE" //Restore factory settings


//variable arguments example
double mountCommand(int num,...){
  va_list valist;
   double sum = 0.0;
   int i;

   /* initialize valist for num number of arguments */
   va_start(valist, num);

   /* access all the arguments assigned to valist */
   for (i = 0; i < num; i++) {
      sum += va_arg(valist, int);
   }

   /* clean memory reserved for valist */
   va_end(valist);

   return sum/num;
}


//Initialize deep sleep function
char* deepSleep(int time){
  char command[1024];
  sprintf(command,"AT+GSLP=%d",time);
  return command;
}

//Serial Port configurations
char* configUART(unsigned int baudrate, int databits,float stopbits, unsigned int parity, unsigned int flowControl){
  char command[1024];
  sprintf(command,"AT+UART=%u,%d,%f,%u,%u",baudrate,databits,stopbits,parity,flowControl);
  return command;
}


/********
* WiFI AT CONFIGURATIONS
*********/

char* WiFiMode();
char* joinAcessPoint(){
  char command[1024];
  sprintf(command,"");
  return command;
}
char* listAcessPoint(){
  char command[1024];
  sprintf(command,"");
  return command;
}
