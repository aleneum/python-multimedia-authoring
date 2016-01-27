#include <Servo.h>

#define MOUTH A1
#define EYELID A2
#define LEFT_EYE A3
#define RIGHT_EYE A4

#define RED 5
#define GREEN 3
#define BLUE 6

#define ANGRY 180
#define SAD 0
#define MID 90

#define HSV_RED 0
#define HSV_YELLOW 60
#define HSV_GREEN 120
#define HSV_CYAN 180
#define HSV_BLUE 240
#define HSV_CYAN 300

Servo servoMouth;
Servo servoEyelid;
Servo servoLeftEye;
Servo servoRightEye;

int maxLeftEye = 180;
int minLeftEye = 18;
int maxRightEye = 120;
int minRightEye = 20;
int maxMouth = 180;
int minMouth = 13;
int maxEyelid = 170;
int minEyelid = 0;

int stateMouth = 1;   // 0 HAPPY; 180 ANGRY
int stateEyelid = 1;  // 0 DOWN; 180 UP;
int stateEye = 1;       // 0 SAD; 180 ANGRY;
int stateLeftEye = 1;   // 180 ANGRY; 0 SAD;
int stateRightEye = 1;  // 0 ANGRY; 180 SAD;

unsigned char r,g,b;
int stateColor = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop(){
  set(ANGRY, ANGRY, ANGRY, HSV_RED);
  delay(10000);
}

void set(int eyes, int eyelid, int mouth, int color) {
  color = color % 360;
  int eyeOp = (eyes > stateEye) ? 1 : -1;
  int lidOp = (eyelid > stateEyelid) ? 1 : -1;
  int mouthOp = (mouth > stateMouth) ? 1 : -1;
  int colorOp = (color > stateColor) ? 1 : -1;
 
  servoRightEye.attach(RIGHT_EYE, minRightEye, maxRightEye);
  servoLeftEye.attach(LEFT_EYE, minLeftEye, maxLeftEye);
  servoMouth.attach(MOUTH, minMouth, maxMouth);
  servoEyelid.attach(EYELID, minEyelid, maxEyelid);

  while((stateEye != eyes) || 
        (stateEyelid != eyelid) || 
        (stateMouth != mouth) ||
        (stateColor != color)) {
    if (stateEye != eyes) {
      stateEye += eyeOp;
      servoLeftEye.write(stateEye);
      servoRightEye.write(180 - stateEye);
    }
    if (stateEyelid != eyelid) {
      stateEyelid += lidOp;
      servoEyelid.write(stateEyelid);
    }
    if (stateMouth != mouth) {
      stateMouth += mouthOp;
      servoMouth.write(stateMouth);
    }
    if (stateColor != color) {
      stateColor += colorOp;
      getRGB(stateColor);
      analogWrite(RED,r);
      analogWrite(GREEN,g);
      analogWrite(BLUE,b);
    }
    delay(100);
  }
  servoRightEye.detach();
  servoLeftEye.detach();
  servoMouth.detach();
  servoEyelid.detach();
}

void getRGB(unsigned int hue){

  unsigned int sat = 0xFF;
  unsigned int val = 0xFF;
  unsigned int H_accent = hue/60;
  unsigned int bottom = ((255 - sat) * val)>>8;
  unsigned int top = val;
  unsigned char rising  = ((top-bottom)  *(hue%60   )  )  /  60  +  bottom;
  unsigned char falling = ((top-bottom)  *(60-hue%60)  )  /  60  +  bottom;

  switch(H_accent) {
  case 0:
    r = top;
    g = rising;
    b = bottom;
    break;

  case 1:
    r = falling;
    g = top;
    b = bottom;
    break;

  case 2:
    r = bottom;
    g = top;
    b = rising;
    break;

  case 3:
    r = bottom;
    g = falling;
    b = top;
    break;

  case 4:
    r = rising;
    g = bottom;
    b = top;
    break;

  case 5:
    r = top;
    g = bottom;
    b = falling;
    break;
  }

}
