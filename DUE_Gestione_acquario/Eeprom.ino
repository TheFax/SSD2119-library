#include <Wire.h>

#define VALIDATOR          0x1200  //deve essere un Integer a 16 bit
#define EEPROM_ADDRESS     B1010000    //Address of 24LC256 eeprom chip

void Eeprom_Reload () {
  Serial.println("<EEPROM> Reload parameters from external EEPROM");

  Wire.begin();

  if (Eeprom_Check_Validator ()==true) {
    Eeprom_Read ();
  } else {
    Parameters_Default ();
    Eeprom_Save ();
  }

}

void Eeprom_Save () {

  Serial.print("<EEPROM> Saving paremeters in EEPROM... ");
  
  //Sunrise Alba
  writeEEPROM_int(2, Sunrise_time);
  writeEEPROM_int(4, Sunrise_duration);
  
  //Sunset Tramonto
  writeEEPROM_int(6, Sunset_time);
  writeEEPROM_int(8, Sunset_duration);

  writeEEPROM_int(10, PWM_1_day);
  writeEEPROM_int(12, PWM_2_day);
  writeEEPROM_int(14, PWM_3_day);
  writeEEPROM_int(16, PWM_4_day);

  writeEEPROM_int(250, VALIDATOR);

  Serial.println("OK!");
  
  if (Eeprom_Check_Validator ()==true) {
    Serial.println("<EEPROM> Parameters saved");
  } else {
    Serial.println("<EEPROM> - Failed to save parameters");
    Serial.println("<EEPROM> Dump EEPROM for debug...  ");
    for (unsigned int address=0; address<20; address ++) {
      Serial.print ("<EEPROM> Address: ");
      Serial.print (address);
      Serial.print (" = Value: ");
      Serial.println (readEEPROM(address), DEC);
  }
  }
}

boolean Eeprom_Check_Validator () {
  Serial.print("<EEPROM> Check validator... ");
  int temp;

  temp = readEEPROM_int(250);
  
  if (temp == VALIDATOR) {
    Serial.println("OK!");
    return true;
  } else {
    Serial.println("FAIL!");
    return false;
  }
}

void Eeprom_Read () {
  if (Eeprom_Check_Validator() == false)  {
    Serial.println("<EEPROM> - Reading aborted");
    return;
  } 
  
  Serial.print("<EEPROM> Reloading parameters from EEPROM... ");
  
  //Sunrise Alba
  Sunrise_time = readEEPROM_int(2);
  Sunrise_duration = readEEPROM_int(4);

  //Sunset Tramonto
  Sunset_time = readEEPROM_int(6);
  Sunset_duration = readEEPROM_int(8);

  //Lights PWM
  PWM_1_day = readEEPROM_int(10);
  PWM_2_day = readEEPROM_int(12);
  PWM_3_day = readEEPROM_int(14);
  PWM_4_day = readEEPROM_int(16);

  Serial.println("OK!");
}

void Parameters_Default () {

  Serial.println("<EEPROM> Defaul parameters in RAM");
  
  //Sunrise Alba
  Sunrise_time = 600;
  Sunrise_duration = 50;

  //Sunset Tramonto
  Sunset_time = 1110;
  Sunset_duration = 50;

  PWM_1_day = 10;
  PWM_2_day = 11;
  PWM_3_day = 12;
  PWM_4_day = 13;
}

void writeEEPROM(unsigned int eeaddress, byte data )
{
  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(data);
  Wire.endTransmission();

  delay(5);
}

byte readEEPROM(unsigned int eeaddress )
{
  byte rdata = 0xFF;

  Wire.beginTransmission(EEPROM_ADDRESS);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(EEPROM_ADDRESS, 1);

  if (Wire.available()) rdata = Wire.read();

  return rdata;
}

void writeEEPROM_int(unsigned int eeaddress, int data) {
  byte low_Byte = ((data >> 0) & 0xFF);
  byte high_Byte = ((data >> 8) & 0xFF);

  writeEEPROM(eeaddress,   high_Byte);
  writeEEPROM(eeaddress + 1, low_Byte);
}

int readEEPROM_int(unsigned int eeaddress) {
  byte high_Byte = readEEPROM(eeaddress);
  byte low_Byte = readEEPROM(eeaddress + 1);
  int temp;
  temp = (((int)low_Byte << 0) & 0xFF) + (((int)high_Byte << 8) & 0xFF00);
  return temp;
}

