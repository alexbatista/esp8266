   #include "mbed.h"
   #include <string.h>
   #define T_PC 0
   #define T_ESP 1
   Serial pc(USBTX, USBRX);
   Serial esp(A0,A1);
   InterruptIn bt(USER_BUTTON);
   enum _STATE{
     RST,
     WIFIMODE,
     CONNECT,
     GETIP,
     CONNECTED,
     OPENTCP,
     SENDTCP,
     SENDDATATCP,
     CLOSETCP
   };
   char host [] = "api.thingspeak.com";
   char server[] = "GET /update?api_key=06R4I3BAROD02GKV&field1=30 HTTP/1.0";
  //  char server[] = "GET /update?api_key=06R4I3BAROD02GKV&field1=30 HTTP/1.1\r\nHost: api.thingspeak.com";
   char ssid[32] = "IC";     // enter WiFi router ssid inside the quotes
   char pwd [32] = "icomputacaoufal"; // enter WiFi router password inside the quotes

   char port [] = "80";
   char tcp [] = "TCP";

   int count = 0, ended, timeout = 2;
   char buf[1024] = {0};
   char snd[255];
   char * m;
   int flag;
   int op;
   int rsp;
   void send(char* command, int target){
     if(target == 1){
       //pc.printf("%s\r\n",command);
       esp.printf("%s\r\n",command);
     }else if(target == 0){
       pc.printf("%s\r\n",command);
     }
     //sleep
   }
   void rxHandler();

   void Reset(){
       // Mexer no whatchdog
       char str[1024];
       sprintf(str,"AT+RST");
       //pc.printf("STR = %s\r\n",str);
       send(str,T_ESP); //set wifi-mode client

   }

   void TypeSingleConnection(){
     char str[1024];
     sprintf(str,"AT+CIPMUX=0");
     send(str,T_ESP);
   }

   void WiFiMode(){
     char str[1024];
     sprintf(str,"AT+CWMODE=1");
     send(str,T_ESP);
     TypeSingleConnection();
   }

   void Connect(){
     char str[1024];
     sprintf(str,"AT+CWJAP=\"%s\",\"%s\"",ssid,pwd);
     send(str,T_ESP);

   }
   void GetIP(){
     char str[1024];
     sprintf(str,"AT+CIFSR");
     send(str,T_ESP);
   }


   void OpenTCP(){
     char str[1024];
     sprintf(str,"AT+CIPSTART=\"%s\",\"%s\",%s",tcp,host,port);
     send(str,T_ESP);

   }
   void SendTCP(){
       char str[1024];
       sprintf(str,"AT+CIPSEND=%d",strlen(server)+4);
       send(str,T_ESP); //set wifi-mode client
   }
   void SendDataTCP(){
     //pegar leitura do  analogico
     char str[1024];
     sprintf(str,"%s\r\n",server);
     send(str,T_ESP);
   }

   void CloseTCP(){
     char str[1024];
     sprintf(str,"AT+CIPCLOSE");
     send(str,T_ESP);
   }
void opca();
void IQRButton();
   _STATE STATUS = RST;
   int main(){
     esp.baud(115200);
     pc.baud(115200);

     esp.attach(rxHandler,Serial::RxIrq);
     bt.rise(&IQRButton);
     pc.printf("Initi RST\n");
     wait(1);
     //Reset();
     esp.printf("AT\r\n");
     STATUS = RST;
     wait(3);
/*
     WiFiMode();

     wait(3);
     pc.printf("_______________");
     pc.printf("\nBUF= %s\n",buf);
     pc.printf("OP = %d\n",op);
     pc.printf("STATUS = %d\n",STATUS);
     memset(buf, 0, sizeof(buf));
     count=0;

     Connect();
     wait(10);
     pc.printf("_______________");
     pc.printf("\nBUF= %s\n",buf);
     pc.printf("OP = %d\n",op);
     pc.printf("STATUS = %d\n",STATUS);
     memset(buf, 0, sizeof(buf));
     count=0;
     OpenTCP();
     wait(10);
     pc.printf("_______________");
     pc.printf("\nBUF= %s\n",buf);
     pc.printf("OP = %d\n",op);
     pc.printf("STATUS = %d\n",STATUS);
     memset(buf, 0, sizeof(buf));
     count=0;
     SendTCP();
     wait(10);
     pc.printf("_______________");
     pc.printf("\nBUF= %s\n",buf);
     pc.printf("OP = %d\n",op);
     pc.printf("STATUS = %d\n",STATUS);
     memset(buf, 0, sizeof(buf));
     count=0;
     SendDataTCP();
     wait(10);
     pc.printf("_______________");
     pc.printf("\nBUF= %s\n",buf);
     pc.printf("OP = %d\n",op);
     pc.printf("STATUS = %d\n",STATUS);
     memset(buf, 0, sizeof(buf));
     count=0;
     //opca();
     */
     while(1){
       //sleep();
       pc.printf("_______________");
       pc.printf("\nBUF= %s\n",buf);
       pc.printf("RSP = %d\n",rsp);
       pc.printf("STATUS = %d\n",STATUS);
       memset(buf, 0, sizeof(buf));
       count=0;
       wait(5);
       opca();
     }


   }

   void rxHandler(){
   // 0 -ok, 1 - error, 2 - `>` send return
   //rsp
     while(esp.readable()) {
          buf[count++] = esp.getc();
     }

    m = buf + strlen(buf) - 4;

    flag = 0;
    op=3;
    rsp = 1;

     for(int i =0; i < count;i++ ){
       if (buf[i] == '>'){
         op = 2;
         rsp = 2;
         flag =1;
         break;
       }
     }
     if(flag == 0){
         for(int i =0; i < count-1;i++ ){
           if ( buf[i] == 'O'){
             if(buf[i+1] == 'K'){
             op = 0;
             rsp = 0;
             break;
           }
         }
       }
    }
   }
void opca(){
  switch (STATUS) {
    case RST:
        if(rsp == 0){
          //op=99;
          WiFiMode();
          STATUS = WIFIMODE;
        }
        else{
          //Reset();
          STATUS = RST;
        }
        break;\

    case WIFIMODE:
        if(rsp == 0){
          Connect();
          STATUS = CONNECT;
        }
        else{
          Reset();
          STATUS = RST;
        }
        break;

    case CONNECT:
        if(rsp == 0){
          GetIP();
          STATUS = GETIP;
        }
        else{
          Connect();
          STATUS = CONNECT;
        }
        break;

    case GETIP:
        if(rsp == 0){
          //GetIP();
          STATUS = CONNECTED;
        }
        else{
          //.Reset();
          //_STATE = RST;
        }
        break;

    case CONNECTED:

        break;

    case OPENTCP:
        if(rsp == 0){
          SendTCP();
          STATUS = SENDTCP;
        }
        else{
          OpenTCP();
          STATUS = OPENTCP;
        }
        break;

    case SENDTCP:
        if(rsp == 2){ // >
          SendDataTCP();
          STATUS = SENDDATATCP;
        }
        else if(rsp == 1){ // error
          SendTCP();
          STATUS = SENDTCP;
        }
          break;

    case SENDDATATCP:
        if(rsp == 0){ // >
          //SendDataTCP();
          CloseTCP();
          STATUS = CONNECTED;
        }
        else { // error
          SendDataTCP();
          STATUS = SENDDATATCP;
        }
        break;

    case CLOSETCP:
        if(rsp == 0){ // >
          //SendDataTCP();
          STATUS = CONNECTED;
        }
        else { // error
          //Reset();
          //STATUS = RST;
        }
        break;
    }
}

   void IQRButton(){
     //get analogico value
     // atualizar
     OpenTCP();
     STATUS = OPENTCP;

   }
