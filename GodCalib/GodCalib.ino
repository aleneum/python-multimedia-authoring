

#include <Servo.h>

#define MOUTH A1
#define EYELID A2
#define LEFT_EYE A3
#define RIGHT_EYE A4

#define COOLDOWN_MAX 5000

int current = MOUTH;

Servo myservo;

int pos = 90;  
int incoming = 0;
int cooldown = 0;
bool isAttached = false;

void setup() {
  Serial.begin(9600);
  myservo.attach(current);  // attaches the servo on pin 9 to the servo object
  isAttached = true;
}

void loop() {
 if (Serial.available() > 0) {
  if (! isAttached) {
    myservo.attach(current);
    isAttached = true;
  }
  incoming = Serial.read();
  if (incoming == 43) { // +
     pos += 1;
     myservo.write(pos);
     Serial.println(pos);
  } else if (incoming == 45) { // -
     pos -= 1;
     myservo.write(pos);
     Serial.println(pos);
  }
  delay(100);
  cooldown = 0;
 } else if (isAttached) {
  cooldown += 10;
  if (cooldown > COOLDOWN_MAX) {
    myservo.detach();
    isAttached = false;
    delay(10);
  }
 }
}

