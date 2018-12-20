/*************************|事前准备|*************************************/
#include <U8glib.h>
#include <Wire.h>
#define LightPin 2
#define INTERVAL_LCD             20             //定义OLED刷新时间间隔  
unsigned long lcd_time = millis();                 //OLED刷新时间计时器
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);     //设置OLED型号
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_fixed_v0r)
#define setFont_SS u8g.setFont(u8g_font_fub25n)
/************************************************************************/
int number = 0;
int i=10;
bool st1 = 0;
bool sta = 0;
long  time;
void setup() {
  Serial.begin(115200);               //串口初始化
  
//  attachInterrupt(0,Count_begin,FALLING);
 pinMode(LightPin,INPUT);

}
void display()//控制OLED显示器
{
  u8g.firstPage();
  do {
    setFont_L;
    u8g.drawStr( 20, 25, "It has passed");//显示文字
    setFont_S;

    u8g.setPrintPos(60, 40);
    u8g.print(number);
  } while (u8g.nextPage());
}
//
//void Count_begin()
//{
//  
//  detachInterrupt(0);
//  attachInterrupt(0,Count_end,RISING);
//}
//void Count_end()
//{
//  number++;
//  display();//显示在oled上
//  sta=1;
//  detachInterrupt(0);
//  attachInterrupt(0,Count_begin,FALLING);
//}



void loop() {
  while(i<100){
  u8g.firstPage();

  do{

     draw();

  }while(u8g.nextPage());
  i++;
  }
  int temp = digitalRead(LightPin);
    delay(100);
    if (temp == 1)
    {
      if (sta == 0)
      {
        number += 1;
        display();//显示在oled上
        sta = 1;
        st1=1;
        Wire.beginTransmission(8); // 发送数据到设备号为8的从机
    Wire.write(number);              //发送1个字节
    Wire.endTransmission();    // 停止发送
      }
    }
    else {
      sta = 0;
    

    }
  }


void draw(){

    u8g.drawRFrame(15,20, 100,20, 1);
    
    u8g.drawRBox(15 ,20 ,i ,20 ,4);
    }



