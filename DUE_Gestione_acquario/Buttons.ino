//Buttons connections to hardware PIN
#define BUTTON_ESC_PIN        25
#define BUTTON_UP_PIN         23
#define BUTTON_DOWN_PIN       24
#define BUTTON_ENT_PIN        22

#define BUTTON_ACTIVE         HIGH
#define BUTTON_NOT_ACTIVE     LOW

void Buttons_Init() {
  Serial.print("<BUTTONS> Init buttons...  ");
  
  //Inizializzo Pin per uso in questo modulo
  pinMode(BUTTON_ESC_PIN, INPUT);
  pinMode(BUTTON_UP_PIN, INPUT);
  pinMode(BUTTON_DOWN_PIN, INPUT);
  pinMode(BUTTON_ENT_PIN, INPUT);

  //Imposto la variabile che contiene i buttons premuti a zero
  Buttons_Status = 0;

  Serial.println("OK!");
}

void Buttons_Read() {
  //Questa funzione trova i fronti di salita sugli ingressi dei pulsanti
  int now_Button_ESC;
  int now_Button_UP;
  int now_Button_DOWN;
  int now_Button_ENT;

  static int ex_Button_ESC = BUTTON_NOT_ACTIVE;
  static int ex_Button_UP = BUTTON_NOT_ACTIVE;
  static int ex_Button_DOWN = BUTTON_NOT_ACTIVE;
  static int ex_Button_ENT = BUTTON_NOT_ACTIVE;

  static unsigned long last_press_milliseconds;

  now_Button_ESC=digitalRead(BUTTON_ESC_PIN);
  now_Button_UP=digitalRead(BUTTON_UP_PIN);
  now_Button_DOWN=digitalRead(BUTTON_DOWN_PIN);
  now_Button_ENT=digitalRead(BUTTON_ENT_PIN);

  if (now_Button_ESC==BUTTON_ACTIVE && ex_Button_ESC==BUTTON_NOT_ACTIVE)   { Buttons_Status = Buttons_Status | BUTTON_ESC;   last_press_milliseconds=millis(); }
  if (now_Button_UP==BUTTON_ACTIVE && ex_Button_UP==BUTTON_NOT_ACTIVE)     { Buttons_Status = Buttons_Status | BUTTON_UP;    last_press_milliseconds=millis(); }
  if (now_Button_DOWN==BUTTON_ACTIVE && ex_Button_DOWN==BUTTON_NOT_ACTIVE) { Buttons_Status = Buttons_Status | BUTTON_DOWN;  last_press_milliseconds=millis(); }
  if (now_Button_ENT==BUTTON_ACTIVE && ex_Button_ENT==BUTTON_NOT_ACTIVE)   { Buttons_Status = Buttons_Status | BUTTON_ENT;   last_press_milliseconds=millis(); }

  if ( now_Button_ESC  == BUTTON_ACTIVE ||
       now_Button_UP   == BUTTON_ACTIVE ||
       now_Button_DOWN == BUTTON_ACTIVE ||
       now_Button_ENT  == BUTTON_ACTIVE ) {
    if (millis() > (last_press_milliseconds + 500)) {
      //Serial.println("<BUTTONS> Button repeat");
      if (now_Button_ESC==BUTTON_ACTIVE)   { Buttons_Status = Buttons_Status | BUTTON_ESC;   }
      if (now_Button_UP==BUTTON_ACTIVE)    { Buttons_Status = Buttons_Status | BUTTON_UP;    }
      if (now_Button_DOWN==BUTTON_ACTIVE)  { Buttons_Status = Buttons_Status | BUTTON_DOWN;  }
      if (now_Button_ENT==BUTTON_ACTIVE)   { Buttons_Status = Buttons_Status | BUTTON_ENT;   }
    }
  }

  ex_Button_ESC = now_Button_ESC;
  ex_Button_UP = now_Button_UP;
  ex_Button_DOWN = now_Button_DOWN;
  ex_Button_ENT = now_Button_ENT;
}

void Buttons_Actions () {
  Serial.println("<BUTTONS> Button pressed");

#define MAX_SUBLEVEL_1    4
#define MAX_SUBLEVEL_2    3
  
  if (Buttons_Status==0) {
    return;  
  }else if (Buttons_Status & BUTTON_ENT) {
    if (Menu_Pointer<3) {
      Menu_Pointer++;
    }
  }else if (Buttons_Status & BUTTON_UP) {
    if (Menu_Pointer == 1) {
      Menu_Level_Main++;  
      if(Menu_Level_Main>MAX_SUBLEVEL_1) Menu_Level_Main=MAX_SUBLEVEL_1;  
    }
    if (Menu_Pointer == 2) {
      Menu_Level_Sub++;  
      if(Menu_Level_Sub>3) Menu_Level_Sub=3;  
    }
    if (Menu_Pointer == 3) {
      delta_Buttons++;
    }
  }else if (Buttons_Status & BUTTON_DOWN) {
    if (Menu_Pointer == 1) {
      Menu_Level_Main--;  
      if(Menu_Level_Main<0) Menu_Level_Main=0;  
    }
    if (Menu_Pointer == 2) {
      Menu_Level_Sub--;  
      if(Menu_Level_Sub<0) Menu_Level_Sub=0;  
    }
    if (Menu_Pointer == 3) {
      delta_Buttons--;
    }
  }else if (Buttons_Status & BUTTON_ESC) {
    if (Menu_Pointer>0) {
      Menu_Pointer--;
    }
  }
 
  Buttons_Status=0;
}
