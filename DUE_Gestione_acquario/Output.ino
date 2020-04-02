//
#define OUTPUT_PWM_ONE           2
#define OUTPUT_PWM_TWO           3
#define OUTPUT_PWM_THREE         5
#define OUTPUT_PWM_FOUR          6

#define OUTPUT_PUMP_ONE          30
#define OUTPUT_PUMP_TWO          31
#define OUTPUT_HEATER            32
#define OUTPUT_AUX               33

#define OUTPUT_HEART_LED         13

void Output_Init () {
  Serial.print("<OUTPUT> Init PWM and digital outputs...  ");
  
  pinMode(OUTPUT_PWM_ONE, OUTPUT);
  pinMode(OUTPUT_PWM_TWO, OUTPUT);
  pinMode(OUTPUT_PWM_THREE, OUTPUT);
  pinMode(OUTPUT_PWM_FOUR, OUTPUT);
  pinMode(OUTPUT_PUMP_ONE, OUTPUT);
  pinMode(OUTPUT_PUMP_TWO, OUTPUT);
  pinMode(OUTPUT_HEATER, OUTPUT);
  pinMode(OUTPUT_AUX, OUTPUT);
  pinMode(OUTPUT_HEART_LED, OUTPUT);

  PWM_1_actual=0;
  PWM_2_actual=0;
  PWM_3_actual=0;
  PWM_4_actual=0;
  
  Output_PWM ();

  Pump_1=HIGH;
  Pump_2=HIGH;
  Heater=HIGH;
  Auxiliary=HIGH;
  
  Output_Digital ();
  
  digitalWrite(OUTPUT_HEART_LED, LOW);

  Light_Percentage=0;
  Actual_Mode=1;
  
  Serial.println("OK!");
}

void Output_Digital () {
  digitalWrite(OUTPUT_PUMP_ONE, Pump_1);
  digitalWrite(OUTPUT_PUMP_TWO, Pump_2);
  digitalWrite(OUTPUT_HEATER, Heater);
  digitalWrite(OUTPUT_AUX, Auxiliary);
}

void Output_PWM () {
  analogWrite(OUTPUT_PWM_ONE, PWM_1_actual);
  analogWrite(OUTPUT_PWM_TWO, PWM_2_actual);
  analogWrite(OUTPUT_PWM_THREE, PWM_3_actual);
  analogWrite(OUTPUT_PWM_FOUR, PWM_4_actual);
}

void Output_Light_Control() {
  long Actual_second = seconds_RTC + (minutes_RTC*60) + (hours_RTC*60*60);
  long Sunrise_second = Sunrise_time * 60;
  long Sunset_second = Sunset_time * 60;
  long Sunrise_duration_second = Sunrise_duration * 60;
  long Sunset_duration_second = Sunset_duration * 60;
  
  
  if (Actual_second < Sunrise_second) {
    //night
    PWM_1_actual=0;
    PWM_2_actual=0;
    PWM_3_actual=0;
    PWM_4_actual=0;
    Actual_Mode=1;
    Light_Percentage=0;
  } else if (Actual_second >= Sunrise_second & Actual_second < Sunrise_second + Sunrise_duration_second) {
    //sunrise
    PWM_1_actual=(float(PWM_1_day)/Sunrise_duration_second)*float(Actual_second - Sunrise_second);
    PWM_2_actual=(float(PWM_2_day)/Sunrise_duration_second)*float(Actual_second - Sunrise_second);
    PWM_3_actual=(float(PWM_3_day)/Sunrise_duration_second)*float(Actual_second - Sunrise_second);
    PWM_4_actual=(float(PWM_4_day)/Sunrise_duration_second)*float(Actual_second - Sunrise_second);
    Serial.print("<OUTPUT> PWM log sunrise: ");  Serial.print(PWM_1_actual); Serial.print("-"); Serial.print(PWM_2_actual); Serial.print("-"); Serial.print(PWM_3_actual); Serial.print("-"); Serial.println(PWM_4_actual); 
    Actual_Mode=2;
    Light_Percentage=(float(100)/Sunrise_duration_second)*float(Actual_second - Sunrise_second);
  } else if (Actual_second >= Sunrise_second + Sunrise_duration_second & Actual_second < Sunset_second) {    
    //day
    PWM_1_actual=PWM_1_day;
    PWM_2_actual=PWM_2_day;
    PWM_3_actual=PWM_3_day;
    PWM_4_actual=PWM_4_day;
    Actual_Mode=3;
    Light_Percentage=100;
  } else if (Actual_second >= Sunset_second &  Actual_second < Sunset_second + Sunset_duration_second ) {    
    //sunset
    PWM_1_actual=PWM_1_day-(float(PWM_1_day)/Sunset_duration_second)*float(Actual_second - Sunset_second);
    PWM_2_actual=PWM_2_day-(float(PWM_2_day)/Sunset_duration_second)*float(Actual_second - Sunset_second);
    PWM_3_actual=PWM_3_day-(float(PWM_3_day)/Sunset_duration_second)*float(Actual_second - Sunset_second);
    PWM_4_actual=PWM_4_day-(float(PWM_4_day)/Sunset_duration_second)*float(Actual_second - Sunset_second);
    Serial.print("<OUTPUT> PWM log sunset: "); Serial.print(PWM_1_actual); Serial.print("-"); Serial.print(PWM_2_actual); Serial.print("-"); Serial.print(PWM_3_actual); Serial.print("-"); Serial.println(PWM_4_actual); 
    Actual_Mode=4;
    Light_Percentage=100-(float(100)/Sunset_duration_second)*float(Actual_second - Sunset_second);
  } else if (Actual_second >= Sunset_second + Sunset_duration_second) {
    //night
    PWM_1_actual=0;
    PWM_2_actual=0;
    PWM_3_actual=0;
    PWM_4_actual=0;
    Actual_Mode=1;
    Light_Percentage=0;
  } 
    if (PWM_1_actual>255 | PWM_1_actual<0)  { Serial.print("<OUTPUT> - PWM1 out of range...  "); Serial.println(PWM_1_actual); PWM_1_actual=0; }
    if (PWM_2_actual>255 | PWM_2_actual<0)  { Serial.print("<OUTPUT> - PWM2 out of range...  "); Serial.println(PWM_2_actual); PWM_2_actual=0; }
    if (PWM_3_actual>255 | PWM_3_actual<0)  { Serial.print("<OUTPUT> - PWM3 out of range...  "); Serial.println(PWM_3_actual); PWM_3_actual=0; }
    if (PWM_4_actual>255 | PWM_4_actual<0)  { Serial.print("<OUTPUT> - PWM4 out of range...  "); Serial.println(PWM_4_actual); PWM_4_actual=0; }
 
}

