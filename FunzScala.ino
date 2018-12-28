
void cambiaStatus(byte rif) {
  // 0-ACC: Accordi
  // 1-AUM: Accordi Aumentati
  // 2-DIM: Accordi Diminuiti
  // 3-INF: info
  String Testo;
  char* TestoFull[] = {"ACCAUMDIMINF"};
  Testo = TestoFull[0];
  statusMode = (statusMode + rif) % 4;
  arduboy.clear();
  //cambia header
  if ((statusMode >= 0) && (statusMode <= 2)) {
    //header
    arduboy.drawFastHLine(0, 9, WIDTH, 1 );
    arduboy.drawFastHLine(0, 22, WIDTH, 1 );
    tinyfont.setCursor(105, 5);
    tinyfont.print((mute == true ? "M" : " "));
    for (int i = 0; i < 3; i++) {
      arduboy.drawChar( 110 + (6 * i), 0, Testo.charAt((statusMode * 3) + i), BLACK, WHITE, 1);
    }
  }
  else if (statusMode == 3) {
    arduboy.drawRect( 0, 0, WIDTH, HEIGHT, 1 );
    arduboy.drawRect( 2, 2, WIDTH - 4, HEIGHT - 4, 1 );
  }

  //cambia display
  if  ((statusMode >= 0) && (statusMode <= 2)) {
    scalapos = 0;
    printAccordi() ;
  }
  else if (statusMode == 3) {
    printInfo();
  }
}


//===================== Mode 0
void printAccordi() {
  String scala = "";
  byte ripetizioni = (statusMode == 0 ? 5 : 3);
  int value;
  int aumodim = 0;
  String Testo;
  char* TestoFull[] = {"MAJMINDORFRILIDMISEOLLOCNAPNAMMIMMIAMAAORIUNGENI"};
  Testo = TestoFull[0];

  //Serial.println(scalapos);
  setModeKeySong( scalapos);
  // Serial.println(returnNote(keyArray[0]),0);
  //Head
  arduboy.setCursor(0, 0);
  arduboy.print(F("Key "));
  //  arduboy.print(padD(String(scalapos/12),2,"0"));
  for (int i = 0; i < 3; i++) {
    arduboy.drawChar( 24 + (6 * i), 0, Testo.charAt(((scalapos / 12) * 3) + i), WHITE, BLACK, 1);
  }
  arduboy.setCursor(44, 0);
  arduboy.print(F(": "));
  arduboy.print(returnNote(keyArray[0], 0));
  arduboy.print(F(" - "));
  arduboy.print(returnNote(keyArray[0], 1));
  arduboy.fillRect( 0, 42, WIDTH, 14, 0 );

  for (int j = 0; j < ripetizioni; j++)    {
    for (int i = 0; i < 7; i++)  {
      if ((j == 2) && (statusMode == 1)) aumodim = 1;
      else  if ((j == 2) && (statusMode == 2))  aumodim = -1;
      else aumodim = 0;
      arduboy.setCursor(1 + (i * 18), 13 + (10 * j) + (j > 0 ? 2 : 0));
      arduboy.print(returnNote(getAccordoPos(i , (j * 2) + aumodim) % 12, 0));
    }
  }
}

void playScalaMod0() {
  int nota;
  int firstnota =  pgm_read_word_near(&allNotes[keyArray[0] % 12]);
  int durata = 1000;
  if (mute == false ) arduboy.audio.on();
  for (int i = 0; i < 7 ; i++) {
    nota = pgm_read_word_near(&allNotes[keyArray[i] % 12]);
    if (nota < firstnota) nota = nota * 2;
    sound.tone(nota, durata);
    arduboy.drawRect(i * 3 * 6, 11, 6 * 3, 11, WHITE);
    arduboy.display();
    while (sound.playing()) {
    }
    //delay(4000);
    arduboy.drawRect(i * 3 * 6, 11, 6 * 3, 11, 0);
    arduboy.display();
  }
  arduboy.audio.off();
}


//===================== Mode 1
void printInfo() {
  arduboy.setCursor(8, 10);
  arduboy.setTextSize(2);
  arduboy.print(F("C"));
  arduboy.setTextSize(1);
  arduboy.print(F("hords"));
  arduboy.setTextSize(2);
  arduboy.print(F("B"));
  arduboy.setTextSize(1);
  arduboy.print(F("oy"));
  arduboy.setTextSize(1);
  arduboy.print(F(" 2.0"));
  arduboy.setCursor(8, 30);

  arduboy.print(F("Max "));
  arduboy.print(F("Scordamaglia "));
  arduboy.setCursor(8, 40);
  arduboy.print(F("maxscorda@gmail.com"));
  arduboy.setCursor(8, 50);

  arduboy.print(F("(c) 2018 "));
  arduboy.setTextSize(1);
}


