#include <Servo.h>
Servo servoLeft;
Servo servoRight;

void setup() {
	tone(4, 3000, 1000);
	delay(1000);
	servoLeft.attach(13);
	servoRight.attach(12);

	Serial.begin(9600);
}

void loop() {
	float tLeft = float(rcTime(8));
	float tRight = float(rcTime(6));
	
	if((tRight + tLeft) < 500.0) {
		servoLeft.detach();
		servoRight.detach();
	}

	float ndShade;
	ndShade = tRight / (tLeft+tRight) - 0.5;
	int speedLeft, speedRight;
	
	if (ndShade > 0.0) {
		speedRight = int(200.0 - (ndShade * 6000.0));
		speedRight = constrain(speedRight, -200, 200);
		speedLeft = 200;
	} else {
		speedLeft = int(200.0 + (ndShade * 6000.0));
		speedLeft = constrain(speedLeft, -200, 200);
		speedRight = 200; // Full speed left wheel
	}

	Serial.print("Light level: ");
	Serial.println(tRight + tLeft);
	Serial.print("Right: ");
	Serial.println(speedRight);
	Serial.print("Left: ");
	Serial.println(speedLeft);

	maneuver(speedLeft, speedRight, 100);
}

long rcTime(int pin) {
	pinMode(pin, OUTPUT);
	digitalWrite(pin, HIGH);
	delay(5);
	pinMode(pin, INPUT);
	digitalWrite(pin, LOW);

	long time = micros();
	while(digitalRead(pin));
	time = micros() - time;
	return time;
}

void maneuver(int speedLeft, int speedRight, int msTime) {
	servoLeft.writeMicroseconds(1500 + speedLeft);
	servoRight.writeMicroseconds(1500 - speedRight);
	if(msTime==-1) {
		servoLeft.detach();
		servoRight.detach();
	}
	delay(msTime);
}
