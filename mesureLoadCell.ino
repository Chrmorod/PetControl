// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>
// Include Particle Device OS APIs
#include "Particle.h"
// Let Device OS manage the connection to the Particle Cloud
SYSTEM_MODE(AUTOMATIC);
// Show system, cloud connectivity, and application logs over USB
// View logs with CLI using 'particle serial monitor --follow'
SerialLogHandler logHandler(LOG_LEVEL_INFO);
/********************************************************************************/
#define DT D2			
#define SCK D3			
HX711ADC celda;			

void setup() {
  Serial.begin(9600);		
  //Serial.println("Balanza con celda de carga");	
  Particle.publish("Balanza con celda de carga");
  celda.begin(DT, SCK);	
  //X=-0.023333333 --> obtained value from adjustScale.ino
  celda.set_scale(1090.420571); // factor_scale = X/know weight = 1090.420571
  celda.tare();			
}
void loop() {
  //Serial.print("Valor (g): ");
  //Serial.println(celda.get_units(10), 1);	
  Particle.publish("Valor (g): "+String(celda.get_units(10), 1));
  celda.power_down();// power down HX711
  delay(3000);		
  celda.power_up();// power up HX711
}