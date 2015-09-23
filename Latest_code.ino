
                                                                                                                 /* Includes */
#include <String.h>
#include <Math.h>
#include <Servo.h> 

String buf = "";     // a string to hold incoming data

                                                                                                                 /* Variables */

int LX = 0;   // Left stick  (-127, 128)
int BTN = 0;  // Button state (0-65k), interesting 32 and 512
int LT = 0;   // Left Trigger (0, 255)
int RT = 0;   // Right Trigger (0, 255) 
//float RATE = 3.5; // Коэффицитент умножения
#define RATE 35/10
#define speed motor.readMicroseconds()
#define rLeft left.readMicroseconds()
#define rRight right.readMicroseconds()
#define rTop top.readMicroseconds()
// исключительный случай, когда
// define-константу не надо заносить в скобки
int button32 = 0;
int n_min = 1580; //Мин угол поворота в мсек              nozzle_min
int n_max = 2200; //Мах угол поворота в мсек              nozzle_max
int m_min = 1051; //Мин разгон                            motor_min
int m_max = 2000; //Мах разгон                            motor_max


Servo left;
Servo top;
Servo right;
Servo motor;



                                                                                                        /* Function declaration*/

//void parseBuffer(String &str);

                                                                                                                      /* Setup*/
void setup() {
  left.attach(11);  // Left Nozzle
  top.attach(10);  // Top Nozzle 
  right.attach(9);  // Right Nozzle
  motor.attach(6);  // Motor
  left.writeMicroseconds(1700);               // Installing standart angles
  top.writeMicroseconds(1700);
  right.writeMicroseconds(1700);
  motor.writeMicroseconds(800);  
  buf.reserve(255);     // Reserve 255 chars
  Serial.begin(115200);    // Initialize UART at 115200 bod
}


                                                                                                                 /*Logic code */
void loop() 
{
  if(LX > 0)                                                  // Left stick  (-127, 128)
    right.writeMicroseconds(n_min + LX * RATE);             // 127*3,5~445 
  
  if(LX < 0)                                                  // Left stick  (-127, 128) 
    left.writeMicroseconds(n_min - LX * RATE);            // -127*(-3,5)~445 

  if (BTN == 512)  // Button state (0-65k)
  {
     if( (m_max > speed) && (speed > m_min) )
          {
           motor.writeMicroseconds(speed - 20);
          }
   }
  else
    {
    motor.writeMicroseconds(m_min);
    }
    
  
  if (RT > 0)   // Right Trigger (0, 255) 
       if ( (m_max > speed ) && ( speed > m_min ))
             {
              motor.writeMicroseconds(speed + RT*RATE);
             }
  
  if (LT > 0)   // Left Trigger (0, 255)
         if ((m_max > speed ) && ( speed > m_min ))
              {
               motor.writeMicroseconds(speed + (LT*(-RATE)));
              } 
    
   if(RT == 0)   // Right Trigger (0, 255) 
         if ((m_max > speed ) && ( speed > m_min ))
              {
               motor.writeMicroseconds(speed + (LT/33));
              }
   
   if(LT == 0)
		delay(1);// Left Trigger (0, 255)
delay(50);   
}   
                                                                                                               /*Serial event*/
void serialEvent() 
{
  while (Serial.available()) 
  {
    char c = (char)Serial.read();  // Read character
    buf += c;                      // Add it to buffer
    
    // If end-of-line, reset buffer and send back the data
    if (c == '\n')
    {
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
      Serial.print(speed,DEC);
      Serial.print(" rLeft=");
      Serial.print(rLeft,DEC);
      Serial.print(" rRight=");
      Serial.print(rRight,DEC);
      Serial.print("\n");    
      return;
    }
  }
}
                                                                                                               /*Buffer parse*/
void parseBuffer()
{
  if(buf.length()<5)
  {
    LX=0;
    BTN=0;
    LT=0;
    RT=0;
    buf.remove(0,buf.length()); // clear buffer
  }
  else
  {
    sscanf(buf.c_str(),
           "%i %i %i %i",
           &LX, &BTN, &LT, &RT);
    buf = "";
  }
}
