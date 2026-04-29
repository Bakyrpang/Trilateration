#include <Servo.h>
Servo servo1;
Servo servo2;
Servo servo3;
Servo servos[3] = {servo1, servo2, servo3};
const int servo_pins[3] = {3,6,9};
const int sensors[3][2] = {{4,5}, {7,8}, {10,11}};
const int interSensorDelay = 100; //delay for reducing sensor noise from other pulses
const int intraSensorDelay = 5; //delay before another sensor measurement
const int servoStepAngle = 5; //step servo by 5 degrees before each measurement
const int measurementDelayPerStep = 10; //delay for servo to stabalize
const int servoBounds[2] = {0, 90};
const int coords[3][2] = {{0,0}, {2,0}, {0,2}};
const int adjustMaxDistance = 2; //factor in delays for pulseIn timeout in get_distance

int get_distance(int trig, int echo, int max_distance = 100){
  float sound_speed = 0.0343; //sound speed in cm/microsecond
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  int duration = pulseIn(echo, HIGH, 2*adjustMaxDistance*max_distance/sound_speed);
  int distance = (duration*sound_speed)/2;
  Serial.println(distance);
  return distance;
}

float trilaterate(int x1, int y1, int x2, int y2, int x3, int y3, int r1, int r2, int r3){
    int a1 = (2*x2) - (2*x1);
    int b1 = (2*y2) - (2*y1);
    int c1 = r1*r1 - r2*r2 - x1*x1  - y1*y1 + x2*x2 + y2*y2;

    int a2 = (2*x3) - (2*x1);
    int b2 = (2*y3) - (2*y1);
    int c2 = r1*r1 - r3*r3 - x1*x1 - y1*y1 + x3*x3 + y3*y3;

    float y = (c2*a1 - a2*c1)/(a1*b2 - a2*b1);
    float x = (c1 - b1*y)/a1;

    return x, y;
}

void setup(){
  Serial.begin(9600);

  //attach servos to corresponding pins
  for(int i = 0; i < 3; i++){
    servos[i].attach(servo_pins[i]);
  }

  //set sensor pin modes
  for(int i = 0; i < 3; i++){
    pinMode(sensors[i][0], OUTPUT);
    pinMode(sensors[i][1], INPUT);
  }
}

void loop(){
  for(int angle = servoBounds[0]; angle <= servoBounds[1]; angle += servoStepAngle){
    for(int i = 0; i < 3; i++){
      servos[i].write(angle);
    }
    delay(servoStepAngle*measurementDelayPerStep);
    int radii[3];
    for(int i = 0; i < 3; i++){
      radii[i] = get_distance(sensors[i][0], sensors[i][1]);
      delay(interSensorDelay);
    }
    if(radii[0] != 0 & radii[1] != 0 & radii[2] != 0){
      int x, y = trilaterate(coords[0][0], coords[0][1], coords[1][0], coords[1][1], coords[2][0], coords[2][1], radii[0], radii[1], radii[2]);
      Serial.print("x: ");
      Serial.println(x);
      Serial.print("y: ");
      Serial.println(y);
    }
  }
}
