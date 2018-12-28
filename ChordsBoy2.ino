#include <Arduboy2.h>
#include <ArduboyTones.h>
#include <Tinyfont.h>
#include "config.h"

Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Tinyfont tinyfont = Tinyfont(arduboy.sBuffer, Arduboy2::width(), Arduboy2::height());
// =========== fine parametri BOOT


                  char note4key ; //numero di note per scala.
char keyArray[13]; //con valori incrementati

const uint16_t allNotes[] PROGMEM = {262, 277, 294, 311,
                                     330, 349, 370, 392,
                                     415, 440, 466, 494
                                    };
//indice scala
int scalapos = 0; //da 0 a 23, 0..11 Maggiore, 12...23 minore
int fretpos = 0;


//contatore di inversione schermo
long blinktime;
//contatore funzioni speciali
long specialcount;
//stato di inversione
boolean statusInvert = false;
//Status (vedi funzione)
byte statusMode = 0;
//mute
boolean mute = false;

void setup() {
  arduboy.boot();
  //arduboy.begin();  //old mode
  Serial.begin(9600);
  initDisplay();

    setModeKeySong(scalapos);

  //audio
  sound.volumeMode(VOLUME_ALWAYS_HIGH);
  blinktime = millis();

}

void loop() {
  // put your main code here, to run repeatedly:
  arduboy.pollButtons();
  LeggiPulsanti();
  if (millis() - blinktime > 10000) {
    statusInvert = !statusInvert;
    arduboy.invert(statusInvert);
    blinktime = millis();
  }
}
