/*
   lewei50 open platform sensor client
   This code is in the public domain.
  */
 
#include <LeweiClient.h>
#include <SPI.h>
#include <Ethernet.h>
 
#define LW_USERKEY "8def5ca5c8bb4fa88b30e06ae810cffe"
#define LW_GATEWAY "01"
 
//delay between updates
#define POST_INTERVAL (5*1000)
 
IPAddress ip(192,168,1, 10);
IPAddress mydns(192,168,1,1);
IPAddress gw(192,168,1,1);
IPAddress subnet(255,255,255,0);
 
LeWeiClient *lwc;
 
void setup() {
   // start serial port:
   Serial.begin(9600);
   // hope no exception here
   //lwc = new LeWeiClient(LW_USERKEY, LW_GATEWAY);
   lwc = new LeWeiClient(LW_USERKEY, LW_GATEWAY,ip,mydns,gw,subnet);
 }
 
void loop() {
      lwc->append("pm25", (int)25);
    lwc->append("pm100", (int)100);

    lwc->send();
     delay(POST_INTERVAL);
  //while (Serial.available()) 
  //{
  //  CopeSerialData(Serial.read()); 
  //}
}

char CopeSerialData(unsigned char ucData)
{
  static unsigned char ucRxBuffer[250];
  static unsigned char ucRxCnt = 0;
  long  pmcf10=0;
  long  pmcf25=0;
  long  pmcf100=0;
  long  pmat10=0;
  long  pmat25=0;
  long  pmat100=0;
  long  pmcount03=0;
  long  pmcount05=0;
  long  pmcount10=0;
  long  pmcount25=0;
  long  pmcount50=0;
  long  pmcount100=0;
  
  ucRxBuffer[ucRxCnt++]=ucData;

  if (ucRxBuffer[0]!=0x42&&ucRxBuffer[1]!=0x4D) 
  {
    ucRxCnt=0;
    return ucRxCnt;
  }
  
  if (ucRxCnt<32) {return ucRxCnt;}
  else
  {
    pmcf10=(float)ucRxBuffer[4]*256+(float)ucRxBuffer[5];
    pmcf25=(float)ucRxBuffer[6]*256+(float)ucRxBuffer[7];
    pmcf100=(float)ucRxBuffer[8]*256+(float)ucRxBuffer[9];
    pmat10=(float)ucRxBuffer[10]*256+(float)ucRxBuffer[11]; 
    pmat25=(float)ucRxBuffer[12]*256+(float)ucRxBuffer[13]; 
    pmat100=(float)ucRxBuffer[14]*256+(float)ucRxBuffer[15]; 
    pmcount03=(float)ucRxBuffer[16]*256+(float)ucRxBuffer[17];
    pmcount05=(float)ucRxBuffer[18]*256+(float)ucRxBuffer[19];
    pmcount10=(float)ucRxBuffer[20]*256+(float)ucRxBuffer[21];
    pmcount25=(float)ucRxBuffer[22]*256+(float)ucRxBuffer[23];
    pmcount50=(float)ucRxBuffer[24]*256+(float)ucRxBuffer[25];
    pmcount100=(float)ucRxBuffer[26]*256+(float)ucRxBuffer[27];
  }
  if (lwc) {
    //t1,t2.. must using the same name setting on web server.
    lwc->append("pm25", (int)pmat25);
    lwc->append("pm100", (int)pmat100);

    lwc->send();
    delay(POST_INTERVAL);
   }
 }
