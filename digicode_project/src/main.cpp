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
const byte LIGNES = 4; // Nombre de lignes
const byte COLONNES = 4; // Nombre de colonnes
// Tableau qui représente les touches du clavier
char touches[LIGNES][COLONNES] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}};

// Broches pour les lignes (en mode OUTPUT)
byte brochesLignes[LIGNES] = {2, 3, 4, 5};
// Broches pour les colonnes (en mode INPUT_PULLUP)
byte brochesColonnes[COLONNES] = {6, 7, 8, 9};
void setup()
{

  buzzer_init();
  // pinMode(TFT_BL, OUTPUT);
  // digitalWrite(TFT_BL, HIGH); // rétroéclairage allumé

  tft.begin();
  tft.fillScreen(ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(1);
  tft.setCursor(10, 10);
  // Initialisation de la communication série pour l'affichage
  Serial.begin(9600);
  Serial.println("Scanner de clavier 4x4 pret.");
  tft.println("yosh.");
  // Configuration des broches des lignes en sortie
  for (int i = 0; i < LIGNES; i++)
  {
    pinMode(brochesLignes[i], OUTPUT);
    digitalWrite(brochesLignes[i], HIGH); // On désactive toutes les lignes au départ
  }
  // Configuration des broches des colonnes en entrée avec résistance de rappel interne
  // Cela maintient les colonnes à l'état HAUT par défaut
  for (int i = 0; i < COLONNES; i++)
  {
    pinMode(brochesColonnes[i], INPUT_PULLUP);
  }
}
void loop()
{
  // On parcourt chaque ligne une par une
  for (int ligne = 0; ligne < LIGNES; ligne++)
  {
    // 1. On active la ligne actuelle en la mettant à l'état BAS
    // Une seule ligne est active (BAS) à la fois
    digitalWrite(brochesLignes[ligne], LOW);
    // 2. On vérifie toutes les colonnes pour cette ligne active
    for (int colonne = 0; colonne < COLONNES; colonne++)
    {
      // Si on lit un état BAS sur une colonne, cela veut dire qu'une touche est pressée
      // La touche pressée connecte la ligne (LOW) à la colonne (qui était HIGH grâce à PULLUP)
      if (digitalRead(brochesColonnes[colonne]) == LOW)
      {
        // On affiche la touche qui a été pressée
        Serial.print("Touche pressee : ");
        Serial.println(touches[ligne][colonne]);

        
        tft.print("touch pressed : ");
        tft.println(touches[ligne][colonne]);
        buzzer_play_tone(440, 100);

        // Petite pause pour l'anti-rebond (debounce) et éviter les lectures multiples
        delay(250);
        
        
      }
    }
    // 3. On désactive la ligne actuelle avant de passer à la suivante
    // C'est très important pour ne pas scanner plusieurs lignes en même temps
    digitalWrite(brochesLignes[ligne], HIGH);
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