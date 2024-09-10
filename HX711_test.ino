#include "HX711ADC.h"

// HX711.DOUT	- pin #A1
// HX711.PD_SCK	- pin #A0

HX711ADC scale(A1, A0, 64);  // El valor predeterminado 128 para "gain" es utilizado por la librería

bool scaleConfigured = false;

// Prototipos de las funciones
int setupScale(String command);
int getReading(String command);
int getAverageReading(String command);
int tareScale(String command);

// Función auxiliar para enviar datos a la nube
void notify(String message) {
  //Serial.println(message);          
  Particle.publish("scale-data", message, PRIVATE);  // Enviar el mensaje a la nube de Particle
}

// Imprime lecturas iniciales antes de configurar la balanza
void printInitialReadings() {
  notify("Before setting up the scale:");
  notify("read: " + String(scale.read()));           // Imprime una lectura cruda del ADC

  notify("read average: " + String(scale.read_average(20)));  // Imprime el promedio de 20 lecturas del ADC

  notify("get value: " + String(scale.get_value(5)));   // Imprime el promedio de 5 lecturas del ADC menos el peso de tara

  notify("get units: " + String(scale.get_units(5), 1));  // Imprime el promedio de 5 lecturas del ADC menos el peso de tara dividido por SCALE
}

// Configuración la balanza
int setupScale(String command) {
  scale.set_scale(2280.f);             // Este valor se obtiene calibrando la balanza con pesos conocidos (2280.f)
  scale.tare();                        // Restablecer la balanza a 0
  scaleConfigured = true;              // Balanza está configurada (true)

  notify("Scale set up and tared.");
  return 1;
}

// Obtener una lectura 
int getReading(String command) {
  if (!scaleConfigured) {
    notify("Scale not configured. Please call setupScale first.");
    return -1;
  }

  float reading = scale.get_units(1);  // Obtener una lectura
  notify("One reading: " + String(reading, 1));

  return reading;                      // Devuelve el valor de la lectura
}

// Obtener promedio de las lecturas 
int getAverageReading(String command) {
  if (!scaleConfigured) {
    notify("Scale not configured. Please call setupScale first.");
    return -1;
  }

  float avgReading = scale.get_units(10);  // Obtener promedio de las 10 lecturas
  notify("Average reading: " + String(avgReading, 1));

  return avgReading;                       // Devuelve el valor promedio
}

// Realizar la tara de la balanza 
int tareScale(String command) {
  scale.tare();
  notify("Scale tared.");
  return 1;
}

void setup() {
  Serial.begin(38400);
  notify("HX711 Demo");

  scale.begin();
  printInitialReadings();

  // Registrar funciones que se pueden llamar desde la consola de Particle
  Particle.function("setupScale", setupScale);
  Particle.function("getReading", getReading);
  Particle.function("getAvgReading", getAverageReading);
  Particle.function("tareScale", tareScale);
}

void loop() {
  // El loop vacio ya que las funciones son llamadas desde la consola
}
