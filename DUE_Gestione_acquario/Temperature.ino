//Modulo per la lettura della temperatura

#define TEMPERATURE_NTC_PIN        A0

void Temperature_Init() {
  //Init per questo modulo
  Serial.print("<TEMPERATURE> Init temperature sensor...  ");
  
  delay(1000);
  Temperature_Real = Thermistor(analogRead(TEMPERATURE_NTC_PIN));
  Temperature_Min = Temperature_Real;
  Temperature_Max = Temperature_Real;

  Serial.println("FIXME!");
}

void Temperature_Read() {
  //Leggo la temperatura
  Temperature_Real = Thermistor(analogRead(TEMPERATURE_NTC_PIN));

  //Aggiorno valori Min e Max
  if (Temperature_Min > Temperature_Real) Temperature_Min = Temperature_Real;
  if (Temperature_Max < Temperature_Real) Temperature_Max = Temperature_Real;
}

double Thermistor(int RawADC) {
  //Questa funzione calcola la temperatura letta da una NTC da 10Kohm collegata in serie ad una resistenza da 10Kohm che fa da pull-up o pull-down
  double Temp;
  
  Temp = log(10000.0 * (1024.0 / RawADC - 1));   // per configurazione PULL-DOWN
  //   = log(10000.0 / (1024.0 / RawADC - 1));   // per configurazione PULL-UP
  Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp )) * Temp );  // Equazione di Steinhart-Hart: https://en.wikipedia.org/wiki/Thermistor
  Temp = Temp - 273.15;                          // Converto Kelvin in Celcius
  //Temp = (Temp * 9.0) / 5.0 + 32.0;            // Converto Celcius in Fahrenheit
  
  return Temp;
}
