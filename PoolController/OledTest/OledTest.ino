#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Bounce2.h>

#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int selMenu = 0;
bool ll = true;

char mainMenu[4][10] = {
  "  Auto    ",
  "  Manual  ",
  "  Stop    ",
  "  Config  "
};

Bounce dbPm = Bounce();
Bounce dbPl = Bounce();
Bounce dbPr = Bounce();

bool inSelM;
bool inSelR;
bool inSelL;

int manSecs = 0;
long oldMillis = 0;

#define PIN_M 3
#define PIN_L 5
#define PIN_R 4


void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay();

  pinMode(PIN_M, INPUT_PULLUP);
  pinMode(PIN_L, INPUT_PULLUP);
  pinMode(PIN_R, INPUT_PULLUP);

  dbPm.attach(PIN_M, 3);
  dbPl.attach(PIN_L, 5);
  dbPr.attach(PIN_R, 5);

  //  Serial.begin(115200);



}

void loop() {

  bool pm;
  bool pl;
  bool pr;


  dbPm.update();
  pm = dbPm.read();

  if (!pm) {
    if (!inSelM) {
      inSelM = true;
      selMenu++;
      if (selMenu > 3) {
        selMenu = 0;
      }
    }
  } else {
    inSelM = false;
  }


  dbPl.update();
  pl = dbPl.read();

  if (!pl) {
    if (!inSelL) {
      inSelL = true;
      manSecs = manSecs + 30;
      selMenu = 1;
    }
  } else {
    inSelL = false;
  }

  dbPr.update();
  pr = dbPr.read();

  if (!pr) {
    if (!inSelR) {
      inSelR = true;
      manSecs = manSecs + (30 * 60);
      selMenu = 1;
    }
  } else {
    inSelR = false;
  }

  // Serial.println(pm);

  mainDisp();
  runTimer();
  runSchedule();
  display.display();
  //delay(1000);
  display.clearDisplay();

}



void mainDisp() {
  int x;
  int h, m, s;

  x = millis() % 500;
  display.setCursor(0, 0);
  display.setTextSize(2);

  display.setTextColor(WHITE);
  display.println("Mon 07:07p");
  display.println();

  switch (selMenu) {
    case 0: // AUTO
      if (ll)
        display.print("         x");
      else
        display.print("         +");
      manSecs = 0;
      break;
    case 1: //Manual
      runTimer();
      h = (manSecs / 3600);
      m = ((manSecs % 3600) / 60) ;
      s = ((manSecs % 3600) % 60);

      if (h < 10) display.print(0);
      display.print(h);
      display.print(":");
      if (m < 10) display.print(0);
      display.print(m);
      display.print(":");
      if (s < 10) display.print(0);
      display.print(s);

      break;
    case 2: // Stop
      //do something when var equals 2
      manSecs = 0;
      break;
    case 3: //Config
      //do something when var equals 2
      break;
    default:
      // if nothing else matches, do the default
      // default is optional
      break;
  }


  display.println();
  display.setTextColor(BLACK, WHITE);
  display.println(mainMenu[selMenu]);



}


void runTimer() {

  //  Serial.print(millis());
  //  Serial.print(" ");
  //  Serial.print(oldMillis);
  //  Serial.println();

  if ((manSecs > 0) and (millis() - oldMillis) > 1000) {

    manSecs--;
    oldMillis = millis();
  }

}

void runSchedule() {

  if ( (millis() % 500) <= 10) {
    ll = !ll;
  }

}






void mockDisplay() {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  display.println("Mon 07:07p");

  display.setTextSize(1);
  display.println("                 AUTO");
  display.println("Monday");

  display.println("00-07:[00111111]");
  display.println("08-15:[00111111]");
  display.println("16-23:[00111111]");
  display.setTextColor(BLACK, WHITE);
  display.print(" Save ");
  display.setTextColor(WHITE);
  display.println(" Cancel ");

  display.display();
  //delay(1);

}
