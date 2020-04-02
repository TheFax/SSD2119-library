// Sistema gestione acquario
// REV      :   1
// Target   :   Arduino DUE + Display SSD2119
// Serial   :
// IDE      :   1.6.8

#define BUTTON_ESC        1
#define BUTTON_UP         2
#define BUTTON_DOWN       4
#define BUTTON_ENT        8
byte Buttons_Status;

double Temperature_Real;
double Temperature_Min;
double Temperature_Max;

int PWM_1_day;
int PWM_2_day;
int PWM_3_day;
int PWM_4_day;

int PWM_1_actual;
int PWM_2_actual;
int PWM_3_actual;
int PWM_4_actual;

byte Actual_Mode;
int Light_Percentage;

int Heater;
int Auxiliary;
int Pump_1;
int Pump_2;

int Sunrise_time;
int Sunrise_duration;
int Sunset_time;
int Sunset_duration;

short Menu_Pointer;
short Menu_Level_Main;
short Menu_Level_Sub;
boolean Force_Complete_Redraw;

int hours_RTC;
int minutes_RTC;
int seconds_RTC;

int delta_Buttons = 0;

void setup() {
  //Inizializzo comunicazione Seriale attraverso porta programmazione del SAM3
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Welcome - ACQUARIUM Firmware");
  Serial.println("You are connected to Programming Serial Port");
  //Inizializzo comunicazione Seriale attraverso porta nativa
  //SerialUSB.begin(115200);

  //Inizializzo comunicazione SPI e display LCD
  Serial.print("<MAIN> Init color display...  ");
  SSD2119_Init();
  Serial.println("OK!");

  //Inizializzo le uscite del micro
  Output_Init();

  //Inizializzo la lettura dei pulsanti
  Buttons_Init();
  
  //Inizializzo la periferica RTC all'interno dell'ARM SAM3
  PCF8583_init(); //RTC

  //Leggo l'orario, per la prima volta
  PCF8583_get_time();

  //Inizializzo la lettura di temperatura
  Temperature_Init();

  //Leggo i parametri da EEPROM
  Eeprom_Reload ();

  
  //Inizializzo la grafica
  Display_Init();
}

void loop() {
  unsigned long time_now;
  unsigned long time_and_30ms;  //FIXME TUTTI I 30ms
  unsigned long time_and_100ms;
  unsigned long time_and_500ms;
  unsigned long time_and_1000ms;

  time_now=millis();

  time_and_30ms=time_now+30;
  time_and_100ms=time_now+100;
  time_and_500ms=time_now+500;
  time_and_1000ms=time_now+1000;

  //Migrazione ad interrupt? https://github.com/ivanseidel/DueTimer

  while (1) {
    
  time_now=millis();
  
  if (time_now < time_and_30ms & 
      time_now < time_and_100ms &
      time_now < time_and_500ms &
      time_now < time_and_1000ms ) {
        //IDLE
        digitalWrite(13, HIGH);
      } else {
        //WORKING
        digitalWrite(13, LOW);
      }
      
  if (time_now >= time_and_30ms) {
    do {
      //Reagisco alla pressione dei pulsanti
      if (Buttons_Status) {
        Buttons_Actions ();
        Display_Show ();
      }

      //Leggo lo stato dei pulsanti
      Buttons_Read();

    } while (Buttons_Status);
    
    //Preparazione per il prossimo task
    time_and_30ms=time_now+10;  
  }
  if (time_now >= time_and_100ms) {
    //Preparazione per il prossimo task
    time_and_100ms=time_now+100;
  }
  if (time_now >= time_and_500ms) {
    //Leggo il termometro
    Temperature_Read();
    
    //Preparazione per il prossimo task
    time_and_500ms=time_now+500;
  }
  if (time_now >= time_and_1000ms) {
    //Aggiorno la grafica
    Display_Show ();

    //Rileggo ora e minuti dall'RTC
    PCF8583_get_time();

    //Aggiorno uscite digitali
    Output_Digital();

    //Calcolo luminosità luci
    Output_Light_Control();
    
    //Aggiorno uscite PWM
    Output_PWM();

    //Preparazione per il prossimo task
    time_and_1000ms=time_now+1000;
  }

  }
}

