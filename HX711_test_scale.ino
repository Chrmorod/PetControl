// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>

// HX711.DOUT	- pin #A1
// HX711.PD_SCK	- pin #A0

HX711ADC balanza(A1, A0, 64);  // El valor predeterminado 128 para "gain" es utilizado por la librería

// funciones
int calibrateScale(String command);

// Función para enviar datos a la nube
void notify(String message) {
  Serial.println(message);          // Mostrar el mensaje en la consola Serial
  Particle.publish("balanza-data", message, PRIVATE);  // Enviar el mensaje a la nube de Particle
}
// Imprime lecturas iniciales antes de configurar la balanza (test)
void printInitialReadings() {
  notify("Before setting up the scale:");
  notify("read: " + String(balanza.read()));           // Imprimir una lectura cruda del ADC

  notify("read average: " + String(balanza.read_average(20)));  // Imprimir el promedio de 20 lecturas del ADC

  notify("get value: " + String(balanza.get_value(5)));   // Imprimir el promedio de 5 lecturas del ADC menos el peso de tara

  notify("get units: " + String(balanza.get_units(5), 1));  // Imprimir el promedio de 5 lecturas del ADC menos el peso de tara dividido por SCALE
}
// Función para calibrar la balanza
int calibrateScale(String command) {
  notify("Lectura del valor del ADC");
  notify("read average: " + String(balanza.read()));
    delay(5000);  
  notify("no ponga ningun objeto sobre la balanza");
    delay(5000);  
  notify("Destarando...");
  balanza.set_scale();
  balanza.tare(20);
  notify("coloque peso conocido");
  return 1;
}

void setup() {
  Serial.begin(38400);

  notify("HX711 Test");
  printInitialReadings();

  // Registro funcion
  Particle.function("calibrateScale", calibrateScale);
}

void loop() {
  notify("valor de lectura: " + String(balanza.get_value(10),0));
  delay(100);
}
