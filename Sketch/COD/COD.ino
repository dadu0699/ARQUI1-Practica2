#include "LedControl.h"
#include <Servo.h>

/*-------PINES---------*/
//SENSORES DE CAMINO
int PS1P = 13;
int PS2P = 12;
int PS3P = 11;
int ps1, ps2, ps3;
//PMW
int PWML = 10;
int PWMR = 9;
//SENSORES OBSTACULOS
int OS1 = 8;
int OS2 = 7;
int OS3 = 6;
//LLANTAS
int LWF = 5; //Izquierda al frente
int LWB = 4; //Izquierda atras
int RWF = 3; //Derecha al frente
int RWB = 2; //Derecha atras

// SERVO MOTOR
int PSVM = 35;
Servo motor;
int posicion = 0;

// SERVO TORTUGA
Servo tortuga;

/* Matriz */
LedControl ledControl = LedControl(47, 48, 49, 1); // LedControl(DIN, CLK, CS / LOAD, # dispositivos)
String matriz[8] {
  "00000000",
  "00000000",
  "00000000",
  "00000000",
  "00000000",
  "00000000",
  "00000000",
  "00000000"
};
int posX = 4;
int posY = 4;

/*------VARIABLES------*/
boolean camino = false;
int giros = 0;

int velocidad = 100;
int Motor1 = 50; //IZQ
int Motor2 = 52; //DER
int Phase1 = 51;
int Phase2 = 53;

void setup() {
  //Para el sensor Ultrasonico
  pinMode(OS1, INPUT);
  pinMode(OS2, INPUT);
  
  pinMode(OS3, OUTPUT);
  tortuga.attach(OS3);
  tortuga.write(90);

  //Sensores
  pinMode(PS1P, INPUT);
  pinMode(PS2P, INPUT);
  pinMode(PS3P, INPUT);
  //Llantas
  pinMode(LWF, OUTPUT);
  pinMode(LWB, OUTPUT);
  pinMode(RWF, OUTPUT);
  pinMode(RWB, OUTPUT);
  //PWM
  pinMode(PWML, OUTPUT);
  pinMode(PWMR, OUTPUT);

  pinMode(PSVM, OUTPUT);
  motor.attach(PSVM);

  //Matriz
  inicializarMatrizControlador();

  // VARIABLES DE MOTORES
  pinMode(Motor1, OUTPUT);
  pinMode(Motor2, OUTPUT);
  pinMode(Phase1, OUTPUT);
  pinMode(Phase2, OUTPUT);
  //**********************
}

void loop() {
  ps1 = digitalRead(PS1P);
  ps2 = digitalRead(PS2P);
  ps3 = digitalRead(PS3P);

  if ((ps1 == HIGH && ps2 == HIGH && ps3 == HIGH) || (ps1 == LOW && ps2 == HIGH && ps3 == LOW)) {
    camino = true;

    posY--;
    encender(posX, posY);

    adelante();
  } else if (ps1 == HIGH && ps2 == HIGH && ps3 == LOW) {
    camino = true;
    izquierda(1);

    posX--;
    encender(posX, posY);

    adelante();
  } else if (ps1 == HIGH && ps2 == LOW && ps3 == LOW) {
    camino = true;
  } else if (ps1 == LOW && ps2 == HIGH && ps3 == HIGH) {
    camino = true;
    derecha(1);

    posX++;
    encender(posX, posY);

    adelante();
  } else if (ps1 == LOW && ps2 == LOW && ps3 == HIGH) {
    camino = true;
  } else if (ps1 == LOW && ps2 == LOW && ps3 == LOW) {
    if (camino) {
      camino = false;
      atras();
      delay(200);
      detener();
      delay(1100);
      if (giros == 0) {
        // Girar 90
        derecha(2100);
        detener();
        delay(245);

        adelante();
        delay(500);
        ps2 = digitalRead(PS2P);
        detener();
        delay(1000);
        if ( ps2 == HIGH) {
          giros = 0;
          camino = true;
        } else {
          atras();
          delay(500);
          giros = 1;
          atras();
          delay(35);
        }
      } else {
        // Girar 180
        izquierda(4100);
        detener();
        delay(245);
        giros = 0;
      }
      adelante();
    }
  }
  recorridoMatriz();
}

void adelante() {
  analogWrite(PWML, velocidad);
  analogWrite(PWMR, velocidad);

  digitalWrite(LWF, HIGH);
  digitalWrite(LWB, LOW);

  digitalWrite(RWF, HIGH);
  digitalWrite(RWB, LOW);

  digitalWrite(Motor1, velocidad);
  digitalWrite(Phase1, 0);
  digitalWrite(Motor2, velocidad);
  digitalWrite(Phase2, 0);
}

void atras() {
  analogWrite(PWML, velocidad);
  analogWrite(PWMR, velocidad);

  digitalWrite(LWF, LOW);
  digitalWrite(LWB, HIGH);

  digitalWrite(RWF, LOW);
  digitalWrite(RWB, HIGH);

  analogWrite(Motor1, 0);
  analogWrite(Phase1, velocidad);
  analogWrite(Motor2, 0);
  digitalWrite(Phase2, velocidad);
}

void detener() {
  digitalWrite(LWF, LOW);
  digitalWrite(LWB, LOW);

  digitalWrite(RWF, LOW);
  digitalWrite(RWB, LOW);

  digitalWrite(Motor1, 0);
  digitalWrite(Phase1, 0);
  digitalWrite(Motor2, 0);
  digitalWrite(Phase2, 0);
}

void derecha(int retraso) {
  analogWrite(PWML, velocidad / 2);
  analogWrite(PWMR, velocidad / 2);

  digitalWrite(LWF, HIGH);
  digitalWrite(LWB, LOW);

  digitalWrite(RWF, LOW);
  digitalWrite(RWB, HIGH);

  digitalWrite(Motor1, 0);
  digitalWrite(Phase1, 0);
  digitalWrite(Motor2, velocidad / 2);
  digitalWrite(Phase2, 0);
  delay(retraso);
}

void izquierda(int retraso) {
  analogWrite(PWML, velocidad / 2);
  analogWrite(PWMR, velocidad / 2);

  digitalWrite(LWF, LOW);
  digitalWrite(LWB, HIGH);

  digitalWrite(RWF, HIGH);
  digitalWrite(RWB, LOW);

  digitalWrite(Motor1, velocidad / 2);
  digitalWrite(Phase1, 0);
  digitalWrite(Motor2, 0);
  digitalWrite(Phase2, 0);
  delay(retraso);
}

void inicializarMatrizControlador() {
  ledControl.shutdown(0, false);
  ledControl.setIntensity(0, 15);
  ledControl.clearDisplay(0);
}

void encender(int posAX, int posAY) {
  if (posAX > 7 || posAY > 7 || posAX < 0 || posAY < 0) {
    posX = 4;
    posY = 4;
    posAX = posX;
    posAY = posY;

    for (int i = 0; i < 8; i++) {
      matriz[i] = "00000000";
    }
    ledControl.clearDisplay(0);
    delay(100);
  }

  String cadena = matriz[posY];
  String parte1 = "";
  String parte2 = "";

  for (int i = 0; i < posAX; i++) {
    parte1 += matriz[posAY].charAt(i);
  }

  for (int i = posX + 1; i < 8; i++) {
    parte2 += matriz[posAY].charAt(i);
  }

  matriz[posAY] = parte1 + "1" + parte2;
}

void recorridoMatriz() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if (matriz[i].charAt(j) == '1') {
        ledControl.setLed(0, i, j, true);
      } else {
        ledControl.setLed(0, i, j, false);
      }
    }
  }
  delay(100);
}

void barredora3vueltas() {
  for (posicion = 1; posicion <= 1080; posicion++) {
    motor.write(posicion);
    delay(15);
  }
  delay(100);
}
