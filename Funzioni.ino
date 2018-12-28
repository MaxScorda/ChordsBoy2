void initDisplay() {
  // here we set the framerate to 30, we do not need to run at default 60 and
  // it saves us battery life.
  arduboy.setFrameRate(30);
  // ab.beginNoLogo();
  cambiaStatus(0);
  arduboy.display();
}

void LeggiPulsanti() {
  int static lastpressed = 0;   //valore ultimo tasto premuto
  boolean static cambio;
  int butpres = arduboy.buttonsState();
  byte oldscalapos;

  //DX-SX: cambia scala
  //UP-DW: cambia fret
  //A(sx): cambia funzioni/mute
  //B(dx): play

  //se tengo premuto per piu' di 3 secondi, mute on o off
  if ((butpres == lastpressed) && (butpres == A_BUTTON)) {
    if (millis() - specialcount >= 3000)  {
      mute = !mute;
      tinyfont.setCursor(105, 5);
      if (mute == 1) tinyfont.print("M");
      else arduboy.fillRect( 105, 5, 4, 4, 0 ); //avendo il background trasparente, cancello disegnando un quadrato nero
      arduboy.display();
      cambio = true;
      specialcount = millis();
    }
  }

  //se lascio A controlla se e' stato premuto per meno ddi 3 secondi percio' cambia status
  if (arduboy.justReleased(A_BUTTON)) {
    if ( (millis() - specialcount < 3000) && (cambio == false))  {
      cambiaStatus(1);
      lastpressed = butpres;
      arduboy.display();
    }
  }

  //tasto premuto
  if ((butpres != lastpressed) && (butpres != 0)) {
    //Serial.println(lastpressed);
    statusInvert = false;
    arduboy.invert(statusInvert);
    if (butpres == RIGHT_BUTTON ) {
      if  ((statusMode >= 0) && (statusMode <= 2))   {
        oldscalapos = scalapos;
        scalapos = ((scalapos / 12) * 12) + (scalapos + 1) % 12;
        printAccordi() ;
      }
      else if (statusMode == 3)   {
      }
    }

    if (butpres == LEFT_BUTTON) {
      if  ((statusMode >= 0) && (statusMode <= 2))   {
        oldscalapos = scalapos;
        scalapos = ((scalapos / 12) * 12) + (scalapos + 11) % 12;
        printAccordi() ;
      }
    }

    if (butpres == UP_BUTTON) {
      if  ((statusMode >= 0) && (statusMode <= 2))   {
        scalapos = (((scalapos / 12) + 1) % 16) * 12;
        printAccordi() ;
      }
      //   if ((statusMode == 1) || (statusMode == 2)) cambiafret(1);
    }

    if (butpres == DOWN_BUTTON) {
      if  ((statusMode >= 0) && (statusMode <= 2))   {
        scalapos = (((scalapos / 12) + 15) % 16) * 12;
        printAccordi() ;
      }
      //    if ((statusMode == 1) || (statusMode == 2)) cambiafret(-1);
    }

    if ( butpres == B_BUTTON) {
      if (statusMode == 0)   playScalaMod0();

    }

    if (butpres == A_BUTTON) {
      //se premo a per la prima volta fai partire il conteggio
      specialcount = millis();
      cambio = false;
    }

    lastpressed = butpres;
    blinktime = millis();
    arduboy.display();
  }
  else if (arduboy.buttonsState() == 0) {
    lastpressed = 0;
  }
}

//-----------------------------------------

void setModeKeySong(int keyset) {
  // setta la tonalita' generale del brano. !!
  // keyset=potvalue=0-11
  // Scala 0 0=Do, 1=Do#, 2=Re, 3=Re#,...,11=Si (non essemdoci minore e maggiore se non come riga del modale da scegliere)
  // Scala 1 12=Do, 13=Do#, ecc
  //ATTENZIONE! La matrice supera l'ottava percio' e' da gestire perche' e' buona a calcolare le giuste ottave in somma ma non sui confronti
  byte rigatono = keyset / 12; //riga matrice, in sostanza il tono 0=maggiore, 1=minore, 2 dorico, ecc...
  char baseinuse[12] ; //12 come le possibili note

  note4key = getScala(rigatono, baseinuse) ;
  keyset = keyset % 12;
  for (int i = 0; i < note4key; i++) {
    keyArray[i] = baseinuse[i] + keyset;
  }
  scalapos = (rigatono * 12) + keyset;

}

char getScala(byte posarray, char keyarray[]) {
  //compila la scala e ritorna il numero note
  // posarray=posizione vettore

  char ret = 0;
  byte readval;
  readval = pgm_read_byte(&scaleMod[posarray][0]);
  while ((readval != 99) && (ret < 12)) {
    keyarray[ret] = readval;
    ret++;
    readval = pgm_read_byte(&scaleMod[posarray][ret]);
  }
  return ret;
}

byte getAccordoPos(int posvettore, int intervallo) {
  /* ritorna il valore della nota (0..127) dato l'indice di basePat con lintervallo nella sua scala
    per calcolra gli accordi dove la triade non sono salti della chiave data ma della nota selezionate
    formula che calcola il valore assoluto dell'intervallo passato, somma il valore dell'intervallo cercato
    e aggiunge un'ottava nel caso di intervalli oltre la lunghezza della scala definita
  */
  return (keyArray[posvettore] + (keyArray[intervallo % note4key] - keyArray[0]) ) % 12 ;
}

String returnNote( int midival, bool tipo ) {
  //Tipo 0: Scala Anglosassone, 0: scala normale

  String score[] = {F("C"), F("Db"), F("D"), F("Eb"),
                    F("E"), F("F"), F("Gb"), F("G"),
                    F("Ab"), F("A"), F("Bb"), F("B")
                   };
  String scoreN[] = {F("Do"), F("Reb"), F("Re"), F("Mib"),
                     F("Mi"), F("Fa"), F("Sob"), F("Sol"),
                     F("Lab"), F("La"), F("Sib"), F("Si")
                    };
  String ret;
  if (tipo == 0) ret = padS(score[midival % 12], 2, " ");
  else  ret = padS(scoreN[midival % 12], 3, " ");
  return ret;
}


//-----------------------
String padS(String sstt, int lens, String charsub) {
  //pad a sx
  String spaces = "";
  for (int i = sstt.length(); i < lens; i++) spaces = spaces + charsub;
  return  sstt + spaces ;
}

String padD(String sstt, int lens, String charsub) {
  //pad a sx
  String spaces = "";
  for (int i = sstt.length(); i < lens; i++) spaces = spaces + charsub;
  return  spaces + sstt ;
}





