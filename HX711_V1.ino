// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>
// Pines de conexión
const int LOADCELL_DOUT_PIN = D2;  // Pin para Data (DT) del HX711
const int LOADCELL_SCK_PIN = D3;   // Pin para Clock (SCK) del HX711

HX711ADC scale;  // Crear un objeto de la clase HX711

float calibration_factor = -7050;  // Factor de calibración, inicialmente un valor estimado
long zero_offset = 0;  // Offset de la celda sin peso
float known_weight = 500;  // Peso conocido para calibrar (en gramos)

void notify(String message) {
  Serial.println(message);  // Mostrar el mensaje en la consola Serial
  Particle.publish("balanza-data", message, PRIVATE);  // Enviar el mensaje a la nube de Particle
}

void setup() {
    Serial.begin(9600);
    delay(1000);
    
    // Inicializar HX711
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    
    // Ajuste para el valor de calibración inicial
    notify("Calibrando la celda de carga...");
    notify("Sin peso en la báscula, espere...");

    // Esperar 5 segundos para asegurar que la báscula está estable sin peso
    delay(5000);
    
    // Toma el valor en bruto sin peso
    zero_offset = scale.read_average(10);  // Toma 10 muestras promedio para obtener el offset sin peso
    notify("Valor offset: " + String(zero_offset));
    
    // Ajustar la celda de carga al offset detectado
    scale.set_offset(zero_offset);

    notify("Coloca un peso conocido en la báscula.");
    notify("Ajuste el factor de calibración con las teclas a/z para subir/bajar.");
}

void loop() {
    // Configurar el factor de calibración ajustado al objeto HX711
    scale.set_scale(calibration_factor);
    
    // Leer el peso en la báscula con el factor de calibración actual
    float weight = scale.get_units(10);  // Toma un promedio de 10 lecturas
    
    notify("Peso: " + String(weight) + " gramos");

    // Permitir ajuste del factor de calibración con teclas 'a' o 'z'
    if (Serial.available()) {
        char temp = Serial.read();
        if (temp == 'a') {
            calibration_factor += 10;  // Incrementa el factor de calibración
        } else if (temp == 'z') {
            calibration_factor -= 10;  // Disminuye el factor de calibración
        }
        notify("Nuevo factor de calibración: " + String(calibration_factor));
    }

    delay(500);
}
