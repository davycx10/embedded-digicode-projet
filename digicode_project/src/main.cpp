#include <Arduino.h>
#include "hardware/pio.h"




// define secret code

const char* secret_code = "2*1D";     // secrect code 
char Inputcode[5] = {0};              // buffer for input user
int inputPosition = 0;                // Track how many digits have already been entered in the buffer

// timer for delay

unsigned long lastTime = 0;
const int  Timeout = 15000;   // limit time

// define keyboard matix 4x4

const byte ROW = 4;
const byte COLS = 4;

char touch[ROW][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

byte PinROWS[ROW] = {2, 3, 4, 5};
byte PinCOLS[COLS] = {6, 7, 8, 9};


// define buzzer

#define BUZZER_PIN 27

void buzzer_init(){
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
}

void buzzer_play_tone(int freq, int duration_ms) {
    uint slice = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint32_t clock = 125000000;
    uint32_t wrap = clock / freq;
    pwm_set_wrap(slice, wrap);
    pwm_set_chan_level(slice, pwm_gpio_to_channel(BUZZER_PIN), wrap / 2);
    pwm_set_enabled(slice, true);
    delay(duration_ms);
    pwm_set_enabled(slice, false);
}

// define TFT
// #define TFT_CS    17
// #define TFT_DC    3
// #define TFT_RST   6
// #define TFT_BL    2
// Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// reset input

void resetInput(){
    inputPosition = 0;
    memset(Inputcode, 0, sizeof(Inputcode));
    Serial.println("Enter Code :");
    // tft.println("Enter Code :");
}

// valide  the code
void validateCode(){
    if (strcmp(Inputcode, secret_code) == 0)
    {
        Serial.println("Code Accept, good job looser");
        // tft.println("Code Accept, good job looser");
        buzzer_play_tone(440, 500);
    } else {
        Serial.println("Code reject looser");
        // tft.println("Code reject looser");
        buzzer_play_tone(220, 500);
    }

    delay(2000);
    resetInput();
    
}

void setup() {
    
     

    buzzer_init();
    Serial.begin(9600);
    // Serial.println("System okay! \n Enter a code");

    // init the keyboard
    for (int i = 0; i < ROW; i++) {
        pinMode(PinROWS[i], OUTPUT);
        digitalWrite(PinROWS[i], HIGH);
    }
    for (int i = 0; i < COLS; i++) {
        pinMode(PinCOLS[i], INPUT_PULLUP);
    }

    resetInput();
}

void loop(){
    unsigned long currentTime = millis();

    if (currentTime - lastTime >= (unsigned long)Timeout)
    {
        Serial.println("Timeout: retry looser!!");
        // tft.println("Timeout: retry looser!!");
        resetInput();
        lastTime = currentTime;
        buzzer_play_tone(840, 100);
    }

    for (int  Row = 0; Row < ROW; Row++)
    {
        digitalWrite(PinROWS[Row], LOW);

        for (int cols = 0; cols < COLS; cols++)
        {
            if (digitalRead(PinCOLS[cols]) == LOW)
            {
                delay(250); // anti - rebound
                if (digitalRead(PinCOLS[cols]) == LOW)
                {
                    char key = touch[Row][cols];
                    Serial.println(key);
                    

                    if (inputPosition < 4)
                    {
                        Inputcode[inputPosition] = key;
                        inputPosition++;
                        Serial.println("Input in progress: ");
                        Serial.println(Inputcode);

                        // tft.println("Input in progress: ");
                        // tft.println(Inputcode);
                        lastTime = currentTime; 
                    }
                    
                    if (inputPosition >= 4) 
                    {
                        validateCode();
                    }
                    
                    while (digitalRead(PinCOLS[cols]) == LOW)
                    {
                        delay(250);
                    }
                    
                }
                
            }
            
        }
        digitalWrite(PinROWS[Row], HIGH);
    }
    
    
}