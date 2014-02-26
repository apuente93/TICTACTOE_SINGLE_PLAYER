#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#if not defined USE_ADAFRUIT_SHIELD_PINOUT 
 #error "For use with the shield, make sure to #define USE_ADAFRUIT_SHIELD_PINOUT in the TFTLCD.h library file"
#endif

// These are the pins for the shield!
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0 
#define WHITE           0xFFFF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

Point p;

//Variable that determines when a game is over
boolean gameOver = false;

//Variables that activates when the tile has been touched
boolean tile1touchedRED = false;
boolean tile2touchedRED = false;
boolean tile3touchedRED = false;
boolean tile4touchedRED = false;
boolean tile5touchedRED = false;
boolean tile6touchedRED = false;
boolean tile7touchedRED = false;
boolean tile8touchedRED = false;
boolean tile9touchedRED = false;

boolean tile1touchedBLUE = false;
boolean tile2touchedBLUE = false;
boolean tile3touchedBLUE = false;
boolean tile4touchedBLUE = false;
boolean tile5touchedBLUE = false;
boolean tile6touchedBLUE = false;
boolean tile7touchedBLUE = false;
boolean tile8touchedBLUE = false;
boolean tile9touchedBLUE = false;

//Variable that holds main screen
boolean mainScreen = true;

boolean singlePlayer = false;

boolean boardNotDrawn = true;

void setup(void) {

  Serial.begin(9600);
  tft.reset();
  
  uint16_t identifier = tft.readRegister(0x0);
  if (identifier == 0x9325) {
    Serial.println("Found ILI9325");
  } else if (identifier == 0x9328) {
    Serial.println("Found ILI9328");
  } else {
    Serial.print("Unknown driver chip ");
    Serial.println(identifier, HEX);
    while (1);
  }

  tft.begin(identifier); 
  tft.fillScreen(BLACK);
  tft.fillRect(30, 100, 180, 50, BLUE);
  tft.fillRect(30, 170, 180, 50, BLUE);
  tft.setTextSize(2);
  tft.setCursor(45, 120);
  tft.println("Single Player");
  tft.setCursor(47, 190);
  tft.println("Multi Player");
 
  pinMode(13, OUTPUT);
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000

unsigned int xHolder = 0;
unsigned int yHolder = 0;

boolean win = false;
boolean lose = false;
boolean tie = false;

int tiles[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
int randomNumber = 0;

boolean computer = false;
boolean player = true;

int zeroCounter = 0;
int counter = 0;

boolean done = false;

void loop()
{

  digitalWrite(13, HIGH);
  p= ts.getPoint();
  digitalWrite(13, LOW);
  
  xHolder = p.x;
  yHolder = p.y;
    // if you're sharing pins, you'll need to fix the directions of the touchscreen pins!
  //pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  //pinMode(YM, OUTPUT);
  
  if (!gameOver)
  {
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE) 
    {
      xHolder = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
      yHolder = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
      
      if (xHolder > 30 && xHolder < 210 && yHolder > 170 && yHolder < 210 && !done)
      {
        singlePlayer = true;
        p.x = 0;
        p.y = 0;
        done = true;
      }
      if (singlePlayer)
      {
        mainScreen = false;
      }
      if (!mainScreen)
      {
        if (boardNotDrawn)
        {
          tft.fillScreen(BLACK);
          tft.drawFastVLine(80, 0, 320, WHITE);
          tft.drawFastVLine(160, 0, 320, WHITE);
          tft.drawFastHLine(0, 106, 240, WHITE);
          tft.drawFastHLine(0, 213, 240, WHITE);
          boardNotDrawn = false;
        }
         //
         //1st Row
         if (!win && !lose)
         {
           if ((player && !computer) && (p.x != 0 && p.y != 0))
           {
             if (p.x> 630 && p.x < TS_MAXX && p.y > 630 && p.y < TS_MAXY)
             {
               tft.drawChar(15, 20, 'X', RED, 0, 10);
               tile1touchedRED = true;
               tiles[0] = 0;
             }
             if (p.x > 410 && p.x < 630 && p.y > 630 && p.y < TS_MAXY)
             {
               tft.drawChar(96, 20, 'X', RED, 0, 10);
               tile2touchedRED = true;
               tiles[1] = 0;
             }
           
             if (p.x > TS_MINX && p.x < 410 && p.y > 630 && p.y < TS_MAXY)
             {
                tft.drawChar(177, 20, 'X', RED, 0, 10);
                tile3touchedRED = true;
                tiles[2] = 0;
             }
             //
             //2nd Row
             if (p.x> 630 && p.x < TS_MAXX && p.y > 400 && p.y < 630)
             {
               tft.drawChar(15, 125, 'X', RED, 0, 10);
               tile4touchedRED = true;
               tiles[3] = 0;
             }
             if (p.x > 410 && p.x < 630 && p.y > 400 && p.y < 630)
             {
               tft.drawChar(96, 125, 'X', RED, 0, 10);
               tile5touchedRED = true;
               tiles[4] = 0;

             }
             if (p.x > TS_MINX && p.x < 410 && p.y > 400 && p.y < 630)
             {
               tft.drawChar(177, 125, 'X', RED, 0, 10);
               tile6touchedRED = true;
               tiles[5] = 0;
             }
             //
             //3rd Row
             if (p.x> 630 && p.x < TS_MAXX && p.y > TS_MINY && p.y < 400)
             {
               tft.drawChar(15, 230, 'X', RED, 0, 10);
               tile7touchedRED = true;
               tiles[6] = 0;
             }
             if (p.x> 410 && p.x < 630 && p.y > TS_MINY && p.y < 400)
             {
               tft.drawChar(96, 230, 'X', RED, 0, 10);
               tile8touchedRED = true;
               tiles[7] = 0;
             }
             if (p.x> TS_MINX && p.x < 410 && p.y > TS_MINY && p.y < 400)
             {
               tft.drawChar(177, 230, 'X', RED, 0, 10);
               tile9touchedRED = true;
               tiles[8] = 0;
             }
             
             if ((tile1touchedRED && tile2touchedRED && tile3touchedRED) || (tile1touchedRED && tile4touchedRED && tile7touchedRED) || 
                (tile1touchedRED && tile5touchedRED && tile9touchedRED) || (tile2touchedRED && tile5touchedRED && tile8touchedRED) || 
                (tile3touchedRED && tile6touchedRED && tile9touchedRED) || (tile3touchedRED && tile5touchedRED && tile7touchedRED) ||
                (tile4touchedRED && tile5touchedRED && tile6touchedRED) || (tile7touchedRED && tile8touchedRED && tile9touchedRED))
             {
                win = true;
                lose = false;
                tie = false;
                gameOver = true;
             }
             for (int i = 0; i < 9; i++)
             {
               if (tiles[i] == 0)
               {
                 zeroCounter++;
               }
             }
             if (zeroCounter == 9)
             {
               tie = true;
               gameOver = true;
             }
             else
             {
               zeroCounter = 0;
               computer = true;
               player = false;
             }
           }

           if ((computer && !player) && (p.x != 0 && p.y != 0))
           {
             randomNumber = random(0, 8);
             Serial.println(randomNumber);
             
             while (tiles[randomNumber] == 0)
             {
                 randomNumber = random(0, 8);
             }
             
             
             if (tiles[randomNumber] == 1)
             {
               tft.drawChar(15, 20, 'O', BLUE, 0, 10);
               tile1touchedBLUE = true;
               tiles[0] = 0;
               computer = false;
               player = true;
             }
             if (tiles[randomNumber] == 2)
             {
               tft.drawChar(96, 20, 'O', BLUE, 0, 10);
               tile2touchedBLUE = true;
               tiles[1] = 0;
               computer = false;
               player = true;
             }
             if (tiles[randomNumber] == 3)
             {
               tft.drawChar(177, 20, 'O', BLUE, 0, 10);
               tile3touchedBLUE = true;
               tiles[2] = 0;
               computer = false;
               player = true;
             }
             if (tiles[randomNumber] == 4)
             {
               tft.drawChar(15, 125, 'O', BLUE, 0, 10);
               tile4touchedBLUE = true;
               tiles[3] = 0;
               computer = false;
               player = true;
             }
             if (tiles[randomNumber] == 5)
             {
               tft.drawChar(96, 125, 'O', BLUE, 0, 10);
               tile5touchedBLUE = true;
               tiles[4] = 0;
               computer = false;
               player = true;
             }
             if (tiles[randomNumber] == 6)
             {
               tft.drawChar(177, 125, 'O', BLUE, 0, 10);
               tile6touchedBLUE = true;
               tiles[5] = 0;
               computer = false;
               player = true;
             }
             if (tiles[randomNumber] == 7)
             {
               tft.drawChar(15, 230, 'O', BLUE, 0, 10);
               tile7touchedBLUE = true;
               tiles[6] = 0;
               computer = false;
               player = true;
             }
             if (tiles[randomNumber] == 8)
             {
               tft.drawChar(96, 230, 'O', BLUE, 0, 10);
               tile8touchedBLUE = true;
               tiles[7] = 0;
               computer = false;
               player = true;
             }
             if (tiles[randomNumber] == 9)
             {
               tft.drawChar(177, 230, 'O', BLUE, 0, 10);
               tile9touchedBLUE = true;
               tiles[8] = 0;
               computer = false;
               player = true;
             }
             if ((tile1touchedBLUE && tile2touchedBLUE && tile3touchedBLUE) || (tile1touchedBLUE && tile4touchedBLUE && tile7touchedBLUE) || 
                (tile1touchedBLUE && tile5touchedBLUE && tile9touchedBLUE) || (tile2touchedBLUE && tile5touchedBLUE && tile8touchedBLUE) || 
                (tile3touchedBLUE && tile6touchedBLUE && tile9touchedBLUE) || (tile3touchedBLUE && tile5touchedBLUE && tile7touchedBLUE) ||
                (tile4touchedBLUE && tile5touchedBLUE && tile6touchedBLUE) || (tile7touchedBLUE && tile8touchedBLUE && tile9touchedBLUE))
             {
                lose = true;
                win = false;
                tie = false;
                gameOver = true;
             }
             for (int i = 0; i < 9; i++)
             {
               if (tiles[i] == 0)
               {
                 zeroCounter++;
               }
             }
             if (zeroCounter == 9)
             {
               tie = true;
               gameOver = true;
             }
             else
             {
               zeroCounter = 0;
             }
           }
         }
         if (tie && !win && !lose)
           {
              tft.fillScreen(BLACK);
              tft.setTextSize(3);
              tft.setCursor(45, 140);
              tft.println("You Tie!");
           }
           if (lose)
           {
             tft.fillScreen(RED);
             tft.setTextSize(3);
             tft.setCursor(45, 140);
             tft.println("You Lose!");
           }
           if (win)
           {
             tft.fillScreen(GREEN);
             tft.setTextSize(3);
             tft.setCursor(45, 140);
             tft.println("You Win!");
           }
      }
    }
  } 
}

