#include "LedControl.h"
#include <Servo.h>
#include <Stepper.h>

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
int objeto = 31;//Led Amarilla

// SERVO TORTUGA
Servo tortuga;

// Stepper
#define PASOS 200
Stepper stepperIZQ(PASOS, 50, 51, 52, 53);
Stepper stepperDCHO(PASOS, 22, 23, 24, 25);

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

/*------VARIABLES------*/
boolean camino = false;
int giros = 0;

int velocidad = 100;
int Motor1 = 50;  //IZQ
int Motor2 = 52; //DER
int Phase1 = 51;
int Phase2 = 53;

void setup() {
  Serial.begin(9600);
  Serial.println(">> UNIVERSIDAD DE SAN CARLOS DE GUATEMALA");
  Serial.println(">> I N G E N I E R I A");
  Serial.println(">> A R Q U I   1");
  Serial.println(">> G R U P O  3");
  Serial.println("\n");
  //Para el sensor Ultrasonico
  pinMode(OS2, INPUT);//Trigger
  pinMode(OS1, OUTPUT);//Echo
  digitalWrite(OS1, LOW);

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

  //Matriz
  inicializarMatrizControlador();

  // VARIABLES DE MOTORES
  pinMode(Motor1, OUTPUT);
  pinMode(Motor2, OUTPUT);
  pinMode(Phase1, OUTPUT);
  pinMode(Phase2, OUTPUT);
  //**********************

  stepperIZQ.setSpeed(velocidad);
  stepperDCHO.setSpeed(velocidad);

  //Servo
  pinMode(PSVM, OUTPUT);
  pinMode(objeto, OUTPUT);
}

void loop() {
  long t;
  long sus;
  long auxT;
  long d;
  int contar = 0;
  boolean seguir = true;
  boolean obstaculo = true;
  detener();
  digitalWrite(OS1, HIGH);
  delayMicroseconds(10);
  digitalWrite(OS1, LOW);

  t = pulseIn(OS2, HIGH);
  d = t / 59;
  /*Serial.println("Original");
  Serial.println(t);*/
  if (d > 0) {

    while (t > 60) {
      detener();
      digitalWrite(OS1, HIGH);
      delayMicroseconds(10);
      digitalWrite(OS1, LOW);
      auxT = pulseIn(OS2, HIGH);
      if (auxT == 0) {
        contar++;
      }
      if (auxT != 0) {
        t = auxT;
      }
     /* Serial.println("Auxiliar");
      Serial.println(auxT);
      Serial.println("Tiempo");
      Serial.println(t);*/
      avanzar();
      if (contar > 1) {
        obstaculo = false;
        break;
        contar = 0;
      }
    }
    if (obstaculo) {
      esquivar();
    }
  } else {
    avanzar();
  }
}
void avanzar() {
  ps1 = digitalRead(PS1P);
  ps2 = digitalRead(PS2P);
  ps3 = digitalRead(PS3P);

  // SENSOR DE OBJETOS
  int sensorValue = analogRead(A0);
  if (sensorValue >= 36 && sensorValue <= 42) { //Amarillo
    Serial.println("\nO B J E T O\n");
    digitalWrite(31, HIGH);
    barredora3vueltas();
  } else {
    digitalWrite(31, LOW);
  }


  if ((ps1 == HIGH && ps2 == HIGH && ps3 == HIGH) || (ps1 == LOW && ps2 == HIGH && ps3 == LOW)
      || (ps1 == HIGH && ps2 == LOW && ps3 == HIGH)) {
    camino = true;
    stepperIZQ.step(1);
    stepperDCHO.step(1);
    adelante();

  } else if (ps1 == HIGH && ps2 == HIGH && ps3 == LOW) {
    camino = true;
    stepperDCHO.step(1);
    izquierda(12);

    stepperIZQ.step(1);
    stepperDCHO.step(1);
    adelante();

  } else if (ps1 == HIGH && ps2 == LOW && ps3 == LOW) {
    camino = true;
    izquierda(17);
    adelante();

  } else if (ps1 == LOW && ps2 == HIGH && ps3 == HIGH) {
    camino = true;
    stepperIZQ.step(1);
    derecha(12);

    stepperIZQ.step(1);
    stepperDCHO.step(1);
    adelante();

  } else if (ps1 == LOW && ps2 == LOW && ps3 == HIGH) {
    camino = true;
    derecha(17);
    adelante();

  } else if (ps1 == LOW && ps2 == LOW && ps3 == LOW) {
    if (camino) {
      camino = false;
      stepperIZQ.step(-1);
      stepperDCHO.step(-1);
      atras();
      delay(200);

      detener();
      delay(1100);
      if (giros == 0) {
        // Girar 90
        stepperIZQ.step(1);
        derecha(2100);

        detener();
        delay(245);

        stepperIZQ.step(1);
        stepperDCHO.step(1);
        adelante();
        delay(400);

        ps2 = digitalRead(PS2P);
        detener();
        delay(1000);

        if ( ps2 == HIGH) {
          giros = 0;
          camino = true;

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
          stepperIZQ.step(-1);
          stepperDCHO.step(-1);
          atras();
          delay(500);

          giros = 1;

          stepperIZQ.step(-1);
          stepperDCHO.step(-1);
          atras();
          delay(35);
        }
      } else {
        // Girar 180
        izquierda(4100);

        detener();
        stepperDCHO.step(1);
        delay(100);
        stepperDCHO.step(1);
        delay(100);

        detener();
        delay(245);
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
      stepperIZQ.step(1);
      stepperDCHO.step(1);
      adelante();
    }
  }
  recorridoMatriz();
}

/*--------------------------  M E T O D O S    DE    M O V I M I E N T O  --------------------------*/

void esquivar() {
  Serial.println("E S Q U I V A N D O   O B J E T O");
  derecha(2100);
  detener();
  adelante();
  delay(2000);
  izquierda(2100);
  detener();
  adelante();
  delay(3500);
  izquierda(2100);
  detener();
  adelante();
  delay(2000);
  derecha(2100);
  detener();
  Serial.println("L I S T O!");
  Serial.println("\n");
}
void adelante() {
  analogWrite(PWML, velocidad);
  analogWrite(PWMR, velocidad);

  digitalWrite(LWF, HIGH);
  digitalWrite(LWB, LOW);

  digitalWrite(RWF, HIGH);
  digitalWrite(RWB, LOW);

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
  analogWrite(PWML, velocidad);
  analogWrite(PWMR, velocidad);

  digitalWrite(LWF, LOW);
  digitalWrite(LWB, HIGH);

  digitalWrite(RWF, LOW);
  digitalWrite(RWB, HIGH);
}

void detener() {
  digitalWrite(LWF, LOW);
  digitalWrite(LWB, LOW);

  digitalWrite(RWF, LOW);
  digitalWrite(RWB, LOW);
}

void derecha(int retraso) {
  analogWrite(PWML, velocidad / 2);
  analogWrite(PWMR, velocidad / 2);

  digitalWrite(LWF, HIGH);
  digitalWrite(LWB, LOW);

  digitalWrite(RWF, LOW);
  digitalWrite(RWB, HIGH);
  delay(retraso);
}

void izquierda(int retraso) {
  analogWrite(PWML, velocidad / 2);
  analogWrite(PWMR, velocidad / 2);

  digitalWrite(LWF, LOW);
  digitalWrite(LWB, HIGH);

  digitalWrite(RWF, HIGH);
  digitalWrite(RWB, LOW);
  delay(retraso);
}

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

void objetoMatriz(int posAX, int posAY) {
  String cadena = matriz[posY];
  String parte1 = "";
  String parte2 = "";

  for (int i = 0; i < posAX; i++) {
    parte1 += matriz[posAY].charAt(i);
  }

  for (int i = posX + 1; i < 8; i++) {
    parte2 += matriz[posAY].charAt(i);
  }

  matriz[posAY] = parte1 + "2" + parte2;
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


/*--------------------------  B A R R E D O R A  --------------------------*/
void barredora3vueltas() {
  Serial.println("A  B A R R E R!");
  Serial.println("- - - - -");
  motor.attach(PSVM);
  for (posicion = 1; posicion <= 977; posicion++) {
    motor.write(posicion);
    delay(15);
  }
  //motor.attach(PSVM);
  Serial.println("L I S T O!");
  Serial.println("\n");
}
