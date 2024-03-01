#include <SoftwareSerial.h>
#include <EasyBuzzer.h>

SoftwareSerial BT(10, 11);

const char NADA = 'n';
const char MODO_AUTO = 'w';
const char MODO_CONTROL = 'W';
const char ADELANTE = 'F';
const char ATRAS = 'B';
const char IZQ = 'L';
const char DER = 'R';
const char PARAR = 'S';

/*
const int Trigger1 = 2;
const int Echo1 = 3;

const int Trigger2 = 4;
const int Echo2 = 5;

const int led1 = 8;
const int led2 = 9;

const int buzzer = 10;

const unsigned int frequency = 1000;
const int beeps = 10;
*/

// Conexiones del Motor A
int enA = 7;
int in1 = 9;
int in2 = 8;

// Conexiones del Motor B
int enB = 6;
int in3 = 4;
int in4 = 5;

void setup() {
	BT.begin(9600);
	Serial.begin(9600);

	pinMode(enA, OUTPUT);
	pinMode(in1, OUTPUT);
	pinMode(in2, OUTPUT);

	pinMode(enB, OUTPUT);
	pinMode(in3, OUTPUT);
	pinMode(in4, OUTPUT);
}

void loop() {
	modoControl();
	modoAuto();
}

char leerBluetooth() {
	char instruccion = NADA;
	if (BT.available()) {
		instruccion = BT.read();
		Serial.write(instruccion);
	}
	if (Serial.available()) {
		BT.write(Serial.read());
	}
	return instruccion;
}

void modoAuto() {
	char instruccion = NADA;
	while (instruccion != MODO_CONTROL) {
		instruccion = leerBluetooth();
		Serial.println("\nModo auto :)");
	}
}

void modoControl() {
	char instruccion = NADA;
	Serial.println("\nModo control :)");
	while (instruccion != MODO_AUTO) {
		instruccion = leerBluetooth();
		if (instruccion == PARAR) {
			parar();
			Serial.println("PARAR");
		}
		if (instruccion == ADELANTE) {
			avanzar();
			Serial.println("ADELANTEEE");
		}
		if (instruccion == ATRAS) {
			retroceder();
			Serial.println("ATRAAAS");
		}
		if (instruccion == IZQ) {
			irIzq();
			Serial.println("PA'LA IZQUIERDAAA");
		}
		if (instruccion == DER) {
			irDer();
			Serial.println("PA'LA DERECHAAAA");
		}
	}
}

/*
	un motor funciona con dos señales:
		cuando las dos estan en LOW, el motor se detiene
		cuando la primera está en HIGH y la segunda está en LOW, el motor gira hacia adelante
		cuando la primera está en LOW y la segunda está en HIGH, el motor gira hacia atrás
		cuando ambas están en HIGH, pues no sé qué hace, tal vez no hace nada, pero no las pongan en HIGH las dos

	entonces, como hay dos motores, cuando queremos que avance, hay que hacer que ambos motores giren hacia adelante,
	como uno de los motores está al revés, tenemos que hacer que los motores giren en sentido contrario, igual para que
	el carro retroceda

	cuando queremos que gire hacia un lado, debemos de hacer que gire solo un lado y el otro se apague

	de momento no podemos ajustar la velocidad a la que el motor acelera y desacelera, así que de momento solo podemos
	prederlos y apagarlos, lo de la velocidad se hará cuando tengamos el shield
*/

void parar() {
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);

	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}

void avanzar() {
	digitalWrite(in1, HIGH);
	digitalWrite(in2, LOW);

	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
}

void retroceder() {
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);

	digitalWrite(in3, HIGH);
	digitalWrite(in4, LOW);
}

void irIzq() {
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);

	digitalWrite(in3, HIGH);
	digitalWrite(in4, HIGH);
}

void irDer() {
	digitalWrite(in1, HIGH);
	digitalWrite(in2, HIGH);

	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}
