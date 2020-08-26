// Peripheral Configuration Code (do not edit)
//---CONFIG_BEGIN---
#pragma GCC push_options
#pragma GCC optimize ("Os")

#include <core.h> // Required by cpu
#include <cpu.h>
#include <Servo.h> // Required by T1:DRIVE
#include <Turtle.h>

#pragma GCC pop_options

// Peripheral Constructors
CPU &cpu = Cpu;
TurtleDrive T1_DRIVE = TurtleDrive (2, 4, 3, 6, 7, 5);
TurtleSonarHead T1_SH = TurtleSonarHead (8, 9, 10);
TurtleLineHunter T1_LH = TurtleLineHunter (11, 12, A0);

void peripheral_setup () {
  T1_DRIVE.begin ();
  T1_SH.begin ();
  T1_LH.begin ();
}

void peripheral_loop() {
}
//---CONFIG_END---

// Configuraciones Propias
int velocidad = 100;

void setup () {
  peripheral_setup();

  T1_DRIVE.forwards(velocidad);
}


void loop() {
  peripheral_loop();

  T1_DRIVE.stop();

  if (T1_LH(0, 0, 0)) {
    T1_DRIVE.forwards(velocidad);
  } else if (T1_LH(1, 1, 1)) {
    T1_DRIVE.forwards(velocidad);
  }

  // DERECHA
  if (T1_LH(0, 0, 1)) {
    T1_DRIVE.drive(1, 1, velocidad);
  } else if (T1_LH(0, 1, 1)) {
    T1_DRIVE.drive(1, 1, velocidad);
  }

  // IZQUIERDA
  if (T1_LH(1, 0, 0)) {
    T1_DRIVE.drive(2, 1, velocidad);
  } else if (T1_LH(1, 1, 0)) {
    T1_DRIVE.drive(2, 1, velocidad);
  }

  if (T1_LH(0, 1, 0)) {
  } else if (T1_LH(1, 0, 1)) {
  }
}
/*
    T1_DRIVE.drive(wheel,dir,speed);
    @Wheel: 1(Izquierda), 2(Derecha), 3(Ambas)
    @Dir: 1(Adelante), 2(Atras)
*/
