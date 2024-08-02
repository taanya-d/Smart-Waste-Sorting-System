#define GLASS_PIN A0
#define ORGANIC_PIN A1
#define METAL_PIN A2
#define CONVAIR_PIN A3
#define SERVO_PUSH_PIN 5
#define SERVO_DUMP_PIN 6
#define READY_PIN 9
#define READY_GND_PIN 8
#define echoPin1 10
#define trigPin1 11
#include <Servo.h>
Servo servoPush;
Servo servoDump;
Servo servoBin;
void setUpGpio() {
  pinMode(GLASS_PIN, INPUT_PULLUP);
  pinMode(ORGANIC_PIN, INPUT_PULLUP);
  pinMode(METAL_PIN, INPUT_PULLUP);
  pinMode(CONVAIR_PIN, OUTPUT);
  pinMode(READY_PIN, OUTPUT);
  pinMode(READY_GND_PIN, OUTPUT);
  servoPush.attach(SERVO_PUSH_PIN);
  servoBin.attach(SERVO_DUMP_PIN);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
}
float getDistance1() {
  long duration = 0;
  float distance = 0;
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration = pulseIn(echoPin1, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}
void selectBinOrganic() {
  servoBin.write(90);
}
void selectBinMetal() {
  servoBin.write(0);
}
void selectBinGlass() {
  servoBin.write(180);
}
void pushGlass() {
  servoPush.write(180);
}
void initGlass() {
  servoPush.write(5);
}
void moveConvair() {
  digitalWrite(CONVAIR_PIN, HIGH);
}
void stopConvair() {
  digitalWrite(CONVAIR_PIN, LOW);
}
uint8_t scan() {
  uint8_t state = 0;
  if (digitalRead(GLASS_PIN) == 0) {
    Serial.println("Object detected wait..");
    delay(3000);
  }
  if (digitalRead(GLASS_PIN) == 0) {
    state |= 0x1;
  }
  if (digitalRead(ORGANIC_PIN) == 0) {
    state |= 0x2;
  }
  if (digitalRead(METAL_PIN) == 0) {
    state |= 0x4;
  }
  return state;
}
void setup() {
  Serial.begin(115200);
  setUpGpio();
  initGlass();
  Serial.println("init glass");
  delay(2000);
  selectBinOrganic();
  Serial.println("bin orgainc init");
  Serial.println("ready");
}
void loop() {
  int diatance = getDistance1();
  Serial.print("diatance");
  Serial.println(diatance);
  if (diatance < 9) {
    Serial.print("<level>Full!");
    Serial.println("</level>");
  }
  if (diatance >= 9) {
    Serial.print("<level>Space Available");
    Serial.println("</level>");
  }
  digitalWrite(READY_PIN, HIGH);
  uint8_t garbage = scan();
  Serial.print("garbage: ");
  Serial.println(garbage);
  if (garbage & 0x01) {
    digitalWrite(READY_PIN, LOW);
    if (garbage & 0x02) {
      Serial.println("metal..");
      selectBinMetal();
      Serial.println("selecting metal");
      Serial.print("<cat>");
      Serial.print("Metal");
      Serial.print("</cat>");
    } else if (garbage & 0x04) {
      selectBinOrganic();
      Serial.println("selecting orgainc");
      Serial.print("<cat>");
      Serial.print("Organic");
      Serial.print("</cat>");
    } else if (garbage & 0x01) {
      selectBinGlass();
      Serial.println("selecting glass");
      Serial.print("<cat>");
      Serial.print("Glass");
      Serial.print("</cat>");
    }
    delay(2000);
    moveConvair();
    Serial.println("Convair started.");
    delay(2000);
    pushGlass();
    Serial.println("Pushing glass.");
    delay(2000);
    initGlass();
    delay(2000);
    Serial.println("glass init done.");
    delay(5000);
    stopConvair();
  }
  delay(500);
}