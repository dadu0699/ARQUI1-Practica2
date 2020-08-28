// Peripheral Configuration Code (do not edit)
//---CONFIG_BEGIN---
#pragma GCC push_options
#pragma GCC optimize ("Os")

#include <core.h> // Required by cpu
#include <cpu.h>
#include <Servo.h> // Required by T1:DRIVE
#include <Turtle.h>

#pragma GCC pop_options
#include "LedControl.h"

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

void setup() {
  peripheral_setup();

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
    delay(25);

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
      }
    }
    delay(25);
  }
}

void adelante() {
  //T1_DRIVE.drive(3,1,velocidad);
  T1_DRIVE.forwards(velocidad);

  analogWrite(Motor1, velocidad);
  analogWrite(Phase1, 0);
  analogWrite(Motor2, velocidad);
  digitalWrite(Phase2, 0);
}

void atras() {
  //T1_DRIVE.drive(3,2,velocidad);
  T1_DRIVE.backwards(velocidad);

  analogWrite(Motor1, 0);
  analogWrite(Phase1, velocidad);
  analogWrite(Motor2, 0);
  digitalWrite(Phase2, velocidad);
}

void derecha() {
  T1_DRIVE.drive(1, 1, velocidad * 0.75);
  T1_DRIVE.drive(2, 1, 0);

  analogWrite(Motor1, 0);
  analogWrite(Phase1, 0);
  analogWrite(Motor2, velocidad);
  digitalWrite(Phase2, 0);
}

void izquierda() {
  T1_DRIVE.drive(2, 1, velocidad * 0.75);
  T1_DRIVE.drive(1, 1, 0);

  analogWrite(Motor1, velocidad);
  analogWrite(Phase1, 0);
  analogWrite(Motor2, 0);
  digitalWrite(Phase2, 0);
}

void detener() {
  T1_DRIVE.stop();

  analogWrite(Motor1, 0);
  analogWrite(Phase1, 0);
  analogWrite(Motor2, 0);
  digitalWrite(Phase2, 0);
}

void giro360() {
  //T1_DRIVE.turn(velocidad);
  T1_DRIVE.drive(1, 1, velocidad);
  T1_DRIVE.drive(2, 2, velocidad);

  analogWrite(Motor2, velocidad);
  analogWrite(Phase2, 0);
  analogWrite(Motor1, 0);
  analogWrite(Phase1, velocidad);
}

void giro360Inverso() {
  //T1_DRIVE.turn(velocidad);
  T1_DRIVE.drive(2, 1, velocidad);
  T1_DRIVE.drive(1, 2, velocidad);

  analogWrite(Motor1, velocidad);
  analogWrite(Phase1, 0);
  analogWrite(Motor2, 0);
  analogWrite(Phase2, velocidad);
}

void esquivar() {
  detener();
}

 /* Matriz */
LedControl ledControl = LedControl(47, 48, 49, 1); // LedControl(DIN, CLK, CS / LOAD, # dispositivos)
byte matriz[8] {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};
int posX = 2;
int posY = 3;

void inicializarMatrizControlador() {
  ledControl.shutdown(0, false);
  ledControl.setIntensity(0, 15);
  ledControl.clearDisplay(0);
}

void recorridoMatriz(){
  for (int i = 0; i < 8; i++) {
    ledControl.setRow(0, i, matriz[i]);
  }
}
/*
    T1_DRIVE.drive(wheel,dir,speed);
    @Wheel: 1(Izquierda), 2(Derecha), 3(Ambas)
    @Dir: 1(Adelante), 2(Atras)
*/
