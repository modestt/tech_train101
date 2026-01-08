#include <LiquidCrystal_I2C.h> //Install LiquidCrystal_I2C.h

#include <stdlib.h> //Install stdlib.h

#include <limits.h> //Install limits.h

#define GAMEUPDATEINERVALK   500

bool boolLeft = false, boolRight = false ;  
bool boolUp = false, boolDown = false ;  
bool boolStartOver = false ;

#define GRAPHIC_WIDTH 16
#define GRAPHIC_HEIGHT 4
enum DisplayItem {GRAPHIC_ITEM_NONE, GRAPHIC_ITEM_A, GRAPHIC_ITEM_B,GRAPHIC_ITEM_NUM};

LiquidCrystal_I2C lcd(0x27,16,2);

byte block[3] = { B01110, B01110, B01110, };

byte apple[3] = { B00100, B01010, B00100, };


struct Pos { uint8_t x=0, y=0; };

struct Pos snakePosHistory[GRAPHIC_HEIGHT*GRAPHIC_WIDTH]; 

//first element is the head.

size_t snakeLength = 0;

enum {SNAKE_LEFT,SNAKE_UP,SNAKE_RIGHT,SNAKE_DOWN} snakeDirection;

struct Pos applePos;

unsigned long lastGameUpdateTick = 0;

unsigned long gameUpdateInterval = GAMEUPDATEINERVALK;

bool thisFrameControlUpdated = false;

enum {GAME_MENU, GAME_PLAY, GAME_LOSE, GAME_WIN} gameState;

uint8_t graphicRam[GRAPHIC_WIDTH*2/8][GRAPHIC_HEIGHT];


void setup() {

Serial.begin(9600);
Serial.print("Init\n\r"); 

lcd.init();

lcd.backlight();
lcd.setCursor(0, 0);

lcd.print(" Snake Game");

lcd.setCursor(0, 1);
lcd.print("w=U,s=D,a=L,d=R");

graphic_generate_characters();

gameState = GAME_MENU;

}

void loop() {

lcd.setCursor(0, 0);

//if(digitalRead(BUTTON_LEFT)==HIGH)
if(boolLeft)
{
  boolLeft = false ;

switch(gameState){

case GAME_PLAY:

switch(snakeDirection){

//case SNAKE_LEFT: snakeDirection=SNAKE_DOWN; break;

//case SNAKE_UP: snakeDirection=SNAKE_LEFT; break;

//case SNAKE_RIGHT: snakeDirection=SNAKE_UP; break;

//case SNAKE_DOWN: snakeDirection=SNAKE_RIGHT; break;

default:
snakeDirection=SNAKE_LEFT;
break ;

}

thisFrameControlUpdated = true;

break;

case GAME_MENU:

game_init();

break;

}

}


lcd.setCursor(8, 0);

//if(digitalRead(BUTTON_RIGHT)==HIGH)

if(boolRight)
{
  boolRight = false ;

switch(gameState){

case GAME_PLAY:

switch(snakeDirection){

//case SNAKE_LEFT: snakeDirection=SNAKE_UP; break;

//case SNAKE_UP: snakeDirection=SNAKE_RIGHT; break;

//case SNAKE_RIGHT: snakeDirection=SNAKE_DOWN; break;

//case SNAKE_DOWN: snakeDirection=SNAKE_LEFT; break;

default:
snakeDirection=SNAKE_RIGHT;
break ;

}

thisFrameControlUpdated = true;

break;

case GAME_MENU:

game_init();

break;

}

}


if(boolUp)
{
  boolUp = false ;

switch(gameState){

case GAME_PLAY:

switch(snakeDirection)
{

//case SNAKE_LEFT: snakeDirection=SNAKE_DOWN; break;

//case SNAKE_UP: snakeDirection=SNAKE_LEFT; break;

//case SNAKE_RIGHT: snakeDirection=SNAKE_UP; break;

//case SNAKE_DOWN: snakeDirection=SNAKE_RIGHT; break;

default:
snakeDirection=SNAKE_UP;
break ;

}

thisFrameControlUpdated = true;

break;

case GAME_MENU:

game_init();

break;

}

}



if(boolDown)
{
  boolDown = false ;

switch(gameState){

case GAME_PLAY:

switch(snakeDirection)
{

//case SNAKE_LEFT: snakeDirection=SNAKE_DOWN; break;

//case SNAKE_UP: snakeDirection=SNAKE_LEFT; break;

//case SNAKE_RIGHT: snakeDirection=SNAKE_UP; break;

//case SNAKE_DOWN: snakeDirection=SNAKE_RIGHT; break;

default:
snakeDirection=SNAKE_DOWN;
break ;


}

thisFrameControlUpdated = true;

break;

case GAME_MENU:

game_init();

break;

}

}




if(millis()-lastGameUpdateTick>gameUpdateInterval){

game_calculate_logic();

game_calculate_display();

lastGameUpdateTick = millis();

thisFrameControlUpdated = false;

}

}    //End Loop

void serialEvent() {
  if (Serial.available()) {

      // get the new byte:
    char inChar = (char)Serial.read();
    if ( inChar == 'a') { boolLeft = true ; Serial.print("\n\rLeft"); }
    if ( inChar == 'd') { boolRight = true ; Serial.print("\n\rRight"); }
    if ( inChar == 'w') { boolUp = true ; Serial.print("\n\rUp"); }
    if ( inChar == 's') { boolDown = true ; Serial.print("\n\rDown"); }
     if ( inChar == 'p') { boolStartOver = true ; Serial.print("\n\rPlay"); }

    
  }
}

void graphic_generate_characters()
{
for (size_t i=0; i<8; i++) {

byte glyph[8];

int upperIcon = (i+1)/3;

int lowerIcon = (i+1)%3;

memset(glyph, 0, sizeof(glyph));

if(upperIcon==1)

memcpy(&glyph[0], &block[0], 3);

else if(upperIcon==2)

memcpy(&glyph[0], &apple[0], 3);

if(lowerIcon==1)

memcpy(&glyph[4], &block[0], 3);

else if(lowerIcon==2)

memcpy(&glyph[4], &apple[0], 3);

lcd.createChar(i, glyph);

}

delay(1); 

//Wait for the CGRAM to be written

}

void game_init()
{

srand(micros());

gameUpdateInterval = GAMEUPDATEINERVALK;

gameState = GAME_PLAY;

snakePosHistory[0].x=3; snakePosHistory[0].y=1;

snakePosHistory[1].x=2; snakePosHistory[1].y=1;

snakePosHistory[2].x=1; snakePosHistory[2].y=1;

snakePosHistory[3].x=0; snakePosHistory[3].y=1;

snakeLength = 4;

snakeDirection = SNAKE_RIGHT;

game_new_apple_pos();

thisFrameControlUpdated = false;

}

void game_calculate_logic() {
  

if(gameState!=GAME_PLAY)

return;

//Calculate the movement of the tail

for(size_t i=snakeLength; i>=1; i--){ 

snakePosHistory[i] = snakePosHistory[i-1];

}

//Calculate the head movement

snakePosHistory[0]=snakePosHistory[1];

switch(snakeDirection){

case SNAKE_LEFT: snakePosHistory[0].x--; break;

case SNAKE_UP: snakePosHistory[0].y--; break;

case SNAKE_RIGHT: snakePosHistory[0].x++; break;

case SNAKE_DOWN: snakePosHistory[0].y++; break;

}

//Look for wall collision

if(snakePosHistory[0].x<0||snakePosHistory[0].x>=GRAPHIC_WIDTH||snakePosHistory[0].y

<0||snakePosHistory[0].y>=GRAPHIC_HEIGHT){

gameState = GAME_LOSE;

return;

}

//Look for self collision

for(size_t i=1; i<snakeLength; i++){

if(snakePosHistory[0].x==snakePosHistory[i].x &&

snakePosHistory[0].y==snakePosHistory[i].y){

gameState = GAME_LOSE;

return;

}

}

if(snakePosHistory[0].x==applePos.x && snakePosHistory[0].y==applePos.y){

snakeLength++;

gameUpdateInterval = gameUpdateInterval*9/10;

if(snakeLength>=sizeof(snakePosHistory)/sizeof(*snakePosHistory))

gameState = GAME_WIN;

else

game_new_apple_pos();

}

}

void graphic_add_item(uint8_t x, uint8_t y, enum DisplayItem item) {

graphicRam[x/(8/2)][y] |= (uint8_t)item*(1<<((x%(8/2))*2));

}

void graphic_flush(){

lcd.clear();

for(size_t x=0; x<16; x++){

for(size_t y=0; y<2; y++){

enum DisplayItem upperItem =

(DisplayItem)((graphicRam[x/(8/2)][y*2]>>((x%(8/2))*2))&0x3);

enum DisplayItem lowerItem =

(DisplayItem)((graphicRam[x/(8/2)][y*2+1]>>((x%(8/2))*2))&0x3);

if(upperItem>=GRAPHIC_ITEM_NUM)

upperItem = GRAPHIC_ITEM_B;

if(lowerItem>=GRAPHIC_ITEM_NUM)

lowerItem = GRAPHIC_ITEM_B;

lcd.setCursor(x, y);

if(upperItem==0 && lowerItem==0)

lcd.write(' ');

else

lcd.write(byte((uint8_t)upperItem*3+(uint8_t)lowerItem-1));

}

}

}

void game_new_apple_pos()

{

bool validApple = true;

do{

applePos.x = rand()%GRAPHIC_WIDTH;

applePos.y = rand()%GRAPHIC_HEIGHT;

validApple = true;

for(size_t i=0; i<snakeLength; i++)

{

if(applePos.x==snakePosHistory[i].x && applePos.y==snakePosHistory[i].y){

validApple = false;

break;

}

}

} while(!validApple);

}


void game_calculate_display() {


graphic_clear();

switch(gameState){

case GAME_LOSE:

lcd.clear();

lcd.setCursor(0, 0);

lcd.print("Yikes!");

lcd.print("'p'->start") ;
lcd.setCursor(0, 1);

lcd.print("Length: ");

lcd.setCursor(8, 1);

lcd.print(snakeLength);

if ( boolStartOver )
{
gameState = GAME_PLAY;
boolStartOver = false ;
game_init();
}

break;

case GAME_WIN:

lcd.clear();

lcd.setCursor(0, 0);

lcd.print("You won. Congratz!");

lcd.setCursor(0, 1);

lcd.print("Length: ");

lcd.setCursor(8, 1);

lcd.print(snakeLength);

break;

case GAME_PLAY:

for(size_t i=0; i<snakeLength; i++)

graphic_add_item(snakePosHistory[i].x, snakePosHistory[i].y, GRAPHIC_ITEM_A);

graphic_add_item(applePos.x, applePos.y, GRAPHIC_ITEM_B);

graphic_flush();

break;

case GAME_MENU:

//Do nothing

break;

}

}



void graphic_clear(){

memset(graphicRam, 0, sizeof(graphicRam));

}


