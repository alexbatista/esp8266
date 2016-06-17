#include "mbed.h"
#include "TextLCD.h"
Serial pc(USBTX, USBRX);
Serial esp(A0,A1);
//DigitalOut myled(LED1);
// PwmOut PWM(LED1);
//AnalogIn Ain(A0);
// Timer t;
// TextLCD lcd(D6, D7, D8, D9, D10, D11); // rs, e, d4-d7
int count = 0, ended, timeout = 2;
char buf[1024] = {0};
char snd[255];
char * m;

char server[] = "GET /update?api_key=TGGXXSHNNX87LYX7&field1=50";
char ssid[32] = "IC";     // enter WiFi router ssid inside the quotes
char pwd [32] = "icomputacaoufal"; // enter WiFi router password inside the quotes

enum states {
  RST = 0
};

void rxHandler();

void flush(void) {
   while (esp.readable()) {
       (void)esp.getc();
   }
}
int ADCdata;


int main() {
  // lcd.cls();
  // lcd.locate(0,1);
  // lcd.printf("Hello_World\n" );
  // wait(2);
 esp.baud(115200);
 pc.baud(115200);
// int i = 0;
// wait(2);

// esp.printf("AT+CWMODE=1\r\n");
//
// esp.printf("AT+CWJAP=\"IC\",\"icomputacaoufal\"\r\n");
//
// esp.printf("AT+CIPMUX=1\r\n");
//
// esp.printf("AT+CIPSTART=0,\"TCP\",\"api.thingspeak.com\",80\r\n");

// wait(2);
// flush();
esp.attach(rxHandler,Serial::RxIrq);

while(1){
/*
   esp.printf("AT+CWMODE=3\r\n");
   wait(1);
   flush();

   esp.printf("AT+CWJAP=\"IC\",\"icomputacaoufal\"\r\n");
   wait(20);
   flush();

   esp.printf("AT+CIPMUX=1\r\n");
   wait(5);
   flush();

   esp.printf("AT+CIPSTART=0,\"TCP\",\"api.thingspeak.com\",80\r\n");
   wait(5);
   flush();

   esp.printf("AT+CIPSEND=0,%d\r\n", sizeof(server)+15);
   wait(3);
   flush();

   esp.printf("%s", server);
   esp.printf("%d", x);
   esp.printf(" HTTP/1.0\r\n\r\n\r\n\r\n\r\n");

   */
   char msg[] = "AT\r\n";
   char a[] = "AT+RST";

   //esp.printf("AT\r\n");
  //  pc.printf("%d\n",sizeof(a) );
  pc.printf("Reset ESP\n");
  esp.printf(msg);
  esp.printf(a);

  //  for (i=0;i < sizeof(msg);i++) {
  //      //if(msg[i]== '\0')
  //       //   break;
  //      esp.putc(msg[i]);
  //      //pc.putc(msg[i]);
  //  }
  //  flush();
   //PWM.period(0.01);
   //PWM = Ain;
   //ADCdata = Ain*100;
   //pc.printf("Values: %d\n",ADCdata);
  //  wait(2);
  wait(1);
 }

 return 0;
}

void rxHandler()
{
  while(esp.readable()) {
       buf[count++] = esp.getc();
      //  esp.getc();
  }
  count = 0;
  m = buf + strlen(buf) - 4;

  // char r[1024];
  // sprintf(r, "%X", buf);

  pc.printf(buf);

  if (strcmp(m,"OK\n\r") == 0){
    pc.printf("ESP has reseted!");
  }else{


  }
  // if(esp.readable())s
    // pc.printf("aaa\n");
    memset(buf, 0, sizeof(buf));
}
