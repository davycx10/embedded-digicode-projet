#include <Arduino.h>

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "hardware/pio.h"




#define TFT_CS    17
#define TFT_DC    3
#define TFT_RST   6
#define TFT_BL    2



Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

#define NUM_LEDS 4
#define WS2812_PIN 1




#define BUZZER_PIN 27


void buzzer_init() {
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


// Définition des broches de l'Arduino connectées au clavier
const byte LIGNES = 4; // number of lignes
const byte COL = 4; // Number of columns
// Tableau qui représente les touches du clavier
char touches[LIGNES][COL] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
  };

// Pins for lines (in OUTPUT mode)
byte PinLignes[LIGNES] = {2, 3, 4, 5};
// Broches pour les colonnes (en mode INPUT_PULLUP)
byte PinCol[COL] = {6, 7, 8, 9};
void setup()
{
  // Initializing the buzzer
  buzzer_init();
  // pinMode(TFT_BL, OUTPUT);
  // digitalWrite(TFT_BL, HIGH); // rétroéclairage allumé

  // tft.begin();
  // tft.fillScreen(ILI9341_WHITE);
  // tft.setTextColor(ILI9341_BLACK);
  // tft.setTextSize(1);
  // tft.setCursor(10, 10);

  // Initializing serial communication for display
  Serial.begin(9600);
  Serial.println("4x4 keyboard scanner ready.");
  tft.println("yosh.");

  // Output line pin configuration
  for (int i = 0; i < LIGNES; i++)
  {
    pinMode(PinLignes[i], OUTPUT);
    digitalWrite(PinLignes[i], HIGH); // All lines are disabled at departure.
  }

  // Pin configuration of input columns with internal pull-up resistor
  // This keeps the columns in the HIGH state by default
  for (int i = 0; i < COL; i++)
  {
    pinMode(PinCol[i], INPUT_PULLUP);
  }
}
void loop()
{
  // On parcourt chaque ligne une par une
  for (int ligne = 0; ligne < LIGNES; ligne++)
  {
    // 1. Activate the current line by setting it to LOW.
    // Only one line can be active (LOW) at a time.
    digitalWrite(PinLignes[ligne], LOW);
    // 2. Check all columns for this active row
    for (int columns = 0; columns < COL; columns++)
    {
      // If a LOW state is read on a column, it means that a key is pressed.
      // The pressed key connects the row (LOW) to the column (which was HIGH thanks to PULLUP).
      if (digitalRead(PinCol[columns]) == LOW)
      {
        // On affiche la touche qui a été pressée
        Serial.print("Touche pressee : ");
        Serial.println(touches[ligne][columns]);

        

        buzzer_play_tone(440, 100);

        // Short pause for debounce and to avoid multiple readings
        delay(250);
        
        
      }
    }
    // 3. We disable the current line before moving on to the next one.
    // This is very important to avoid scanning multiple lines at the same time.
    digitalWrite(PinLignes[ligne], HIGH);
  }
}



// Le loop() recommence indéfiniment, scannant le clavier en continu




















// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}