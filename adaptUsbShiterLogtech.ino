/* 
  Code base amstudio with my adaptaions 
  Video project Amstudio: https://youtu.be/dLpWEu8kCec?si=2NlNjffRh_RWp1Nb
*/
/* 
  Borard use: Raspberry pi pico;
  Shifter: logitech g29 shiter

  I'm use the port 3.3V in pico and not having problemas with shifter, the project amstudio thys use arduino with port 5V;
  the conections respect example de video.

  In my case, I reset axios in the libary becouse in forza 5 mapping buttons not work better
*/

#include <Joystick.h>

#define HS_XAXIS_12        400
#define HS_XAXIS_56        500
#define HS_YAXIS_135       800
#define HS_YAXIS_246       300

// Sequential shifter mode analog axis thresholds
#define SS_UPSHIFT_BEGIN   670
#define SS_UPSHIFT_END     600
#define SS_DOWNSHIFT_BEGIN 430
#define SS_DOWNSHIFT_END   500

// Handbrake mode analog axis limits
#define HB_MAXIMUM         530
#define HB_MINIMUM         400
#define HB_RANGE           (HB_MAXIMUM-HB_MINIMUM)

// Digital inputs definitions
#define DI_REVERSE         1
#define DI_MODE            3
#define DI_RED_CENTERRIGHT 4
#define DI_RED_CENTERLEFT  5
#define DI_RED_RIGHT       6
#define DI_RED_LEFT        7
#define DI_BLACK_TOP       8
#define DI_BLACK_RIGHT     9
#define DI_BLACK_LEFT      10
#define DI_BLACK_BOTTOM    11
#define DI_DPAD_RIGHT      12
#define DI_DPAD_LEFT       13
#define DI_DPAD_BOTTOM     14
#define DI_DPAD_TOP        15

// Shifter state
#define DOWN_SHIFT         -1
#define NO_SHIFT           0
#define UP_SHIFT           1

// Shifter mode
#define SHIFTER_MODE       0
#define HANDBRAKE_MODE     1

// LED blink counter
int led=0;

// Shifter state
int shift=NO_SHIFT;

// Handbrake mode
int mode=SHIFTER_MODE;

int b[16];

int gear=0;                          // Default value is neutral


void setup() {
  pinMode(A0, INPUT_PULLUP);   // X axis
  pinMode(A2, INPUT_PULLUP);   // Y axis
  // pinMode(A1, INPUT); // handbreak

  pinMode(2, INPUT); 

  for(int i=0; i<16; i++) b[i] = 0;
  b[DI_MODE] =0;
  // Initialize Joystick Library
  Serial.begin(9600);
  Joystick.begin();
}

int lastButtonState = 0;

void loop() {
  // int handbrake = analogRead(A1);
  // int mapperRangeHandBrake = map(handbrake, 1023, 810, -124, 124);
  // Joystick.slider(mapperRangeHandBrake);

  int x = analogRead(A0);                 // X axis
  int y = analogRead(A2);

  // Reset Function Joystick, my case necessario for exec forza 5 without reset game detect axios with not using
  Joystick.X(0);
  Joystick.Y(0);
  Joystick.Zrotate(0);
  Joystick.sliderLeft(0);
  Joystick.slider(0);
  Joystick.sliderRight(0);
  Joystick.Z(0);
      
 
  Serial.print(" X: ");             // Y axis
  Serial.print(x);             // Y axis
  Serial.print("; Y: ");            // Y axis
  Serial.print(y);            // Y axis
  Serial.print("; DG: ");            // Y axis
  Serial.print(digitalRead(2));            // Y axis
  Serial.println("");

  int _isreverse = digitalRead(2);
  int _gear_ = 0;

if( _isreverse == 1 ){

      _gear_ = 7;
      b[DI_REVERSE]= 1;

}else{ 
  

  if(b[DI_MODE]==0)                    // H-shifter mode?
  {
    if(x<HS_XAXIS_12)                  // Shifter on the left?
    {
      if(y>HS_YAXIS_135) _gear_=1;       // 1st gear
      if(y<HS_YAXIS_246) _gear_=2;       // 2nd gear
    }
    else if(x>HS_XAXIS_56)             // Shifter on the right?
    {
      if(y>HS_YAXIS_135) _gear_=5;       // 5th gear
      if(y<HS_YAXIS_246) _gear_=6;       // 6th gear
     
    }
    else                               // Shifter is in the middle
    {
      if(y>HS_YAXIS_135) _gear_=3;       // 3rd gear
      if(y<HS_YAXIS_246) _gear_=4;       // 4th gear
    }
   
  }

}
  
  
  if(gear!=6) b[DI_REVERSE]=0;         // Reverse gear is allowed only on 6th gear position
  
   if (_gear_ != gear ){
      gear = _gear_;
      desactivar();
      Joystick.button(gear, HIGH);
   }
   delay(50);
}

void desactivar(){
  // Depress virtual button for current gear
  for(int i = 0; i <= 10 ; i++ )  Joystick.button(i, LOW);
}
