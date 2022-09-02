#include "cat.h"

#include "TFT_eSPI.h"
#include <WiFi.h>
#include "time.h"


TFT_eSPI lcd = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&lcd);
TFT_eSprite spr = TFT_eSprite(&lcd); //sprite for seconds
TFT_eSprite spr2 = TFT_eSprite(&lcd); //sprite for calendar
TFT_eSprite cal = TFT_eSprite(&lcd);

const char* ssid     = "xxxxxxxx";
const char* password = "xxxxxxxx";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec =3600;             //time zone * 3600 , my time zone is  +1 GTM
const int   daylightOffset_sec = 3600;   


#define gray 0x6B6D
#define blue 0x0AAD
#define orange 0xC260
#define purple 0x604D
#define green 0x1AE9

char timeHour[3];
char timeMin[3];
char timeSec[3];
char day[3];
char month[6];
char year[5];
char timeWeekDay[3];
String dayInWeek;
String IP;


int left=0;
int right=14;

void setup(void)
{    

    pinMode(left,INPUT_PULLUP);
    pinMode(right,INPUT_PULLUP);
    lcd.init();
    lcd.setRotation(1);
    sprite.createSprite(320, 170);
    sprite.setSwapBytes(true);
    sprite.setTextColor(TFT_WHITE,0xEAA9);
    sprite.setTextDatum(4);

    cal.createSprite(218,26);
    cal.fillSprite(TFT_GREEN);
    cal.setTextColor(TFT_WHITE,TFT_GREEN);

    spr.createSprite(80,40);
    spr.fillSprite(TFT_GREEN);
    spr2.createSprite(80,64);
    spr2.fillSprite(TFT_GREEN);
    spr2.setTextDatum(4);
    spr.setTextColor(TFT_WHITE,TFT_GREEN);
    spr2.setTextColor(TFT_WHITE,TFT_GREEN);
    spr.setFreeFont(&Orbitron_Light_32);
    spr2.setFreeFont(&Orbitron_Light_24);
    

   Serial.begin(9600);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi connected.");
   IP=WiFi.localIP().toString();

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

int x=0;
long startF=0;
long endF=0;
double fps=0;



void printLocalTime()
  {
  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo)){
    
    return;
  }
  
  strftime(timeHour,3, "%H", &timeinfo);
  strftime(timeMin,3, "%M", &timeinfo);
  strftime(timeSec,3, "%S", &timeinfo);

    
   strftime(timeWeekDay,10, "%A", &timeinfo);
  dayInWeek=String(timeWeekDay);

  
  strftime(day,3, "%d", &timeinfo);
  strftime(month,6, "%B", &timeinfo);
  strftime(year,5, "%Y", &timeinfo);

  }

long t=0;
int xt=230;
int yt=8;

 uint32_t volt = (analogRead(4) * 2 * 3.3 * 1000) / 4096;

void loop()
{
  if(digitalRead(left)==0)
  xt--;
  if(digitalRead(right)==0)
  xt++;
   
  startF=millis();
  
  spr.fillSprite(TFT_GREEN);
  spr2.fillSprite(TFT_GREEN);
  spr2.setFreeFont(&Orbitron_Light_24);
  sprite.pushImage(0,0,aniWidth,aniHeigth,logo2[x]);
  sprite.setTextColor(purple,TFT_WHITE);
  sprite.fillRoundRect(xt,yt,80,26,3,TFT_WHITE);
  sprite.fillRoundRect(xt,yt+70,80,16,3,TFT_WHITE);

  
  
  
  sprite.drawString(String(month)+"/"+String(day),xt+40,yt+70+8,2);
  
  sprite.drawString(String(timeHour)+":"+String(timeMin),xt+40,yt+13,4);
  spr.drawString(String(timeSec),4,6);
  spr2.drawRoundRect(0,0,80,34,3,TFT_WHITE);
  
  spr2.drawString("FPS",62,14,2);
  spr2.drawString(String((int)fps),26,14);
  spr2.drawString("CONNECTED",40,44,2);
  spr2.setTextFont(0);
  spr2.drawString(IP,40,60);

  cal.drawRoundRect(0,0,217,26,3,TFT_WHITE);
  cal.drawString("LilyGO T-DISPLAY S3",8,4,2);
 

  //cal.drawString(dayInWeek,20,30);

  cal.pushToSprite(&sprite,xt-224,yt,TFT_GREEN);
  spr.pushToSprite(&sprite,xt+4,yt+22,TFT_GREEN);
  spr2.pushToSprite(&sprite,xt,yt+70+16+6,TFT_GREEN);
  sprite.pushSprite(0,0);
  //delay(20);

  if(t+1000<millis()){
  printLocalTime();
  t=millis();
  }

  x++;
  if(x==framesNumber)
  x=0;
  
  endF=millis();
  fps=1000/(endF-startF);
}

  
