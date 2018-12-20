/*************************|事前准备|*************************************/
#define INTERVAL_NET      10000                 //定义上传时间间隔

unsigned long net_time1 = millis();
unsigned long LastTime = millis();

/************************************************************************/
int number = 0;
int speed = 0;
long  time;
#include <Wire.h>                                  //调用库  
#include "ESP8266.h"

#define SSID           "wqqWIFI"                   // WiFi名称（不能超过32字节）
#define PASSWORD       "12345678"

#define IDLE_TIMEOUT_MS  20000      // 链接等待时间（超时时间）
/*********************|WiFi链接部分|****************************/
#define HOST_NAME   "api.heclouds.com"

// 修改成自己的ID
#define DEVICEID   "505412621"
#define PROJECTID "3GaVgWzcver5QzRo"
#define HOST_PORT   (80)

// 修改成自己在OneNet上面的apikey
// 大概长这样: VeFI0HZ44Qn5dZO14AuLbWSlSlI=
String apiKey = "1S5FqZzWNK6WuFBJZ8Xl0YOMBho=";
char buf[10];
/**************************************************************/

String mCottenData;
String jsonToSend;
/**************************|上传部分定义时间|**************************/
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3); /* RX:D3, TX:D2 */
ESP8266 wifi(mySerial);
//ESP8266 wifi(Serial1);                                      //定义一个ESP8266（wifi）的对象
unsigned long sensor_time = millis();                        //传感器采样时间计时器
/**********************************************************************/
//int SensorData;                                   //用于存储传感器数据
String postString;                                //用于存储发送数据的字符串
//String jsonToSend;                                //用于存储发送的json格式参数
void setup() {
  /**************|超级超级长的WiFi初始化阶段|*******************/

  Serial.begin(115200);
  //ESP8266初始化

  Serial.print("setup begin\r\n");

  Serial.print("FW Version:");
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print("to station + softap ok\r\n");
  } else {
    Serial.print("to station + softap err\r\n");
  }

  if (wifi.joinAP(SSID, PASSWORD)) {      //加入无线网
    Serial.print("Join AP success\r\n");
    Serial.print("IP: ");
    Serial.println(wifi.getLocalIP().c_str());
  } else {
    Serial.print("Join AP failure\r\n");
  }

  if (wifi.disableMUX()) {
    Serial.print("single ok\r\n");
  } else {
    Serial.print("single err\r\n");
  }

  Serial.print("setup end\r\n");
  Wire.begin(8);                // 加入i2c总线，设置从机地址为#8
  Wire.onReceive(receiveEvent); // 注册接受到主机字符的事件

}

void loop() {

  
  speed=number*60000/(millis()-LastTime);

    Serial.print("number  ");
  Serial.println(number);
  Serial.print("speed  ");
  Serial.println(speed);
  delay( INTERVAL_NET);      
    updateSensorData();
    
   
    Serial.print("sending...\n");



  delay(200);          



}
void receiveEvent(int howMany) {
  if ( Wire.available())
  {
    number = Wire.read();
    Serial.println(number);         // 把整数打印到串口监视器中，并回车

  }

}
void updateSensorData() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
    Serial.print("create tcp ok\r\n");

    jsonToSend = "{\"Number\":";
    itoa(number, buf, 10);
    jsonToSend += "\"" + String(buf) + "\"";
    jsonToSend += "}";



    postString = "POST /devices/";
    postString += DEVICEID;
    postString += "/datapoints?type=3 HTTP/1.1";
    postString += "\r\n";
    postString += "api-key:";
    postString += apiKey;
    postString += "\r\n";
    postString += "Host:api.heclouds.com\r\n";
    postString += "Connection:close\r\n";
    postString += "Content-Length:";
    postString += jsonToSend.length();
    postString += "\r\n";
    postString += "\r\n";
    postString += jsonToSend;
    postString += "\r\n";
    postString += "\r\n";
    postString += "\r\n";

    const char *postArray = postString.c_str();                 //将str转化为char数组
    Serial.println(postArray);
    wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
    Serial.println("send success");
    if (wifi.releaseTCP()) {                                 //释放TCP连接
      Serial.print("release tcp ok\r\n");
    }
    else {
      Serial.print("release tcp err\r\n");
    }
    postArray = NULL;                                       //清空数组，等待下次传输数据

  } else {
    Serial.print("create tcp err\r\n");
  }

}
