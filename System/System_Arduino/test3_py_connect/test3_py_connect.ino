char cmd;

void setup() {
  
  // 시리얼 통신 시작 (boadrate: 9600)
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {

  // 컴퓨터로부터 시리얼 통신이 전송되면, 한줄씩 읽어와서 cmd 변수에 입력
  if(Serial.available()){
    cmd = Serial.read(); 

    if(cmd=='a'){
      Serial.println("아두이노: a");
      digitalWrite(13, HIGH);
      delay(100);
    }
    else if(cmd=='b'){
      Serial.println("아두이노: b");
      digitalWrite(13, LOW);
      delay(100);
    }
  }
}
