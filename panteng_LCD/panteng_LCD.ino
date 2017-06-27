/*
   lewei50 open platform sensor client
   This code is in the public domain.
  */

#include <SPI.h>
 
//delay between updates
#define POST_INTERVAL (5*1000)

static unsigned char ucRxBuffer[250];
static unsigned char ucRxCnt = 0;
int  pmcf10=0;
int  pmcf25=0;
int  pmcf100=0;
int  pmat10=0;
int  pmat25=0;
int  pmat100=0;
int  pmcount03=0;
int  pmcount05=0;
int  pmcount10=0;
int  pmcount25=0;
int  pmcount50=0;
int  pmcount100=0;

const float pm25Ihigh[7]= {50,100,150,200,300,400,500};
const float pm25Ilow[7] = {0,51,101,151,201,301,401};
const float pm25Chigh[7]= {34.9,74.9,114.9,149.9,249.9,349.9,499.9};
const float pm25Clow[7] = {0,35,75,115,150,250,350};

const float pm100Ihigh[7]= {50,100,150,200,300,400,500};
const float pm100Ilow[7] = {0,51,101,151,201,301,401};
const float pm100Chigh[7]= {49.9, 149.9, 249.9,349.9,419.9,499.9, 599.9};
const float pm100Clow[7] = {0,50,150,250,350,420,500};

void setup() {
   // start serial port:
   Serial.begin(9600);
   Serial1.begin(9600); //LCD
   Serial2.begin(9600); //CO2
   Serial3.begin(9600); //pm2.5
 }
 
void loop() {
  
  while (Serial1.available()) 
  {
    CopeSerialDataPM(Serial1.read()); 
  }
  
  char CO2[7]={0x42,0x4D,0xE3,0x00,0x00, 0x01,0x72};
  Serial2.write(&CO2[0],7);
  delay(500);

  //while (Serial2.available()) 
  //{
    CopeSerialDataCO2(); 
  //}
  
  Serial.println("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
  delay(POST_INTERVAL);
 }


char CopeSerialDataPM(unsigned char ucData)
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
    for (int i=0;i<32;i++)
    {
      Serial.print(ucRxBuffer[i]);
      Serial.print("  ");
    }
    
    Serial.println("");    
    
    pmcf10=(float)ucRxBuffer[4]*256+(float)ucRxBuffer[5];Serial.print("PM1.0_CF1:");Serial.print(pmcf10);Serial.print("   ");  
    pmcf25=(float)ucRxBuffer[6]*256+(float)ucRxBuffer[7];Serial.print("PM2.5_CF1:");Serial.print(pmcf25);Serial.print("   ");
    pmcf100=(float)ucRxBuffer[8]*256+(float)ucRxBuffer[9];Serial.print("PM10_CF1:");Serial.print(pmcf100);Serial.println("   ");
    pmat10=(float)ucRxBuffer[10]*256+(float)ucRxBuffer[11];  Serial.print("PM1.0_AT:");Serial.print(pmat10);Serial.print("   ");
    pmat25=(float)ucRxBuffer[12]*256+(float)ucRxBuffer[13];  Serial.print("PM2.5_AT:");Serial.print(pmat25);Serial.print("   ");
    pmat100=(float)ucRxBuffer[14]*256+(float)ucRxBuffer[15];  Serial.print("PM10_AT:");Serial.print(pmat100);Serial.println("   ");
    pmcount03=(float)ucRxBuffer[16]*256+(float)ucRxBuffer[17];  Serial.print("PMcount0.3:");Serial.print(pmcount03);Serial.print("   ");
    pmcount05=(float)ucRxBuffer[18]*256+(float)ucRxBuffer[19];  Serial.print("PMcount0.5:");Serial.print(pmcount05);Serial.print("   ");
    pmcount10=(float)ucRxBuffer[20]*256+(float)ucRxBuffer[21];  Serial.print("PMcount1.0:");Serial.print(pmcount10);Serial.println("   ");
    pmcount25=(float)ucRxBuffer[22]*256+(float)ucRxBuffer[23];  Serial.print("PMcount2.5:");Serial.print(pmcount25);Serial.print("   ");
    pmcount50=(float)ucRxBuffer[24]*256+(float)ucRxBuffer[25];  Serial.print("PMcount5.0:");Serial.print(pmcount50);Serial.print("   ");
    pmcount100=(float)ucRxBuffer[26]*256+(float)ucRxBuffer[27];  Serial.print("PMcount10:");Serial.print(pmcount100);Serial.println("   ");

    //convert to AQI
    int flag;
    int pm25_AQI;
    int pm100_AQI;
    
    if (pmat25>pm25Chigh[6])flag=7;
    else if (pmat25>pm25Chigh[5])flag=6;
    else if (pmat25>pm25Chigh[4])flag=5;
    else if (pmat25>pm25Chigh[3])flag=4;
    else if (pmat25>pm25Chigh[2])flag=3;
    else if (pmat25>pm25Chigh[1])flag=2;
    else if (pmat25>pm25Chigh[0])flag=1;
    else flag=0;
    
    pm25_AQI=(pm25Ihigh[flag]-pm25Ilow[flag])*((float)pmat25-pm25Clow[flag])/(pm25Chigh[flag]-pm25Clow[flag])+pm25Ilow[flag];

    if (pmat100>pm100Chigh[6])flag=7;
    else if (pmat100>pm100Chigh[5])flag=6;
    else if (pmat100>pm100Chigh[4])flag=5;
    else if (pmat100>pm100Chigh[3])flag=4;
    else if (pmat100>pm100Chigh[2])flag=3;
    else if (pmat100>pm100Chigh[1])flag=2;
    else if (pmat100>pm100Chigh[0])flag=1;
    else flag=0;
    
    pm100_AQI=(pm100Ihigh[flag]-pm100Ilow[flag])*((float)pmat100-pm100Clow[flag])/(pm100Chigh[flag]-pm100Clow[flag])+pm100Ilow[flag];

    Serial3.print("t1.txt=\"PM2.5 AQI:  ");
    Serial3.print(pm25_AQI);
    Serial3.print("\"");
    Serial3.write(255);
    Serial3.write(255);
    Serial3.write(255);

    Serial.print("PM2.5 AQI:");Serial.print(pm25_AQI);Serial.println("   ");
    
    Serial3.print("t2.txt=\"PM10 AQI:   ");
    Serial3.print(pm100_AQI);
    Serial3.print("\"");
    Serial3.write(255);
    Serial3.write(255);
    Serial3.write(255);

    Serial.print("PM10 AQI:");Serial.print(pm100_AQI);Serial.println("   ");

    Serial.println(" *****************************************************************  ");
    Serial.println("   ");

    ucRxCnt=0;
  }
  return ucRxCnt;
}

void CopeSerialDataCO2()
{
  static unsigned char co2_data[12];
  if(Serial2.readBytes(co2_data,12)==12)
  {
    Serial.print("CO2: ");
    
    for (int i=0;i<12;i++)
    {
      Serial.print(co2_data[i]);
      Serial.print("  ");
    }

    Serial.println("");

    long co2 = 0;
    co2 = (float)co2_data[4]*256+(float)co2_data[5];

    Serial3.print("t3.txt=\"CO2:      ");
    Serial3.print(co2);
    Serial3.print(" ppm");
    Serial3.print("\"");
    Serial3.write(255);
    Serial3.write(255);
    Serial3.write(255);
    
  }
}
