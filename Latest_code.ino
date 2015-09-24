
			/* Includes */
#include <String.h>
#include <Math.h>
#include <Servo.h>

//закомментировать, когда будешь собирать боевой вариант

			/* Variables */
String buf = "";     // a string to hold incoming data

int LX = 0;   // Left stick  (-127, 128)
int BTN = 0;  // Button state (0-65k), interesting 32 and 512
int LT = 0;   // Left Trigger (0, 255)
int RT = 0;   // Right Trigger (0, 255) 

#define RATE 35/10

#define	SPEED	motor.readMicroseconds()
#define	R_LEFT	left.readMicroseconds()
#define	R_RIGHT	right.readMicroseconds()
#define	R_TOP	top.readMicroseconds()
// исключительный случай, когда
// define-константу не надо заносить в скобки
int button32 = 0;
#define	N_MIN	1580	/*Мин угол поворота в мсек              nozzle_min*/
#define	N_MAX	2200	/*Мах угол поворота в мсек              nozzle_max*/
#define M_MIN	1051	/*Мин разгон                            motor_min*/
#define M_MAX	2000	/*Мах разгон                            motor_max*/


Servo left;
Servo top;
Servo right;
Servo motor;
			/* Function declaration*/

			/* Setup*/
void setup() {
	left.attach(11);			// Left Nozzle
	top.attach(10);				// Top Nozzle 
	right.attach(9);			// Right Nozzle
	motor.attach(6);			// Motor
	left.writeMicroseconds(1700);		// Installing standart angles
	top.writeMicroseconds(1700);
	right.writeMicroseconds(1700);
	motor.writeMicroseconds(800);  
	buf.reserve(50);			// Reserve 50 chars (тебе за глаза хватит)
	Serial.begin(115200);			// Initialize UART at 115200 bod
}


			/*Logic code */
void loop(){
	if(LX > 0)						// Left stick  (-127, 128)
		right.writeMicroseconds(N_MIN + LX * RATE);	// 127*3,5~445 

	if(LX < 0)						// Left stick  (-127, 128) 
		left.writeMicroseconds(N_MIN - LX * RATE);	// -127*(-3,5)~445 

	if (BTN & 0x0200){					// Button state (0-65k)
	//это можно расшифровать как "если кнопка RB нажата", при этом могут быть нажаты и другие
		if( (SPEED > M_MIN) ){
			motor.writeMicroseconds((SPEED-20>M_MIN)?(SPEED-20):M_MIN);
		}
	}
	else{
		if ( (RT > 0) && (M_MAX > SPEED ) ){
			motor.writeMicroseconds(SPEED+(RT*RATE));
		}

		if ( (LT > 0) && ( SPEED > M_MIN ) )	{
			motor.writeMicroseconds((SPEED-LT*0.05>M_MIN)?(SPEED-LT*0.05):M_MIN);
		} 

		if ( (RT == 0) && (LT == 0) && (SPEED > M_MIN) ){
			motor.writeMicroseconds((SPEED-5>M_MIN)?(SPEED-5):M_MIN);
		}
	}
	delay(50);   
}   
			/*Serial event*/
void serialEvent(){
	while (Serial.available()){
		char c = (char)Serial.read();  // Read character
		buf += c;                      // Add it to buffer

		// If end-of-line, reset buffer and send back the data
		if (c == '\n'){
			parseBuffer();  // Parse received data
			delay(1);      // Small delay for synchronization
			// Sendback the data
			Serial.print("LX=");
			Serial.print(LX,DEC);
			Serial.print(" BTN=");
			Serial.print(BTN,DEC);
			Serial.print(" LT=");
			Serial.print(LT,DEC);
			Serial.print(" RT=");
			Serial.print(RT,DEC);
			Serial.print(" speed=");
			Serial.print(SPEED,DEC);
			Serial.print(" rLeft=");
			Serial.print(R_LEFT,DEC);
			Serial.print(" rRight=");
			Serial.print(R_RIGHT,DEC);
			Serial.print("\n");
			return;
		}
	}
}
			/*Buffer parse*/
void parseBuffer(){
	if(buf.length()<5){
		LX=BTN=LT=RT=0;
		buf= ""; // clear buffer
	}
	else{
		sscanf(	buf.c_str(),
			"%i %i %i %i",
			&LX, &BTN, &LT, &RT);
		buf = "";
	}
}

