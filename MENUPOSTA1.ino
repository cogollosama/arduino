#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <Wire.h>
#define DHT11_PIN 2
dht DHT;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
int sensorhumedad = A0;
int valorsensor;

//Input & Button Logic

int BOTONOK = 13;
int ESTADOBOTONOK;
int ULTIMOESTADOBOTONOK = {LOW};
bool BANDERAOK = {LOW};
long ULTIMOTIEMPODELAYOK = (0);
long DELAYOK = 5;                     //BOTON OK

const int CANTIDADBOTONES = 4;
const int PINESDEBOTONES[CANTIDADBOTONES] = {9, 10, 11, 12};
int ESTADOBOTON[CANTIDADBOTONES];
int ULTIMOESTADOBOTON[CANTIDADBOTONES] = {LOW, LOW, LOW, LOW};
bool BANDERAS[CANTIDADBOTONES] = {LOW, LOW, LOW, LOW};
long ULTIMOTIEMPODELAY[CANTIDADBOTONES] = {0, 0, 0, 0};
long DELAYBOTONES = 5;              // BOTONES ARRIBA ABAJO IZQ DER

//LCD Menu Logic
const int CANTIDADPANTALLAS = 4;
int PANTALLAACTUAL = 0;
String PANTALLAS[CANTIDADPANTALLAS][2] = {{"Cantidad Macetas", "Unidades"}, {"Capacidad Maceta", "Lts"},
  {"Cuando Regar:", "% Humedad"}, {"Cuanto Regar:", "Lts"}
};
int VALORES[CANTIDADPANTALLAS];

void setup() {

  for (int i = 0; i < CANTIDADBOTONES; i++) {
    pinMode(PINESDEBOTONES[i], INPUT);
    digitalWrite(PINESDEBOTONES[i], HIGH); // pull-up 20k NO NECESITO LAS RESISTENCIAS DE 10K QUE PUSE YA QUE ACA TENESSSSSSS
    pinMode(BOTONOK, INPUT);
    digitalWrite(BOTONOK, HIGH); //SETPIN AND CURRENT DEL BOTONOK
  }

  lcd.begin(16, 2);
  lcd.print("   Soluciones");
  lcd.setCursor(0, 2);
  lcd.print("   Fumonas");
}

void loop() {
  setInputFlags();
  setInputFlagsOK();
  resolveInputFlags();
  resolveInputFlagsOK();
}

void setInputFlags() {
  for (int i = 0; i < CANTIDADBOTONES; i++) {
    int LEYENDO = digitalRead(PINESDEBOTONES[i]);
    if (LEYENDO != ULTIMOESTADOBOTON[i]) {
      ULTIMOTIEMPODELAY[i] = millis();
    }
    if ((millis() - ULTIMOTIEMPODELAY[i]) > DELAYBOTONES) {
      if (LEYENDO != ESTADOBOTON[i]) {
        ESTADOBOTON[i] = LEYENDO;
        if (ESTADOBOTON[i] == HIGH) {
          BANDERAS[i] = HIGH;
        }
      }
    }
    ULTIMOESTADOBOTON[i] = LEYENDO;
  }
}

void resolveInputFlags() {
  for (int i = 0; i < CANTIDADBOTONES; i++) {
    if (BANDERAS[i] == HIGH) {
      ACCIONBOTONES(i);
      BANDERAS[i] = LOW;
      printScreen();
    }
  }
}

void ACCIONBOTONES(int input) {
  if (input == 0) {
    if (PANTALLAACTUAL == 0) {
      PANTALLAACTUAL = CANTIDADPANTALLAS - 1;
    } else {
      PANTALLAACTUAL--;
    }
  } else if (input == 1) {
    if (PANTALLAACTUAL == CANTIDADPANTALLAS - 1) {
      PANTALLAACTUAL = 0;
    } else {
      PANTALLAACTUAL++;
    }
  } else if (input == 2) {
    FUNCIONVALORES(0);
  } else if (input == 3) {
    FUNCIONVALORES(1);
  }
}

void FUNCIONVALORES(int key) {
  if (key == 0) {
    VALORES[PANTALLAACTUAL]++;
  } else if (key == 1) {
    VALORES[PANTALLAACTUAL]--;
  }
}
void printScreen() {
  lcd.clear();
  lcd.print(PANTALLAS[PANTALLAACTUAL][0]);
  lcd.setCursor(0, 1);
  lcd.print(VALORES[PANTALLAACTUAL]);
  lcd.print(" ");
  lcd.print(PANTALLAS[PANTALLAACTUAL][1]);
}

void setInputFlagsOK() {

  int LEYENDOOK = digitalRead(BOTONOK);
  if (LEYENDOOK != ULTIMOESTADOBOTONOK) {
    ULTIMOTIEMPODELAYOK = millis();
  }
  if ((millis() - ULTIMOTIEMPODELAYOK) > DELAYOK) {
    if (LEYENDOOK != ESTADOBOTONOK) {
      ESTADOBOTONOK = LEYENDOOK;
      if (ESTADOBOTONOK == HIGH) {
        BANDERAOK = HIGH;
      }
    }
  }
  ULTIMOESTADOBOTONOK = LEYENDOOK;
}

void resolveInputFlagsOK() {
  for (int i = 0; i < BOTONOK; i++) {
    if (BANDERAOK == HIGH) {
      for (int x = 0; x < 9999999; x++)
        printScreenOK();
      BANDERAOK = LOW;
    }
  }
}
void printScreenOK() {
  lcd.clear();
  valorsensor = analogRead(sensorhumedad);
  valorsensor = map(valorsensor, 1050, 250, 0, 100); 
              
  PANTALLAACTUAL = 0;                                      
  unsigned long CANTIDADMACETAS = VALORES[PANTALLAACTUAL];
  
  PANTALLAACTUAL = 1;                                      
  unsigned long CAPACIDAD = VALORES[PANTALLAACTUAL] ;  
  
  PANTALLAACTUAL = 2;
  unsigned long HUMEDADREGAR = VALORES[PANTALLAACTUAL];
  
  PANTALLAACTUAL = 3;
  unsigned long CANTIDADAGUA = VALORES[PANTALLAACTUAL];

  unsigned long tempo = (((CAPACIDAD / 10) * CANTIDADMACETAS) * CANTIDADAGUA);
  unsigned long DELAY = 1000;

  
  if (valorsensor <= HUMEDADREGAR) {
    digitalWrite(3, HIGH);
    delay(tempo * DELAY);
    digitalWrite(3, LOW);
  }                                       

  lcd.setCursor(0, 0);
  lcd.print(" Humedad:  ");
  lcd.print(valorsensor);
  lcd.println("%   ");
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0, 1);
  lcd.print(" Temp:");
  lcd.print(DHT.temperature, 0);
  lcd.print((char)223);
  lcd.print(" H:");
  lcd.print(DHT.humidity, 0);
  lcd.print("%");
  delay(4000);


}
