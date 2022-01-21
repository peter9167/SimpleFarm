#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 7
#define RFM95_INT 2

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

//-------------------[_SETUP & INITIALIZE_]----------------------//
void setup() 
{   
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("");
    while (1);
  }
  Serial.println("");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("");
    while (1);
  }
  Serial.print(""); 
  Serial.println(RF95_FREQ);

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  
  rf95.setTxPower(23, false);
}

//-------------------[_LOOP & PROCESSING_]----------------------//
void loop()
{
  if (rf95.available())
  { 
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len))
    {
      //RH_RF95::printBuffer("Received: ", buf, len);
      //Serial.print("Got: ");
      //Serial.println((char*)buf);
      //Serial.print("RSSI: ");
      //Serial.println(rf95.lastRssi(), DEC);

      // Send a reply --> 클라이언트로 데이터 수신에 대한 확답을 보낸다.
      uint8_t data[] = "And hello back to you";
      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      //Serial.println("Sent a reply");

//--------------------------------------------------------
      // 받은 데이터 처리 부분
      // buf에 있는 아스키값을 float형으로 변환하여 humi, temp변수에 저장한다.
      
      uint16_t humiBuf;
      uint16_t tempBuf;
      float humi;
      float temp;

      humiBuf = (buf[0] - 0x30) * 1000;
      humiBuf += (buf[1] - 0x30) * 100;
      humiBuf += (buf[3] - 0x30) * 10;
      humiBuf += (buf[4] - 0x30);

      tempBuf = (buf[0+5] - 0x30) * 1000;
      tempBuf += (buf[1+5] - 0x30) * 100;
      tempBuf += (buf[3+5] - 0x30) * 10;
      tempBuf += (buf[4+5] - 0x30);

      humi = (float)(humiBuf / 100);
      temp = (float)(tempBuf / 100);

      Serial.println(humi);
      Serial.println(temp);
      
    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}
