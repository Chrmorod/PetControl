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

HX711ADC celda(DT,SCK,64);		

void setup() {
  Serial.begin(9600);		
  celda.begin(DT, SCK);		
  //Serial.println("Para obtener factor de escala:");	
  Particle.publish("Para obtener factor de escala:");
  celda.set_scale();		//default scale factor
  celda.tare();			//tare or stored zeroing
  //Serial.println("Colocar un peso conocido (20 seg.)");	
  Particle.publish("Colocar un peso conocido (20 seg.)");
  delay(20000);	
  //Serial.println(celda.get_units(10));  
  Particle.publish(String(celda.get_units(10)));
  //Serial.println("Hecho. Dividir el valor mostrado por el peso colocado");
  Particle.publish("Hecho. Dividir el valor mostrado por el peso colocado");
}
void loop() {}
