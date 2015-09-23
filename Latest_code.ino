
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
float RATE = 3.5; // Коэффицитент умножения
int button32 = 0;
int n_min = 1580; //Мин угол поворота в мсек              nozzle_min
int n_max = 2000; //Мах угол поворота в мсек              nozzle_max
int m_min = 1051; //Мин разгон                            motor_min
int m_max = 2000; //Мах разгон                            motor_max
int m_stop = 1050; //Stop                                 motor_stop



Servo left;
Servo top;
Servo right;
Servo motor;

int speed = motor.readMicroseconds();

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
    right.writeMicroseconds(n_min + (LX * RATE));             // 127*3,5~445 
  
  if(LX > 0)                                                  // Left stick  (-127, 128) 
    left.writeMicroseconds(n_min + (LX * (-RATE )));          // -127*(-3,5)~445 
  
  if(LX == 0)                                                  // Left stick  (-127, 128)
    left.writeMicroseconds(n_min);
    right.writeMicroseconds(n_min); 
  delay(1);


  if (BTN == 512)  // Button state (0-65k)
  {
     while (m_max > speed > m_min)
          {
           motor.writeMicroseconds(speed - 20);
           delay(50);
          }
   }
  else
    {
    motor.writeMicroseconds(m_min);
    }
    
  
  if (RT > 0)   // Right Trigger (0, 255) 
       while (m_max > speed > m_min)
             {
              motor.writeMicroseconds(speed + (RT * 0, 05));
             }
   delay(50);
  
  if (LT > 0)   // Left Trigger (0, 255)
         while (m_max > speed > m_min)
              {
               motor.writeMicroseconds(speed - (LT*(-0,07)));
              }
   delay(50);   
    
   if(RT == 0)   // Right Trigger (0, 255) 
         while (m_max > speed > m_min)
              {
               motor.writeMicroseconds(speed - (LT*(-0, 03)));
               }
   delay(1);
   
   if(LT == 0)   // Left Trigger (0, 255)
   delay(1);
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
  /*
    int 
    i = buf.indexOf(' ');
    LX = buf.substring(0,i).toInt();
    buf.remove(0,i+1);
    i = buf.indexOf(' ');
    BTN = buf.substring(0,i).toInt();
    buf.remove(0,i+1);
    i = buf.indexOf(' ');
    LT = buf.substring(0,i).toInt();
    buf.remove(0,i+1);
    i = buf.indexOf('\n');
    RT = buf.substring(0,i).toInt();
    buf.remove(0,buf.length());
  */
    sscanf(buf.c_str(),
           "%i %i %i %i",
           &LX, &BTN, &LT, &RT);
    buf.clear();
  }
}