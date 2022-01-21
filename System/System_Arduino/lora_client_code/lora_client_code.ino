#include <SPI.h>
#include <RH_RF95.h>
#include <DHT11.h>

#define RFM95_CS      10
#define RFM95_RST     7
#define RFM95_INT     2
#define node_id       "B"

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

//------------------------------------------------------
#define DH11pin     9
DHT11 dht11(DH11pin);
//------------------------------------------------------

void setup() 
{
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
  
  Serial.println("Sending to rf95_server");
  // Send a message to rf95_server

  String radiopacket = humiStr; //"Hello World #";
  radiopacket += tempStr; //String(packetnum++);
  //radiopacket += humiStr; //" from ";
  //radiopacket += tempStr; //node_id;

  Serial.print("Sending "); 
  Serial.println(radiopacket); 
  delay(10);
  rf95.send((uint8_t*)radiopacket.c_str(), radiopacket.length()+1);

  Serial.println("Waiting for packet to complete..."); 
  delay(10);
  rf95.waitPacketSent();
  
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  Serial.println("Waiting for reply..."); 
  delay(10);
  
  if (rf95.waitAvailableTimeout(1000))
  { 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))
   {
      Serial.print("Got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);    
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
