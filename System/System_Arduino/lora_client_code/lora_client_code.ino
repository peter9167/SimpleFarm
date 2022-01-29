#include <SPI.h>
#include <RH_RF95.h>
#include <DHT11.h>
#include <TimerOne.h>

#define RFM95_CS      10
#define RFM95_RST     7
#define RFM95_INT     2
#define node_id       "B"

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

const int Pump_Pin = 3; // 사용가능한 핀 (3, 4, 5, 6, 8)
const int LED_Pin = 4;
const int Fan_Pin = 5;
int incomingByte;      // a variable to read incoming serial data into

//------------------------------------------------------
#define DH11pin     9
DHT11 dht11(DH11pin);
//------------------------------------------------------

void setup() 
{
  pinMode(Pump_Pin, OUTPUT);
  pinMode(LED_Pin, OUTPUT);
  pinMode(Fan_Pin, OUTPUT);

  Timer1.initialize(250000); //1000000μs = 1s, 1000us = 0.001s, 1000us = 1ms
  Timer1.attachInterrupt(timerIsr);
  
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("Arduino LoRa TX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); 
  Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  
  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop()
{
  int i;
  float humi, temp;

#if 0
  if((i = dht11.read(humi, temp)) == 0) {   // 온도, 습도 값을 읽어오면
    Serial.print("humidity:");              // ‘시리얼 플로터’ 사용위해 이부분 주석 필요
    Serial.println(humi);                   // 습도값 출력
    Serial.print("temperature:");           // ‘시리얼 플로터’ 사용위해 이부분 주석 필요
    Serial.println(temp);                   // 온도값 출력 
  } 
  else{ 
    Serial.print("Error:");                    
    Serial.print(i);                          
  } 
#endif

  dht11.read(humi, temp);
  
  String humiStr = String(humi);
  String tempStr = String(temp);
  
//  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server

  String radiopacket = humiStr; //"Hello World #";
  radiopacket += tempStr; //String(packetnum++);
  //radiopacket += humiStr; //" from ";
  //radiopacket += tempStr; //node_id;

  Serial.print("Sending "); 
  Serial.println(radiopacket); 
  delay(10);
  rf95.send((uint8_t*)radiopacket.c_str(), radiopacket.length()+1);

//  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95.waitPacketSent();
  
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

//  Serial.println("Waiting for reply..."); 
  delay(10);
  
  if (rf95.waitAvailableTimeout(1000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.println(buf[0]);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      // buf[0] : server에서 전달된 문자 아스키코드(ASCLL) 값을 숫자로 변경됨.
      if(buf[0] == 48){ // server에서 문자 0을 전달받아서 48로 비교
        digitalWrite(Pump_Pin, HIGH);
        Serial.println("server_send : 0 -> Pump HIGH");
      } 
      if(buf[0] == 49) { // server에서 문자 1을 전달받아서 49로 비교
        digitalWrite(Pump_Pin, LOW);
        Serial.println("server_send : 1 -> Pump LOW");
      }
      if(buf[0] == 50) { // server에서 문자 2를 전달받아서 50로 비교
        digitalWrite(LED_Pin, HIGH);
        Serial.println("server_send : 2 -> LED HIGH");
      }
      if(buf[0] == 51) { // server에서 문자 3을 전달받아서 51로 비교
        digitalWrite(LED_Pin, LOW);
        Serial.println("server_send : 3 -> LED LOW");
      }
      if(buf[0] == 52) { // server에서 문자 4를 전달받아서 52로 비교
        digitalWrite(Fan_Pin, HIGH);
        Serial.println("server_send : 4 -> Fan LOW");
      }
      if(buf[0] == 53) { // server에서 문자 5을 전달받아서 53로 비교
        digitalWrite(Fan_Pin, LOW);
        Serial.println("server_send : 5 -> Fan LOW");
      }
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
  else
  {
    Serial.println("No reply, is there a listener around?");
  }
  
  delay(10000); 
}

void timerIsr(){
  
}
