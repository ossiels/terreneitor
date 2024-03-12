#include <AFMotor.h>
#include <SoftwareSerial.h>
#include <EasyBuzzer.h>

SoftwareSerial BT(9, 10);

const char MODO_AUTO = 'w';
const char MODO_CONTROL = 'W';
const char ADELANTE = 'F';
const char ATRAS = 'B';
const char IZQ = 'L';
const char DER = 'R';
const char PARAR = 'S';

const char ESI = 'G';
const char EII = 'H';
const char ESD = 'I';
const char EID = 'J';

const int Trigger1 = 14;
const int Echo1 = 15;

char instruccion;

long distancia;

/*
const int Trigger2 = 4;
const int Echo2 = 5;

const int buzzer = 10;

const unsigned int frequency = 1000;
const int beeps = 10;
*/

// Conexiones del Motor A
AF_DCMotor motor1(1); //pin 11

// Conexiones del Motor B
AF_DCMotor motor2(2); //pin 3

AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

void setup() {
  instruccion = PARAR;
  distancia = 0;
	BT.begin(9600);
	Serial.begin(9600);
  cambiarVelocidad(50);

  pinMode(Trigger1, OUTPUT); //pin como salida
  pinMode(Echo1, INPUT);  //pin como entrada
  digitalWrite(Trigger1, LOW);//Inicializamos el pin con 0
}

void loop() {
  modoControl();
  modoAuto(); 
}

char leerBluetooth() {
	if (BT.available()) {
		instruccion = BT.read();
		Serial.write(instruccion);
	}
	if (Serial.available()) {
		BT.write(Serial.read());
	}
}

void modoControl() {
	//Serial.println("\nModo control :)");
  while (instruccion != MODO_AUTO) {
  leerBluetooth();
    
    if (instruccion == PARAR) {
      parar();
    }
    if (instruccion == ADELANTE) {
      avanzar();
    }
    if (instruccion == ATRAS) {
      retroceder();
    }
    if (instruccion == IZQ) {
      irIzq();
    }
    if (instruccion == DER) {
      irDer();
    }
    if (instruccion == ESI){
      esqSupIzq();
    }
    if (instruccion == EID) {
      esqInfDer();
    }
    if (instruccion == ESD) {
      esqSupDer();
    }
    if (instruccion == EID) {
      esqInfDer();
    }
    int velocidad = checkVelocidad();
    cambiarVelocidad(velocidad);
  }
}

void modoAuto() {
  cambiarVelocidad(60);
  //Serial.println("\nModo auto :)");
  while (instruccion != MODO_CONTROL) {
    leerDistancia();
    leerBluetooth();
    if (distancia <= 25 && distancia > 0) {
      esquivar();
    } else {
      avanzar();
    }
  }
}

void esquivar() {
  // while(distancia <= 5){
  //   retroceder();
  //   leerDistancia();
  // }

  //Gira hacia la derecha por dos segundos
  long duration = 1000;
  long startTime = millis();

  while (millis() - startTime <= duration) {
    retroceder();
  }

  duration = 2000;
  startTime = millis();

  while (millis() - startTime <= duration) {
    retroceder();
  }

}

void parar() {
	Serial.print(" All motor stopped");
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  
}

void retroceder() {
  Serial.print("back");
	motor1.run(FORWARD);
	motor2.run(FORWARD);
  motor3.run(FORWARD);
	motor4.run(FORWARD);
}

void avanzar() {
  Serial.print("fwd");
	motor1.run(BACKWARD);
	motor2.run(BACKWARD);
  motor3.run(BACKWARD);
	motor4.run(BACKWARD);
}

void irIzq() {
  Serial.print("left");
	motor1.run(FORWARD);
	motor2.run(BACKWARD);
	motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void irDer() {
  Serial.print("right");
	motor1.run(BACKWARD);
	motor2.run(FORWARD);
	motor3.run(FORWARD);
  motor4.run(BACKWARD);
}

void esqSupIzq() {
  motor1.setSpeed(125);
  motor2.setSpeed(250);
  motor3.setSpeed(250);
  motor4.setSpeed(125);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
}

void esqInfIzq() {
  motor1.setSpeed(125);
  motor2.setSpeed(250);
  motor3.setSpeed(250);
  motor4.setSpeed(125);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}

void esqInfDer() {
  motor1.setSpeed(250);
  motor2.setSpeed(125);
  motor3.setSpeed(125);
  motor4.setSpeed(250);
  motor1.run(FORWARD);
  motor2.run(BACKWARD); 
  motor3.run(BACKWARD);
  motor4.run(FORWARD); 
}

void esqSupDer() {
  motor1.setSpeed(250);
  motor2.setSpeed(125);
  motor3.setSpeed(125);
  motor4.setSpeed(250);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}

//checa si se quiere cambiar la velocidad
int checkVelocidad() {
  int velocidad = 0;
  switch (instruccion) {
    case '0':
      velocidad = 95;
      break;
    case '1':
      velocidad = 111;
      ;
      break;
    case '2':
      velocidad = 127;
      break;
    case '3':
      velocidad = 143;
      break;
    case '4':
      velocidad = 159;
      break;
    case '5':
      velocidad = 175;
      break;
    case '6':
      velocidad = 191;
      break;
    case '7':
      velocidad = 207;
    case '8':
      break;
      velocidad = 223;
    case '9':
      velocidad = 255;
    break;
  }
  return velocidad;
}

void cambiarVelocidad(int vel){
  motor1.setSpeed(vel);
  motor2.setSpeed(vel);
  motor3.setSpeed(vel);
  motor4.setSpeed(vel);
}

void leerDistancia() {
  digitalWrite(Trigger1, HIGH);
  delayMicroseconds(10);          //Enviamos un pulso de 10us
  digitalWrite(Trigger1, LOW);
  
  long t = pulseIn(Echo1, HIGH); //obtenemos el ancho del pulso
  long nuevaDistancia = t/58;

  if(nuevaDistancia > 0) distancia = nuevaDistancia;

  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
}
