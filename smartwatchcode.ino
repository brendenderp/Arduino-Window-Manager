/*
Hey Im Brenden McGraw-Hobbs
Thanks for checking out my project! I made this when I was 17 years old 
its a window system with support for multiple running program and multiple instancing of programs.
sorry if there are silly spelling errors in the comments!
*/
#include <Time.h>
#include <TimeLib.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET     7 
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
const unsigned char PROGMEM Sun [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x00,
0x00, 0x3F, 0xFC, 0x00, 0x00, 0x7F, 0xFE, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x03, 0xFF, 0xFF, 0xC0,
0x03, 0xFF, 0xFF, 0xC0, 0x07, 0xFF, 0xFF, 0xE0, 0x0F, 0xFF, 0xFF, 0xF0, 0x0F, 0xFF, 0xFF, 0xF0,
0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xF8,
0x00, 0x00, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xF8, 0x1F, 0xFF, 0xFF, 0xF8,
0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xC0,
0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFC, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char PROGMEM Cursor [] = {
0xC0, 0x00, 0xE0, 0x00, 0xF0, 0x00, 0xF8, 0x00, 0xFC, 0x00, 0xFE, 0x00, 0xFF, 0x00, 0xFF, 0x80,
0xFF, 0xC0, 0xFF, 0xE0, 0xFF, 0xF0, 0xFC, 0x00, 0xF8, 0x00, 0xF0, 0x00, 0xE0, 0x00, 0xC0, 0x00
};const unsigned char PROGMEM CursorSel [] = {
0xC0, 0x00, 0xA0, 0x00, 0x90, 0x00, 0x88, 0x00, 0x84, 0x00, 0x82, 0x00, 0x81, 0x00, 0x80, 0x80,
0x80, 0x40, 0x80, 0x20, 0x83, 0xF0, 0x84, 0x00, 0x88, 0x00, 0x90, 0x00, 0xA0, 0x00, 0xC0, 0x00
};
const unsigned char PROGMEM Clock [] = {
0xFF, 0x81, 0x89, 0x89, 0xB9, 0x81, 0x81, 0xFF, 
};


//todos 
/*
add Button function to save on storage space
add calculator
add easy support for making new programs



BASIC Interpriter? 
*/





//these values keep track of the cusors position on the screen. this value is also read when determining if a button is being pressed
int cursX= 10;
int cursY= 10;


//the menu variable is used to determine if the desktop is the current focus of the program.
int Menu = 0;
int CursorItem;

//time since last user input and also joystick positions
int inputTime =0;
int InputY = 0;
int InputX = 0;

//the reason I use ints here is so that I can tell how long the button is pressed
int LeftClick= 0;
int RightClick= 0;



//these are the only global arrays I need. windows Keeps track of the Current windows Running

//the Array stores the info in this order, program running, X position, Y position , Window ID, active state

//X position and Y position control the current location of the window on screen, this value allows you to move the windows and schould be used to keep track of window based eliments(text images etc.)

//Window ID is used to keep track of the windows, for instance you dont want all of the windows to have the same position right! So I use an ID to keep track of them! 
//also notice that its diffrent then the current program running

//active state is quite simple! is this window opened? no? well its inactive then! yes?... well then its active!

int Windows[5][5];
int proglist[2] = {5,10,};




void setup() {
 setTime(10,0,0,4,12,2018);
  
  pinMode(4,INPUT_PULLUP);
   pinMode(5,INPUT_PULLUP);
   
   //you should remove this line if your not debugging! 
  Serial.begin(9600);

  
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    for(;;); // Don't proceed, loop forever
  }
  // screen is connected. clear the buffer and display notin'
display.display();
 display.setTextSize(1);      // Normal 1:1 pixel scale
 display.setTextColor(WHITE); // Draw white text
 display.setCursor(0, 0);  
display.display();
}



//Let there be light!
void loop() {

  CheckInput();
  
  display.clearDisplay();
 
if(inputTime>199)
  { 
    Screensaver();
    goto skip;
  }
//cursor handeler
cursX += InputX/5;
cursY += InputY/5;

//set cursor bounds to screen
if (cursX < 0)// up bound
{ cursX =0; }
if (cursY < 0)// left bound
{ cursY =0; }
if (cursX > 63) // down bound
{ cursX =63; }
if (cursY > 127)// right bound
{ cursY =127; }





//if the cursor is over something then set the cursor to the wire mode cursor
if(CursorItem==0){
display.drawBitmap(cursY, cursX, Cursor, 16, 16, 1);
}
else{display.drawBitmap(cursY, cursX, CursorSel, 16, 16, 1); }




if(Menu ==0){
  display.drawBitmap(0, 0, Clock, 8,8, 1);
  //draw the clock and set a hitbox based off its postion and size
  if (cursY > 0-1 && cursY < 0+9 && cursX >0-1 && cursX < 0+9 )
  {
    CursorItem = 25;
  if(RightClick==1) 
  {CreateWindow(10,10,1,1);}
    
  }
  else{ if(CursorItem == 25) {CursorItem = 0;}}
  
  //this function is all the magic!
  DrawWindow();
  
  
  }
  



skip:

display.display();
// ready for another go?
}



 void CheckInput()
 {
   InputY = analogRead(A3)/25;
  InputX = analogRead(A2)/25;
 
 //here is my code for handling button input, it may look werid but theres a reason
 // what I do is I take the button input and if the button is pressed I set 
  if (!digitalRead(4))
    {
        LeftClick ++;
    }
    else {LeftClick=0;}

      if (!digitalRead(5))
      {
        RightClick ++;
      }else {RightClick=0;}
  
  //ignore flux
  if (InputY == 19 ||InputY == 21 || InputY == 18)
  {
     InputY = 20;
 
  }
    if (InputX == 19 ||InputX == 21|| InputX == 18)
  {InputX = 20;
   
    }
if(digitalRead(5))
   {
      if(inputTime<200){inputTime++;}
   }
    else{inputTime=0;}

  InputY -= 20;
  InputX -= 20;
   InputY = InputY * (-1);
   InputX = InputX * (-1);
  //ignore fluxuations in the joystick input

  
 }


//Window Manager --------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define winX = 1;
#define winY = 2;
#define winID = 3;

void CreateWindow(int X,int Y,int ID,int prog)
  {
    int winAV =0;
    for(int check =0;check<4;check++)
    {//check windows currently being used. whichever is not being used will be put into the proglist[prog] var
    if (Windows[check][4]==0){winAV=1; proglist[prog]=check;}
   
    
    }
    
  
  if  (winAV==1)
  {
    Windows[proglist[prog]][0] =prog;
  Windows[proglist[prog]][1] =X;
  Windows[proglist[prog]][2] =Y;
  Windows[proglist[prog]][3] =random(2, 20);
  Windows[proglist[prog]][4] =1;
  
  }


}




void DrawWindow()
{
 for(int Curwin=0;Curwin<4;Curwin++)
  if(Windows[Curwin][4]==1)
  {
  //top bar
   display.drawRect(Windows[Curwin][1],Windows[Curwin][2],64,8,WHITE);
   //main box
  display.drawRect(Windows[Curwin][1],Windows[Curwin][2],64,40,WHITE);
  //close button
  display.drawRect(Windows[Curwin][1]+56,Windows[Curwin][2],8,8,WHITE);

    
  display.setTextColor(WHITE);
 display.setCursor(Windows[Curwin][1]+8, Windows[Curwin][2]+8);
   
 display.print(hour());
 display.print(":");
   display.println(minute());


  //detect if button is over close button
 if (cursY < Windows[Curwin][1]+65 && cursY > Windows[Curwin][1]+55 && cursX < Windows[Curwin][2]+9 && cursX > Windows[Curwin][2]-1 )
  {
   
    CursorItem = Windows[Curwin][3];
    
if(RightClick==1) 
  {
    //when a window is killed destroy all of the evidence.
    
for(int DIE = 0; DIE<5;DIE++){
  Windows[Curwin][DIE] = 0;
  CursorItem =0;
  }




CursorItem =0;
  }

    
  }
else{ if(CursorItem == Windows[Curwin][3]) {CursorItem = 0;}}



 //detect if button is over bar
 if (cursY < Windows[Curwin][1]+66 && cursY > Windows[Curwin][1]-1 && cursX < Windows[Curwin][2]+9 && cursX > Windows[Curwin][2]-1 &&CursorItem == 0 )
  {
   
    CursorItem = Windows[Curwin][3];

  if(RightClick>0) 
  {
    Windows[Curwin][1]= cursY-8;
    Windows[Curwin][2]= cursX-4;
   
   
  }
    
  }
  //If the window is closed then set the Cursor Item back to 0
  else{if(CursorItem == Windows[Curwin][3] && !(cursY < Windows[Curwin][1]+65 && cursY > Windows[Curwin][1]+55 && cursX < Windows[Curwin][2]+9 && cursX > Windows[Curwin][2]-1) ) {CursorItem = 0;}}

  }
}



//Screen saver ----------------------------------------------------------------------------------------------------



float Lines[4] = {2,6,14,25}; // line positions

void Screensaver()
{




 //for every line there is move down at a speed of 0.10 multiplied by the currant distance from the start (speed is cpu bound)
for(int l=0;l<3;l++)
{
Lines[l] = Lines[l]+1 * Lines[l] *0.10 ; //adjust this to change the speed
if  (Lines[l] > 25){Lines[l] = 2;}

}



  
  //draw the sun and horizon line
 display.drawBitmap(48, 15, Sun, 32, 32, 1);
 display.drawLine(0, 44, 128 , 44, WHITE);

// draw vertical lines with offset of 4 based off ditance from center
  for (int i=0; i<7; i++) 
       {
      float linepos = i -3; // if you want to change the number of vertical lines youll need to change this and the above Lines[] array
      display.drawLine(i *16 +16 , 44, i*16 +16 +linepos *5 ,64, WHITE); 
       }


 //draw all of the lines
for(int l=0;l<3;l++)
{
display.drawLine(0, Lines[l] +42, 128 ,Lines[l]+42, WHITE); 
}
  
}
