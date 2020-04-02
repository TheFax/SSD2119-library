#include <Wire.h>

#define PCF8583_ADDRESS    B1010001  //Qui, specificare l'indirizzo a 7 bit del PCF.
                                     // Con A0=0 B1010000
                                     // Con A0=1 B1010001

// initialization 
void PCF8583_init() {
  Serial.print("<PCF8583> Init external RTC...  ");
  
  Wire.begin();

  Wire.beginTransmission(PCF8583_ADDRESS);
  Wire.write(0x00);  //Scriverò nell'indirizzo: 0x00
  Wire.write(0x04);  //... Ecco il dato da scrivere: 0x04
  Wire.endTransmission();
  
  Serial.println("OK!");
}

void PCF8583_get_time() {
  Wire.beginTransmission(PCF8583_ADDRESS);
  Wire.write(0xC0);   // stop counting, don't mask
  Wire.endTransmission();

  Wire.beginTransmission(PCF8583_ADDRESS);
  Wire.write(0x02);   //Indirizzo dei secondi
  Wire.endTransmission();
  Wire.requestFrom(PCF8583_ADDRESS, 3);

  seconds_RTC = PCF8583_bcd_to_byte(Wire.read());
  minutes_RTC = PCF8583_bcd_to_byte(Wire.read());
  hours_RTC   = PCF8583_bcd_to_byte(Wire.read());
}


void PCF8583_set_time() {
  Wire.beginTransmission(PCF8583_ADDRESS);
  Wire.write(0xC0);   // stop counting, don't mask
  Wire.endTransmission();

  Wire.beginTransmission(PCF8583_ADDRESS);
  Wire.write(0x02);
  Wire.write(PCF8583_int_to_bcd(seconds_RTC));
  Wire.write(PCF8583_int_to_bcd(minutes_RTC));
  Wire.write(PCF8583_int_to_bcd(hours_RTC));
  Wire.endTransmission();
  
  PCF8583_init(); // re set the control/status register to 0x04
}

int PCF8583_bcd_to_byte(byte bcd){
  return ((bcd >> 4) * 10) + (bcd & 0x0f);
}

byte PCF8583_int_to_bcd(int in){
  return ((in / 10) << 4) + (in % 10);
}

