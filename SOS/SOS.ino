/*
 * Fecha: 10/10/2025
 * Descripción: 
 * Implementa el mensaje de auxilio internacional "SOS" ( ...---... ) 
 * usando un LED conectado al pin 13 de Arduino.
 */



// Pin donde está conectado el LED
const int ledPin = 13; 

// Duraciones en milisegundos basadas en el estándar Morse
const int punto = 250;         // Duración base (0.250 segundos) para el "punto" (.)
const int raya = punto * 3;    // El "raya" (-) es tres veces la duración del punto
const int pausaSimbolo = punto;   // Pausa entre cada punto o raya
const int pausaLetra = 1000;      // Pausa entre letras
const int pausaMensaje = 1000;    // Pausa entre mensaje


/**
 * @brief Función de configuración inicial.
 * Se ejecuta una sola vez al inicio del programa.
 */
 void setup() {
  // Configura el pin del LED como salida para poder controlar su estado.
  pinMode(ledPin, OUTPUT);
}


/**
 * @brief Función principal que se ejecuta continuamente.
 * Contiene la lógica para enviar el mensaje SOS ( ...---... ) en bucle.
 */
void loop() {

  // 1. Letra S = "..." (3 puntos rápidos)
  // Bucle para los tres puntos de la 'S'
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH); 
    delay(punto);

    // APAGA el LED (fin del punto)
    digitalWrite(ledPin, LOW);
    delay(pausaSimbolo); 
  }

  // Pausa que marca el fin de la letra 'S' y el inicio de la 'O'
  delay(pausaLetra);

  // 2. Letra O = "---" (3 rayas)
  // Bucle para las tres rayas de la 'O'
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(raya); 

    // APAGA el LED (fin de la raya)
    digitalWrite(ledPin, LOW);
    delay(pausaSimbolo);
  }

  // Pausa larga que marca el fin de la letra 'O' y el inicio de la siguiente 'S'
  delay(pausaLetra);

  // 3. Letra S = "..." (3 puntos rápidos)
  // Bucle para los tres puntos de la última 'S'
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(punto);

    // APAGA el LED (fin del punto)
    digitalWrite(ledPin, LOW);
    delay(pausaSimbolo);
  }

  // Tiempo de espera antes de que el mensaje "SOS" se repita (inicio del próximo loop)
  delay(pausaMensaje);
}
