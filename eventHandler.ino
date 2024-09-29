// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>
// Include Particle Device OS APIs
#include "Particle.h"
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);
// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);
/******************************************************************************/
#define DT D2			
#define SCK D3	
#define STP D6
#define DIR D4
bool engineState = false;
float knowWeight = 138.0; //138 g --> Change knowWeight
float CALIBRATION_FACTOR = 1;
HX711ADC celda(DT,SCK,64);
// notifications - Particle Cloud
void notify(String message) {
  Particle.publish("Taki-App-Info", message, PRIVATE); 
}
int handlerEngine(String command) {
    digitalWrite(DIR, LOW);
    for(int i=0;i<100;i++){
        digitalWrite(STP,HIGH);
        delay(10);
        digitalWrite(STP, LOW);
        delay(10);
    }
        return 1;
}

float handlerCalibrationFactor(float rawValue = 0) {
    if(rawValue != 0){
        //float calibration_factor = 1090.420571; 
        CALIBRATION_FACTOR = rawValue/knowWeight;
    }else{
        CALIBRATION_FACTOR = 1;
    }
    celda.set_scale(CALIBRATION_FACTOR);
    celda.tare();
    return CALIBRATION_FACTOR;
}
float handlerCalibrationFactorWrapper(String command){ //only for Remote Function on Particle
    float rawValue = command.toFloat();
    float calibration_factor = handlerCalibrationFactor(rawValue);
    return calibration_factor;
}
float handlerAdjustScale(String command){
    notify("Para obtener factor de escala:");
    handlerCalibrationFactor();
    notify("Colocar un peso conocido (20 seg.)");
    delay(20000);
    float result = celda.get_units(10);
    notify("Hecho. Dividir el valor mostrado por el peso colocado");
    return result;
}
float handlerWeight(String command){
    notify("Valor (g): "+String(celda.get_units(10), 1));
    celda.power_down();// power down HX711
    delay(3000);		
    celda.power_up();// power up HX711
    return celda.get_units(10);
}
void setup() {
    Serial.begin(9600);
    celda.begin(DT, SCK); //Pin Conf - Data and Clock - HX711
    pinMode(STP,OUTPUT); //Pin Conf - Steps - A4988
    pinMode(DIR,OUTPUT); //Pin Conf - Direction - A4988
    //float rawValue = handlerAdjustScale();
    //handlerCalibrationFactor(rawValue);
    
    // Registry Remote Function on Particle
    Particle.function("handlerEngine", handlerEngine); 
    Particle.function("handlerAdjustScale", handlerAdjustScale);
    Particle.function("handlerCalibrationFactorWrapper",handlerCalibrationFactorWrapper);
    Particle.function("handlerWeight",handlerWeight);
}

void loop() {}