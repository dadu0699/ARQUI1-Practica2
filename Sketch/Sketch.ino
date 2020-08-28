// Peripheral Configuration Code (do not edit)
//---CONFIG_BEGIN---
#pragma GCC push_options
#pragma GCC optimize ("Os")

#include <core.h> // Required by cpu
#include <cpu.h>
#include <Servo.h> // Required by T1:DRIVE
#include <Turtle.h>
#include "LedControl.h"

#pragma GCC pop_options

// Peripheral Constructors
CPU &cpu = Cpu;
TurtleDrive T1_DRIVE = TurtleDrive (2, 4, 3, 6, 7, 5);
TurtleSonarHead T1_SH = TurtleSonarHead (8, 9, 10);
TurtleLineHunter T1_LH = TurtleLineHunter (11, 12, A0);

void peripheral_setup () {
  T1_DRIVE.begin();
  T1_SH.begin();
  T1_LH.begin();
}

void peripheral_loop() {
}
//---CONFIG_END---

// Configuraciones Propias
int velocidad = 100;
int Motor1 = 50; //IZQ
int Motor2 = 52; //DER
int Phase1 = 51;
int Phase2 = 53;

// SERVO MOTOR
int PSVM = 35;
Servo motor;
int posicion = 0;

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
int posX = 2;
int posY = 6;
boolean ziczacVH = true; // Vertical = true, Abajo = false;
boolean ladoID = true;  // Izquierda = true, Derecha = false;
boolean ladoAB = true; // Arriba = true, Abajo = false;
int conteoCamino = 0;

void setup() {
  peripheral_setup();

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

boolean camino = false;
int giros = 0;

void loop() {
  peripheral_loop();

  if (T1_LH(1, 1, 1) || T1_LH(0, 1, 0)) {
    camino = true;
    adelante();

  } else if (T1_LH(1, 1, 0)) {
    izquierda();
    delay(5);
    derecha();
    delay(2);
  } else if (T1_LH(1, 0, 0)) {
    izquierda();
    delay(10);
    derecha();
    delay(5);

  } else if (T1_LH(0, 1, 1)) {
    derecha();
    delay(5);
    izquierda();
    delay(2);

  } else if (T1_LH(0, 0, 1)) {
    derecha();
    delay(10);
    izquierda();
    delay(5);

  } else if (T1_LH(0, 0, 0)) {
    if (camino) {
      atras();
      delay(243);
      detener();
      delay(1100);

      if (giros == 0) {
        // Girar 90
        giro360();
        delay(1000);
        detener();

        adelante();
        delay(35);
        if (T1_LH(1, 1, 1) || T1_LH(0, 1, 0)) {
          giros = 0;
          if (ziczacVH) {
            if (ladoAB) {
              ladoID = false;
            } else {
              ladoID = true;
            }
          } else {
            if (ladoID) {
              ladoAB = true;
            } else {
              ladoAB = false;
            }
          }
          ziczacVH = !ziczacVH;
        } else {
          giros = 1;
          atras();
          delay(35);
        }

      } else {
        // Girar 90
        giro360Inverso();
        delay(2000);
        detener();
        giros = 0;

        if (ziczacVH) {
          if (ladoAB) {
            ladoID = true;
          } else {
            ladoID = false;
          }
        } else {
          if (ladoID) {
            ladoAB = false;
          } else {
            ladoAB = true;
          }
        }
        ziczacVH = !ziczacVH;
      }
    }
    delay(25);
  }
  recorridoMatriz();
}

void adelante() {
  //T1_DRIVE.drive(3,1,velocidad);
  T1_DRIVE.forwards(velocidad);

  digitalWrite(Motor1, velocidad);
  digitalWrite(Phase1, 0);
  digitalWrite(Motor2, velocidad);
  digitalWrite(Phase2, 0);

  conteoCamino++;
  if (conteoCamino == 8) {
    if (ziczacVH) {
      if (ladoAB) {
        posY--;
      } else {
        posY++;
      }
    } else {
      if (ladoID) {
        posX--;
      } else {
        posX++;
      }
    }
    encender(posX, posY);
    conteoCamino = 0;
  }
}

void atras() {
  //T1_DRIVE.drive(3,2,velocidad);
  T1_DRIVE.backwards(velocidad);

  digitalWrite(Motor1, 0);
  digitalWrite(Phase1, velocidad);
  digitalWrite(Motor2, 0);
  digitalWrite(Phase2, velocidad);
}

void derecha() {
  T1_DRIVE.drive(1, 1, velocidad * 0.75);
  T1_DRIVE.drive(2, 1, 0);

  digitalWrite(Motor1, 0);
  digitalWrite(Phase1, 0);
  digitalWrite(Motor2, velocidad);
  digitalWrite(Phase2, 0);
}

void izquierda() {
  T1_DRIVE.drive(2, 1, velocidad * 0.75);
  T1_DRIVE.drive(1, 1, 0);

  digitalWrite(Motor1, velocidad);
  digitalWrite(Phase1, 0);
  digitalWrite(Motor2, 0);
  digitalWrite(Phase2, 0);
}

void detener() {
  T1_DRIVE.stop();

  digitalWrite(Motor1, 0);
  digitalWrite(Phase1, 0);
  digitalWrite(Motor2, 0);
  digitalWrite(Phase2, 0);
}

void giro360() {
  //T1_DRIVE.turn(velocidad);
  T1_DRIVE.drive(1, 1, velocidad);
  T1_DRIVE.drive(2, 2, velocidad);

  digitalWrite(Motor2, velocidad);
  digitalWrite(Phase2, 0);
  digitalWrite(Motor1, 0);
  digitalWrite(Phase1, velocidad);
}

void giro360Inverso() {
  //T1_DRIVE.turn(velocidad);
  T1_DRIVE.drive(2, 1, velocidad);
  T1_DRIVE.drive(1, 2, velocidad);

  digitalWrite(Motor1, velocidad);
  digitalWrite(Phase1, 0);
  digitalWrite(Motor2, 0);
  digitalWrite(Phase2, velocidad);
}

void esquivar() {
  detener();
}
/*
    T1_DRIVE.drive(wheel,dir,speed);
    @Wheel: 1(Izquierda), 2(Derecha), 3(Ambas)
    @Dir: 1(Adelante), 2(Atras)
*/

void inicializarMatrizControlador() {
  ledControl.shutdown(0, false);
  ledControl.setIntensity(0, 15);
  ledControl.clearDisplay(0);
}

void encender(int posAX, int posAY) {
  if (posAX > 7 || posAY > 7 || posAX < 0 || posAY < 0) {
    posX = 3;
    posY = 3;
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
