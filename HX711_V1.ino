// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>
// Pines de conexión
// Pines de conexión
const int LOADCELL_DOUT_PIN = D2;  // Pin para Data (DT) del HX711
const int LOADCELL_SCK_PIN = D3;   // Pin para Clock (SCK) del HX711

HX711ADC scale;  // Crear un objeto de la clase HX711

float calibration_factor = -179704.000000;  // Factor de calibración, inicialmente un valor estimado -179844
long zero_offset = 0;  // Offset de la celda sin peso
float known_weight = 500;  // Peso conocido para calibrar (en gramos)

// Función para notificaciones en Serial y Particle Cloud
void notify(String message) {
  Serial.println(message);  // Mostrar el mensaje en la consola Serial
  Particle.publish("balanza-data", message, PRIVATE);  // Enviar el mensaje a la nube de Particle
}

// Función que se llamará desde la nube para ajustar el factor de calibración
int adjustCalibrationFactor(String command) {
  if (command == "s") {
    calibration_factor += 10;  // Incrementa el factor de calibración
  } else if (command == "b") {
    calibration_factor -= 10;  // Disminuye el factor de calibración
  } else {
    return -1;  // Si el comando no es válido, retorna error
  }
  
  // Notificar el nuevo factor de calibración
  notify("Nuevo factor de calibración: " + String(calibration_factor));
  return 1;  // Retorna éxito
}
// Método para leer el peso en gramos
float getWeightGrams() {
    scale.set_scale(calibration_factor); 
    float weight = scale.get_units(50); 
    
    return weight;  // Devolver el valor del peso en gramos
}

void setup() {
    Serial.begin(9600);
    delay(1000);
    
    // Inicializar HX711
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

    // Registrar la función de ajuste de calibración en la nube
    Particle.function("adjustCalibration", adjustCalibrationFactor);
    
    // Ajuste para el valor de calibración inicial
    notify("Calibrando la celda de carga...");
    notify("Sin peso en la báscula, espere...");

    // Esperar 5 segundos para asegurar que la báscula está estable sin peso
    delay(5000);
    
    // Toma el valor en bruto sin peso
    zero_offset = scale.read_average(50);  // Toma 10 muestras promedio para obtener el offset sin peso
    notify("Valor offset: " + String(zero_offset));
    
    // Ajustar la celda de carga al offset detectado
    scale.set_offset(zero_offset);

    notify("Coloca un peso conocido en la báscula.");
    notify("Ajuste el factor de calibración con los comandos 'a' o 'z' desde la nube.");
}

void loop() {
    // Configurar el factor de calibración ajustado al objeto HX711
    scale.set_scale(calibration_factor);
    
    // Leer el peso en la báscula con el factor de calibración actual
    float weight = scale.get_units(50);  // Toma un promedio de 10 lecturas
    
    notify(String(weight));

    delay(5000);  // Leer cada 5 segundos
}
