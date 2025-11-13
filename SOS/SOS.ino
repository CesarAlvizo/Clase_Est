/*
 * Fecha: 10/10/2025
 * Descripción: 
 * Implementa el mensaje de auxilio internacional "SOS" ( ...---... ) 
 * usando un LED conectado al pin 13 de Arduino.
 */

const int ledPin = 13; 

// Duraciones en milisegundos basadas en el estándar Morse
const int punto = 250;         
const int raya = punto * 3;    
const int pausaSimbolo = punto;   
const int pausaLetra = 1000;       
const int pausaMensaje = 1000;    

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {

  // S = "..."
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH); 
    delay(punto);
    digitalWrite(ledPin, LOW);
    delay(pausaSimbolo); 
  }

  delay(pausaLetra);

  // O = "---"
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(raya); 
    digitalWrite(ledPin, LOW);
    delay(pausaSimbolo);
  }

  delay(pausaLetra);

  // S = "..."
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(punto);
    digitalWrite(ledPin, LOW);
    delay(pausaSimbolo);
  }

  delay(pausaMensaje);
}
