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

// define led blue
#define LEDBLUE_PIN 16

void ledBlue_init(){
     pinMode(LEDBLUE_PIN, OUTPUT);
}

void ledBlue_blink(){
    digitalWrite(LEDBLUE_PIN, HIGH); // Allumer la LED
  delay(1000);                  // Attendre 500 ms
  digitalWrite(LEDBLUE_PIN, LOW);  // Éteindre la LED
  delay(500);    
}

// define led green
#define LEDGREEN_PIN 17
void ledGreen_init(){
    pinMode(LEDGREEN_PIN, OUTPUT);
}

void ledGreen_blink(){
    digitalWrite(LEDGREEN_PIN, HIGH); // Allumer la LED
  delay(2000);                  // Attendre 500 ms
  digitalWrite(LEDGREEN_PIN, LOW);  // Éteindre la LED
  delay(500);
}


#define LEDB_PIN 12

void ledB_init(){
    pinMode(LEDB_PIN, OUTPUT);
}

void ledB_blink(){
    digitalWrite(LEDB_PIN, HIGH); // Allumer la LED
  delay(3000);                  // Attendre 500 ms
  digitalWrite(LEDB_PIN, LOW);  // Éteindre la LED
  delay(500);
}


// reset input

void resetInput(){
    inputPosition = 0;
    memset(Inputcode, 0, sizeof(Inputcode));
    Serial.println("Enter Code :");
    
    
}

// valide  the code
void validateCode(){
    if (strcmp(Inputcode, secret_code) == 0)
    {
        Serial.println("Code Accept, good job looser");
        // tft.println("Code Accept, good job looser");
        buzzer_play_tone(5000, 500);
        ledBlue_blink();
    } else {
        Serial.println("Code reject looser");
        // tft.println("Code reject looser");
        buzzer_play_tone(3000, 500);
        ledGreen_blink();
    }

    delay(2000);
    resetInput();
    
}

void setup() {
    ledB_init();
    ledGreen_init();
    ledBlue_init();
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
        buzzer_play_tone(2500, 500);
        ledB_blink();
        resetInput();
        lastTime = currentTime;
        
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