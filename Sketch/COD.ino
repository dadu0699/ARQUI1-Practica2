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
int velocidad = 0;
int estabilidad = 0;
int graduacion, referencia;
unsigned long tiempo;
int centro = 0;
int direccion = 0;


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

  if ((ps1 == LOW && ps2==HIGH && ps3 == LOW) || (ps1 == HIGH && ps2==HIGH && ps3 == HIGH)) {
    //VELOCIDAD
    analogWrite(PWML, 100);
    analogWrite(PWMR, 100);

    digitalWrite(LWF, HIGH);
    digitalWrite(LWB, LOW);

    digitalWrite(RWF, HIGH);
    digitalWrite(RWB, LOW);

    delay(300);
  }

}
