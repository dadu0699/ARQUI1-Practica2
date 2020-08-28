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

/*------VARIABLES------*/
boolean camino = false;
int giros = 0;
void setup() {
  //Para el sensor Ultrasonico
  pinMode(OS1, INPUT);
  pinMode(OS2, INPUT);
  pinMode(OS3, INPUT);
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

}

void loop() {
  ps1 = digitalRead(PS1P);
  ps2 = digitalRead(PS2P);
  ps3 = digitalRead(PS3P);

  if ((ps1 == HIGH && ps2 == HIGH && ps3 == HIGH) || (ps1 == LOW && ps2 == HIGH && ps3 == LOW)) {
    camino = true;
    adelante();
  } else if (ps1 == HIGH && ps2 == HIGH && ps3 == LOW) {
    camino = true;
    izquierda(1);
    adelante();
  } else if (ps1 == HIGH && ps2 == LOW && ps3 == LOW) {
    camino = true;
  } else if (ps1 == LOW && ps2 == HIGH && ps3 == HIGH) {
    camino = true;
    derecha(1);
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

}

void adelante() {
  analogWrite(PWML, 100);
  analogWrite(PWMR, 100);

  digitalWrite(LWF, HIGH);
  digitalWrite(LWB, LOW);

  digitalWrite(RWF, HIGH);
  digitalWrite(RWB, LOW);
}

void atras() {
  analogWrite(PWML, 100);
  analogWrite(PWMR, 100);

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
  analogWrite(PWML, 50);
  analogWrite(PWMR, 50);

  digitalWrite(LWF, HIGH);
  digitalWrite(LWB, LOW);

  digitalWrite(RWF, LOW);
  digitalWrite(RWB, HIGH);

  delay(retraso);
}

void izquierda(int retraso) {
  analogWrite(PWML, 50);
  analogWrite(PWMR, 50);

  digitalWrite(LWF, LOW);
  digitalWrite(LWB, HIGH);

  digitalWrite(RWF, HIGH);
  digitalWrite(RWB, LOW);

  delay(retraso);
}
