// GeckoPad Firmware v1

#include <Keyboard.h>
#include "ArduinoLowPower.h"
#include "KeyCodes.h"

// BLUETOOTH SETUP
#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"
//#if SOFTWARE_SERIAL_AVAILABLE
//  #include <SoftwareSerial.h>
//#endif
#define FACTORYRESET_ENABLE         0
#define MINIMUM_FIRMWARE_VERSION    "0.6.6"
#define VERBOSE_MODE                1
/* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
bool BluetoothOn = 0;

// NEO PIXEL SETUP
#include <Adafruit_NeoPixel.h>
#include "Adafruit_Keypad.h"
#define ROWS 5 // rows
#define COLS 6 // columns
#define NEOPIXEL_PIN 5
#define NUM_PIXELS (ROWS * COLS)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);
uint8_t rowPins[ROWS] = {18, 17, 16, 15, 14}; //connect to the row pinouts of the keypad
uint8_t colPins[COLS] = {13, 12, 11, 10, 9, 6}; //connect to the column pinouts of the keypad
bool lit[ROWS * COLS] = {0};


//define the symbols on the buttons of the keypads
const keymap_t keys[ROWS][COLS] = {
  {MODE_OTHER,  MODE_NUM,   MODE_SYM,   ENUMKEY_enter,  MOD_LSHIFT, ENUMKEY_cpslck},
  {MOD_LGUI,    ENUMKEY_U,  ENUMKEY_I,  ENUMKEY_N,  ENUMKEY_S,  ENUMKEY_Y},
  {MOD_LALT,    ENUMKEY_O,  ENUMKEY_E,  ENUMKEY_H,  ENUMKEY_T,  ENUMKEY_D},
  {MOD_LCTRL,   ENUMKEY_F,  ENUMKEY_A,  ENUMKEY_R,  ENUMKEY_W,  ENUMKEY_P},
  {MODE_INPUT,  MODE_M5,    MODE_M4,    MODE_M3,    MODE_M2,    MODE_M1}
};

const keymap_t otherkeys[ROWS][COLS] = {
  {MODE_OTHER,  MODE_NUM,   ENUMKEY_comma,  ENUMKEY_dot,  MOD_LSHIFT, ENUMKEY_cpslck},
  {MOD_LGUI,    MOD_LGUI,   ENUMKEY__,      ENUMKEY_K,    ENUMKEY_G,  ENUMKEY_X},
  {MOD_LALT,    ENUMKEY_Q,  ENUMKEY_Z,      ENUMKEY_L,    ENUMKEY_C,  ENUMKEY_V},
  {MOD_LCTRL,   ENUMKEY_tab,ENUMKEY_ping,   ENUMKEY_B,    ENUMKEY_M,  ENUMKEY_J},
  {MODE_INPUT,  MODE_M5,    MODE_M4,        MODE_M3,      MODE_M2,    MODE_M1}
};

const keymap_t numkeys[ROWS][COLS] = {
  {MODE_OTHER,  MODE_NUM,       MODE_SYM,   ENUMKEY_enter,  MOD_LSHIFT, ENUMKEY_cpslck},
  {MOD_LGUI,    ENUMKEY_dot,    ENUMKEY_3,  ENUMKEY_2,      ENUMKEY_1,  ENUMKEY_0},
  {MOD_LALT,    MACRO_plus,     ENUMKEY_6,  ENUMKEY_5,      ENUMKEY_4,  ENUMKEY_slash},
  {MOD_LCTRL,   ENUMKEY_equal,  ENUMKEY_9,  ENUMKEY_8,      ENUMKEY_7,  MACRO_asterisk},
  {MODE_INPUT,  MODE_M5,        MODE_M4,    MODE_M3,        MODE_M2,    MODE_M1}
};

const keymap_t symkeys[ROWS][COLS] = {
  {MODE_OTHER,  MODE_NUM,       MODE_SYM,         ENUMKEY_enter,    MOD_LSHIFT,   ENUMKEY_cpslck},
  {MOD_LGUI,    MACRO_quotes,   MACRO_lt,         MACRO_openparen,  ENUMKEY_lbr,  MACRO_opencurly},
  {MOD_LALT,    ENUMKEY_slash,  MACRO_exclamation,ENUMKEY_minus,    MACRO_atsign, MACRO_underscore},
  {MOD_LCTRL,   MACRO_question, MACRO_ampersand,  MACRO_asterisk,   MACRO_colon,  ENUMKEY_smcol},
  {MODE_INPUT,  MODE_M5,        MODE_M4,          MODE_M3,          MODE_M2,      MODE_M1}
};

const keymap_t symotherkeys[ROWS][COLS] = {
  {MODE_OTHER,  MODE_NUM,       MODE_SYM,         ENUMKEY_enter,    MOD_LSHIFT,   ENUMKEY_cpslck},
  {MOD_LGUI,    ENUMKEY_grave,  MACRO_gt,         MACRO_closeparen, ENUMKEY_rbr,  MACRO_closecurly},
  {MOD_LALT,    ENUMKEY_bckslsh,MACRO_percent,    MACRO_pipe,       MACRO_tilde,  MACRO_caret},
  {MOD_LCTRL,   ENUMKEY_tab,    MACRO_plus,       ENUMKEY_equal,    MACRO_dollar, MACRO_hash},
  {MODE_INPUT,  MODE_M5,        MODE_M4,          MODE_M3,          MODE_M2,      MODE_M1}
};

const keymap_t funckeys[ROWS][COLS] = {
  {MODE_OTHER,  MODE_NUM,     MODE_SYM,       ENUMKEY_enter,  MOD_LSHIFT,   ENUMKEY_cpslck},
  {MOD_LGUI,    ENUMKEY_del,  ENUMKEY_bckspc, ENUMKEY_rarr,   ENUMKEY_darr, ENUMKEY_larr},
  {MOD_LALT,    ENUMKEY_ins,  MOD_LCTRL,      MOD_LALT,       ENUMKEY_uarr, ENUMKEY_pgdn},
  {MOD_LCTRL,   ENUMKEY_esc,  ENUMKEY_end,    ENUMKEY_home,   ENUMKEY_break,ENUMKEY_pgup},
  {MODE_INPUT,  MODE_M5,      MODE_M4,        MODE_M3,        MODE_M2,      MODE_M1}
};


//initialize an instance of class NewKeypad
Adafruit_Keypad customKeypad = Adafruit_Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);

enum InputMode {
  USBINPUT,
  BLUETOOTH
};
InputMode inputMode = USBINPUT;
enum ColourMode {
  RAINBOW,
  FADE,
  FADEBLUE,
  FADEPURPLE,
  FADEGREEN
};
ColourMode colourMode = FADE;
enum Mode {
  ALPHA,
  OTHER,
  NUM,
  SYM,
  FUNCTION
};
Mode mode = ALPHA;

bool keySent = false;

bool isCapsLocked = false;
bool isNumLocked = false;
bool isSymLocked = false;

bool isCTRL = false;
bool isALT = false;
bool isSHIFT = false;
bool isGUI = false;


keymap_t modKeys = 0x00;



// TIMEING SETUP
unsigned long startMillis;  //some global variables available anywhere in the program
unsigned long currentMillis;
const unsigned long period = 300*1000;  //the value is a number of milliseconds

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)

  Serial.begin(115200);
  //while (!Serial);
  Serial.println("GeckoPad");
  strip.begin();
  strip.setBrightness(40);
  strip.show(); // Initialize all pixels to 'off'

  customKeypad.begin();
  for (int i = 0; i < ROWS * COLS; i++) {
    lit[i] = false;
  }
  delay(3000);
  Keyboard.begin();

  
  startMillis = millis();  //initial start time

}

uint8_t j = 0; // color ticker

void loop() {
  currentMillis = millis();  //get the current "time" (actually the number of milliseconds since the program started)
  //Serial.println("Test NeoPixels");
  customKeypad.tick();

  while (customKeypad.available()) {
    keypadEvent e = customKeypad.read();
    Serial.print((char)e.bit.KEY);
    if (e.bit.EVENT == KEY_JUST_PRESSED) {
      Serial.println(" pressed");
      uint8_t row = e.bit.ROW;
      uint8_t col = e.bit.COL;
      Serial.print("Row: "); Serial.print(row);
      Serial.print(" col: "); Serial.print(col);
      Serial.print(" -> ");
      uint16_t keynum;
      if (row % 2 == 0) { // even row
        keynum = row * COLS + col;
      } else { // odd row the neopixels go BACKWARDS!
        keynum = row * COLS + (5 - col);
      }
      Serial.println(keynum);
      initialLightKey(keynum);
      sendKeystroke(e, row, col);
      
      startMillis = currentMillis;  //IMPORTANT to save the start time of the current LED state.
    }
    else if (e.bit.EVENT == KEY_JUST_RELEASED) {
      Serial.println(" released");
      uint8_t row = e.bit.ROW;
      uint8_t col = e.bit.COL;
      sendRelease(e, row, col);
    }
  }

  setKeyColour();
  
  if (currentMillis - startMillis >= period)  //test whether the period has elapsed
  {
    gotoSleep();
  }
}





void sendKeystroke(keypadEvent e, uint8_t row, uint8_t col) {
//      if (customKeypad.isPressed(' ') and (char)e.bit.KEY != ' ') {
//          sendKeystroke2((char)spckeys[row][col]);
//          Space = 1;
//      } else if (Space == 1 and (char)e.bit.KEY == ' ') {
//        Space = 0;
//      } else if (customKeypad.isPressed(KEY_F24) and (char)e.bit.KEY != KEY_F24) {
//          sendKeystroke2((char)funckeys[row][col]);
//          Number = 1;
//      } else if (Number == 1 and (char)e.bit.KEY == KEY_F24) {
//        Number = 0;
//      } else if ((char)e.bit.KEY == KEY_LEFT_SHIFT) {
//        sendKeystroke2(KEY_LEFT_SHIFT);
//      } else if ((char)e.bit.KEY == 3) {
//        changeMode();
//      } else if ((char)e.bit.KEY == 65) {
//        ChangeColourMode();
//      } else if ((char)e.bit.KEY == KEY_LEFT_GUI) {
//        key_gui = !key_gui; // invert status
//      } else if ((char)e.bit.KEY == KEY_LEFT_CTRL) {
//        key_ctrl = !key_ctrl; // invert status
//      } else if ((char)e.bit.KEY == KEY_LEFT_ALT) {
//        key_alt = !key_alt; // invert status
//      } else { 
//        sendKeystroke2((char)e.bit.KEY);
//      }
  keySent = false;

  keymap_t theKey;
   if (mode == ALPHA) {
    if(isSymLocked) theKey = symkeys[row][col];
    else theKey = keys[row][col];
  } else if (mode == OTHER) {
    if(isSymLocked) theKey = symotherkeys[row][col];
    else theKey = otherkeys[row][col];
  } else if (mode == NUM and isNumLocked==false) {
    theKey = funckeys[row][col];
  } else if (mode == NUM and isNumLocked==true) {
    theKey = numkeys[row][col];
  }
  //symotherkeys
  switch (theKey)  {
    case MODE_INPUT:
      changeInputMode();
      return;
    case MODE_OTHER:
          mode = OTHER;
        return;
    case MODE_NUM:
        mode = NUM;
      return;
    case MODE_SYM:
      if (isSymLocked) {
        isSymLocked = false;
      } else {
        isSymLocked = true;
      }
      return;
    case MODE_FUNC:
      if (mode == FUNCTION) {
        mode = ALPHA;
        //isNumLocked = false;
      } else {
        mode = FUNCTION;
      }
      return;
    // Handle mode locks
    case ENUMKEY_cpslck:
      if (isCapsLocked){
        isCapsLocked = false;
        modKeys = 0x00;
      } else {
        isCapsLocked = true;
        modKeys = 0x02;
      }
      return;
    case MOD_LCTRL:
      modKeys = modKeys ^ 0x01;
      isCTRL = true;
      keySent = true;
      return;
    case MOD_LSHIFT:
      modKeys = modKeys ^ 0x02;
      isSHIFT = true;
      return;
    case MOD_LALT:
      modKeys = modKeys ^ 0x04;
      isALT = true;
      keySent = true;
      return;
    case MOD_LGUI:
      modKeys = modKeys ^ 0x08;
      isGUI = true;
      keySent = true;
      return;
    case MOD_RCTRL:
      modKeys = modKeys ^ 0x10;
      return;
    case MOD_RSHIFT:
      modKeys = modKeys ^ 0x20;
      return;
    case MOD_RALT:
      modKeys = modKeys ^ 0x40;
      return;
    case MOD_RGUI:
      modKeys = modKeys ^ 0x80;
      return;
    case MULTI_CtlAlt:
      modKeys = modKeys ^ 0x01;
      modKeys = modKeys ^ 0x04;
      return;
    /* Everything after this sends actual keys to the system; break rather than
       return since we want to reset the modifiers after these keys are sent. */
    case MACRO_000:
      sendRawKey(0x00, 0x27);
      sendRawKey(0x00, 0x27);
      sendRawKey(0x00, 0x27);
      break;
    case MACRO_00:
      sendRawKey(0x00, 0x27);
      sendRawKey(0x00, 0x27);
      break;
    case MACRO_quotes:
      sendRawKey(0x02, 0x34);
      break;
    case MACRO_parens:
      sendRawKey(0x02, 0x26);
      sendRawKey(0x02, 0x27);
      sendRawKey(0x00, 0x50);
      break;
    case MACRO_dollar:
      sendRawKey(0x02, 0x21);
      break;
    case MACRO_percent:
      sendRawKey(0x02, 0x22);
      break;
    case MACRO_ampersand:
      sendRawKey(0x02, 0x24);
      break;
    case MACRO_asterisk:
      sendRawKey(0x02, 0x25);
      break;
    case MACRO_question:
      sendRawKey(0x02, 0x38);
      break;
    case MACRO_plus:
      sendRawKey(0x02, 0x2E);
      break;
    case MACRO_openparen:
      sendRawKey(0x02, 0x26);
      break;
    case MACRO_closeparen:
      sendRawKey(0x02, 0x27);
      break;
    case MACRO_opencurly:
      sendRawKey(0x02, 0x2F);
      break;
    case MACRO_closecurly:
      sendRawKey(0x02, 0x30);
      break;
    case MACRO_exclamation:
      sendRawKey(0x02, 0x1E);
      break;
    case MACRO_atsign:
      sendRawKey(0x02, 0x1F);
      break;
    case MACRO_gt:
      sendRawKey(0x02, 0x37);
      break;
    case MACRO_lt:
      sendRawKey(0x02, 0x36);
      break;
    case MACRO_colon:
      sendRawKey(0x02, 0x33);
      break;
    case MACRO_pipe:
      sendRawKey(0x02, 0x31);
      break;
    case MACRO_tilde:
      sendRawKey(0x02, 0x35);
      break;
    case MACRO_caret:
      sendRawKey(0x02, 0x23);
      break;
    case MACRO_hash:
      sendRawKey(0x02, 0x20);
      break;
    case MACRO_underscore:
      sendRawKey(0x02, 0x2D);
      break;
    // MODE BUTTONS M1 to M5
    case MODE_M1:
      ChangeColourMode();
      return;
    case MODE_M2:
      return;
    case MODE_M3:
      return;
    case MODE_M4:
      return;
    case MODE_M5:
      return;

      
    // Send the key
    default:
      sendRawKey(modKeys, theKey);
      break;
    
  }
  modKeys = 0x00;
  isCTRL = false;
  isALT = false;
  isSHIFT = false;
  isGUI = false;
  //mode = ALPHA;
  // Reset the modKeys and mode based on locks
  if (isCapsLocked){
    modKeys = 0x02;
  }
//  if (isNumLocked){
//    mode = NUM;
//  }
//  if (isSymLocked){
//    mode = SYM;
//  }
}

void sendRawKey(char modKey, char rawKey){
  keySent = true;
  // Format for Bluefruit Feather is MOD-00-KEY.
  String keys = String(modKey, HEX) + "-00-" + String(rawKey, HEX);
  
  ble.print("AT+BLEKEYBOARDCODE=");
  ble.println(keys);
  // Must send this to release the keys.
  ble.println("AT+BLEKEYBOARDCODE=00-00");
}

void sendRelease(keypadEvent e, uint8_t row, uint8_t col) {
  keymap_t theKey;
   if (mode == ALPHA) {
    theKey = keys[row][col];
  } else if (mode == OTHER) {
    theKey = otherkeys[row][col];
  } else if (mode == NUM and isNumLocked==false) {
    theKey = funckeys[row][col];
  } else if (mode == NUM and isNumLocked==true) {
    theKey = numkeys[row][col];
  } else if (mode == SYM) {
    theKey = symkeys[row][col];
  } else {
    theKey = funckeys[row][col];
  }
 switch (theKey)  {
    case MODE_OTHER:
      if (mode == OTHER) {
        mode = ALPHA;
      }
      if( keySent == false) {
        sendRawKey(ENUMKEY__, ENUMKEY_spc);  // send space key
      }
      return;
    case MODE_NUM:
      if( keySent == false and isNumLocked == false) {
        isNumLocked = true;
      } else {
        isNumLocked = false;
        mode = ALPHA;
      }
      return;
    default:
      break;
  }
}

//void sendKeystroke2(char key) {
//  if(inputMode == USBINPUT) {
//    Keyboard.write((char)key);
//  } else {
//      sendBluetoothKey(key);
//  }
//}

void changeInputMode() {
  if(inputMode == BLUETOOTH ) {
    inputMode = USBINPUT;
  } else {
    inputMode = BLUETOOTH;
  }
  
  if(inputMode == USBINPUT) {
    ble.end();
    BluetoothOn = 0;
  }
  if(inputMode == BLUETOOTH and BluetoothOn==0) {
    setupBluetooth();
    BluetoothOn = 1;
  }
}


//////////////////////////////////////////////////////////////
///////////////////////// BLUETOOTH //////////////////////////
//////////////////////////////////////////////////////////////

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  //while (1);
}

void setupBluetooth() {
  delay(500);

  
  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'GeckoPad Keyboard': "));
  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=GeckoPad" )) ) {
    error(F("Could not set device name?"));
  }

  /* Enable HID Service */
  Serial.println(F("Enable HID Service (including Keyboard): "));
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    if ( !ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ))) {
      error(F("Could not enable Keyboard"));
    }
  }else
  {
    if (! ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ))) {
      error(F("Could not enable Keyboard"));
    }
  }

  /* Add or remove service requires a reset */
  Serial.println(F("Performing a SW reset (service changes require a reset): "));
  if (! ble.reset() ) {
    error(F("Couldn't reset??"));
  }

  Serial.println();
  Serial.println(F("Go to your phone's Bluetooth settings to pair your device"));
  Serial.println(F("then open an application that accepts keyboard input"));

  Serial.println();
  

}


//////////////////////////////////////////////////////////////
///////////////////////// COLOURING //////////////////////////
//////////////////////////////////////////////////////////////

void initialLightKey(int keynum) {
  if (colourMode == FADE) {
    strip.setPixelColor(keynum, strip.Color( random(0, 255),  random(0, 255),  random(0, 255)));
    lit[keynum] = 1;
  } else if (colourMode == FADEBLUE) {
    strip.setPixelColor(keynum, strip.Color( 0,  0,  255));
    lit[keynum] = 1;
  } else if (colourMode == FADEPURPLE) {
    strip.setPixelColor(keynum, strip.Color( 150,  50,  255));
    lit[keynum] = 1;
  } else if (colourMode == FADEGREEN) {
    strip.setPixelColor(keynum, strip.Color( 0,  255,  0));
    lit[keynum] = 1;
  } else {
    lit[keynum] = !lit[keynum]; // invert neopixel status
  }
}

void setKeyColour() {
  for(int i=0; i< strip.numPixels(); i++) {
    if(i == 23) {  // CTRL
      if(isCTRL) {
        strip.setPixelColor(i, 255, 0, 0);
      } else {
        strip.setPixelColor(i, 0, 0, 0);
      }
    } else if(i == 12) {  // ALT
      if(isALT) {
        strip.setPixelColor(i, 255, 0, 0);
      } else {
        strip.setPixelColor(i, 0, 0, 0);
      }
    } else if(i == 11) {  // GUI
      if(isGUI) {
        strip.setPixelColor(i, 255, 0, 0);
      } else {
        strip.setPixelColor(i, 0, 0, 0);
      }
    } else if(i == 4) {  // SHIFT
      if(isSHIFT) {
        strip.setPixelColor(i, 255, 0, 0);
      } else {
        strip.setPixelColor(i, 0, 0, 0);
      }
    } else if(i == 5) {  // CAPS
      if(isCapsLocked) {
        strip.setPixelColor(i, 255, 0, 0);
      } else {
        strip.setPixelColor(i, 0, 0, 0);
      }
    } else if(i == 2) {  // SYMBOL MODE
      if(isSymLocked) {
        strip.setPixelColor(i, 255, 0, 0);
      } else {
        strip.setPixelColor(i, 0, 0, 0);
      }
    } else if(i == 1) {  // NUMBER MODE
      if(isNumLocked or mode == NUM) {
        strip.setPixelColor(i, 255, 0, 0);
      } else {
        strip.setPixelColor(i, 0, 0, 0);
      }
    } else if(i == 24) {
      if(inputMode == USBINPUT) {
        strip.setPixelColor(i, 255, 0, 0);
      } else if(inputMode == BLUETOOTH) {
        strip.setPixelColor(i, 0, 0, 255);
      }
    } else if(i == 29) {  // M1
      if(colourMode == RAINBOW) strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      else if(colourMode == FADE) strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      else if(colourMode == FADEBLUE) strip.setPixelColor(i, 0, 0, 255);
      else if(colourMode == FADEPURPLE) strip.setPixelColor(i, 150,  50,  255);
      else if(colourMode == FADEGREEN) strip.setPixelColor(i, 0, 255, 0);
      else strip.setPixelColor(i, 0, 0, 0);
//    } else if(i == 28) {  // M2
//      if(colourMode == FADE) strip.setPixelColor(i, 255, 0, 0);
//      else strip.setPixelColor(i, 0, 0, 0);
//    } else if(i == 27) {  // M3
//      if(colourMode == FADEBLUE) strip.setPixelColor(i, 0, 0, 255);
//      else strip.setPixelColor(i, 0, 0, 0);
//    } else if(i == 26) {  // M4
//      if(colourMode == FADEPURPLE) strip.setPixelColor(i, 150,  50,  255);
//      else strip.setPixelColor(i, 0, 0, 0);
//    } else if(i == 25) {  // M5
//      if(colourMode == FADEGREEN) strip.setPixelColor(i, 0, 255, 0);
//      else strip.setPixelColor(i, 0, 0, 0);
    } else if (lit[i]) {
      if (colourMode != RAINBOW) {
        // random colour dimming dtdddtdwthprtdwetdwthrwdtptwhesarnsui
        strip.setPixelColor(i,DimColor(i));  // Dim out the random colour
        delay(10);
      } else {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
    } else {
      strip.setPixelColor(i, 0);
    }
  }
  strip.show();
  j++;
  delay(10);
}

// Calculate 50% dimmed version of a color (used by ScannerUpdate)
uint32_t DimColor(int i)
{
  uint32_t ColorOfThePixel = strip.getPixelColor(i);
  uint8_t r = Red(ColorOfThePixel);
  uint8_t g = Green(ColorOfThePixel);
  uint8_t b = Blue(ColorOfThePixel);
  if (r == 0 and g == 0 and b == 0) {
    lit[i] = !lit[i]; // invert neopixel status
  }
  // Shift R, G and B components one bit to the right
  uint32_t dimColor = strip.Color(r, g, b);
  return dimColor;
}

// Returns the Red component of a 32-bit color
uint8_t Red(uint32_t color)
{
    return (color >> 16) & 0xFF;
}
// Returns the Green component of a 32-bit color
uint8_t Green(uint32_t color)
{
    return (color >> 8) & 0xFF;
}
// Returns the Blue component of a 32-bit color
uint8_t Blue(uint32_t color)
{
    return color & 0xFF;
}
//
void ChangeColourMode() {
  Serial.println("Changing colour mode.."); 
  if(colourMode == RAINBOW)           colourMode = FADE;
  else if (colourMode == FADE)        colourMode = FADEBLUE;
  else if (colourMode == FADEBLUE)    colourMode = FADEPURPLE;
  else if (colourMode == FADEPURPLE)  colourMode = FADEGREEN;
  else colourMode = RAINBOW;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}



//////////////////////////////////////////////////////////////
////////////////////////// SLEEPING //////////////////////////
//////////////////////////////////////////////////////////////

void gotoSleep() {
  Serial.println("Going to sleep..."); 
  //sleep_enable(); // Enabling sleep mode
  //attachInterrupt(13, wakeUp, HIGH); // attaching interrupt to pin d2
  
  //set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Setting the sleep mode, in our case full sleep
  digitalWrite(LED_BUILTIN, LOW); // turning the LED off
  for(int i=0; i< strip.numPixels(); i++) {
    lit[i] = false;
    strip.setPixelColor(i, 0);
  }
  strip.show();
  LowPower.attachInterruptWakeup(13, wakeUp, RISING);
  delay(1000); // wait a second to allow the led to be turned off before going to sleep
  //sleep_cpu(); // activiting sleep mode
  LowPower.deepSleep();
  Serial.println("just woke up!"); // next line of code executed after the interrupt
}

void wakeUp() {
  Serial.println("Interrupt Fired"); // print message to serial monitor
  //sleep_disable(); // Disable sleep mode
  detachInterrupt(13);  // Remove the interrupt from pin 2 
  customKeypad.begin();  // Fix up the keyboard after intrrupt messes it up
  digitalWrite(LED_BUILTIN, LOW); // turning the LED off
  Serial.begin(115200);
  startMillis = millis();  //initial start time
}




//void measureBattery() {
//  // put your main code here, to run repeatedly:
//  float measuredvbat = analogRead(VBATPIN);
//  measuredvbat *= 2;    // we divided by 2, so multiply back
//  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
//  measuredvbat /= 1024; // convert to voltage
//  Serial.print("VBat: " );
//  Serial.println(measuredvbat);
//  delay(5000);                // waits for a second
//}
