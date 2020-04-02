#define DISPLAY_COLOR_RECTANGLE          0x777777  //I bordi dei rettangoli
#define DISPLAY_COLOR_RECTANGLE_FILL     0x333333  //Il riempimento del rettangolo superiore e inferiore
#define DISPLAY_COLOR_CENTRAL_FILL       0x000000  //Il riempimento del rettangolo centrale

#define DISPLAY_COLOR_TITLE              0x00CCA9  //Il colore del titolo
#define DISPLAY_COLOR_TEMPERATURE        0xFAFC5B  //Il colore della scritta di temperatura
#define DISPLAY_COLOR_INFO_OK            0x00CCA9  //Il colore delle scritte in basso, se sono notizie positive

#define DISPLAY_COLOR_SELECT_MENU        0xFFD200  //Il colore del testo/selezione del menu'

#define DISPLAY_REDRAW_LIMIT             400       //Dopo quante scritture devo fare un redraw completo?

#define DELTA_RIGHE    18                          //Distanza in pixel tra le righe del menù (deve essere almeno 16)

unsigned short Number_of_redraw;

void Display_Init () {
  Serial.print("<DISPLAY> Init user interface...  ");

  Menu_Pointer          = 0;
  Menu_Level_Main       = 0;
  Menu_Level_Sub       = 0;
  Force_Complete_Redraw = true;
  Number_of_redraw      = 0;

  Serial.println("OK!");
}

void Display_Show () {
  static byte ex_Menu_Pointer = 255;

  //Rilevatore di variazioni di Menu_Pointer...
  //Entrando...
  if (ex_Menu_Pointer == 0 & Menu_Pointer == 1) {
    //Arrivo qui se siamo appena entrati nel menu principale
    Force_Complete_Redraw = true;
  }
  if (ex_Menu_Pointer == 1 & Menu_Pointer == 2) {
    //Arrivo qui se siamo appena entrati in uno dei sub-menu
    Display_clear_menu();
  }
  if (ex_Menu_Pointer == 2 & Menu_Pointer == 3) {
    //Arrivo qui se siamo appena entrati in uno dei sub-menu
    Display_clear_menu();
  }
  //Uscendo...
  if (ex_Menu_Pointer == 3 & Menu_Pointer == 2) {
    //Arrivo qui se siamo appena entrati in uno dei sub-menu
    Display_clear_menu();
  }
  if (ex_Menu_Pointer == 2 & Menu_Pointer == 1) {
    //Arrivo qui se siamo appena entrati in uno dei sub-menu
    Display_clear_menu();
    Menu_Level_Sub=0;
  }
  if (ex_Menu_Pointer == 1 & Menu_Pointer == 0) {
    //Arrivo qui se siamo appena entrati in uno dei sub-menu
    Force_Complete_Redraw = true;
    Menu_Level_Main=0;
  }

  if (Menu_Pointer == 0) {
    //Visualizzerò la homepage
    if (Force_Complete_Redraw == true) Display_HomeBackground();
    Display_HomeData();
  } else if (Menu_Pointer == 1) {
    //Visualizzerò il Main menu
    if (Force_Complete_Redraw == true) Display_MenuBackground();
    Display_Show_Main_Menu ();
  } else if (Menu_Pointer == 2) {
    if (Force_Complete_Redraw == true) Display_MenuBackground();
    Display_Show_Sub_Menu ();
  } else if (Menu_Pointer == 3) {
    if (Force_Complete_Redraw == true) Display_MenuBackground();
    Display_Show_Setting ();
  }

  ex_Menu_Pointer = Menu_Pointer;

  Number_of_redraw ++;
  if (Number_of_redraw >= DISPLAY_REDRAW_LIMIT) {
    //Richiedo un redraw
    Force_Complete_Redraw = true;
  } else {
    //Questo è l'unico punto che azzera la richiesta di Force_Complete_Redraw
    Force_Complete_Redraw = false;
  }
}

void Display_HomeBackground () {
  Serial.print("<DISPLAY> Complete redraw of homepage...  ");

  SSD2119DrawRectangle(2, 2, 318, 38, DISPLAY_COLOR_RECTANGLE);
  SSD2119DrawRectangleFill(3, 3, 317, 37, DISPLAY_COLOR_RECTANGLE_FILL);

  Bdrawstring (20, 5 , "Acquarium", DISPLAY_COLOR_TITLE, DISPLAY_COLOR_RECTANGLE_FILL);

  SSD2119DrawRectangle(2, 40, 318, 200, DISPLAY_COLOR_RECTANGLE);
  SSD2119DrawRectangleFill(3, 41, 317, 199, DISPLAY_COLOR_CENTRAL_FILL);

  Bdrawstring (30, 105 , "Temp:", DISPLAY_COLOR_TEMPERATURE, DISPLAY_COLOR_CENTRAL_FILL);
  Bdrawstring (30, 155 , "Mode:", DISPLAY_COLOR_TEMPERATURE, DISPLAY_COLOR_CENTRAL_FILL);

  SSD2119DrawRectangle(2, 202, 318, 238, DISPLAY_COLOR_RECTANGLE);
  SSD2119DrawRectangleFill(3, 203, 317, 237, DISPLAY_COLOR_RECTANGLE_FILL);

  Number_of_redraw = 0;

  Serial.println("OK!");
}

void Display_HomeData () {
  static byte what = 0;
  char txt[100];

  //Uso uno switch, con la variabile "what" che rolla, per visualizzare una cosa alla volta...
  //questo rende più reattivo il micro alla pressione dei tasti, perchè così è meno occupato
  //a disegnare sullo schermo
  switch (what) {
    case 0:
      //Orario, con font grande
      sprintf(txt, "%02i:%02i ", hours_RTC, minutes_RTC);
      Bdrawstring (30, 55 , txt, DISPLAY_COLOR_TEMPERATURE, DISPLAY_COLOR_CENTRAL_FILL);
      break;

    case 1:
      //Temperatura centrale, con font grande
      sprintf(txt, "%0.1f  ", Temperature_Real);
      Bdrawstring (130, 105 , txt , DISPLAY_COLOR_TEMPERATURE, DISPLAY_COLOR_CENTRAL_FILL);
      break;

    case 2:
      //Temperatura MIN e MAX
      sprintf(txt, "MAX: %0.1f  ", Temperature_Max);
      SSD2119DisplayString(220, 105, txt, 0xFF3A2F , DISPLAY_COLOR_CENTRAL_FILL);
      sprintf(txt, "MIN: %0.1f  ", Temperature_Min);
      SSD2119DisplayString(220, 124, txt, 0x0062FF , DISPLAY_COLOR_CENTRAL_FILL);
      break;

    case 3:
      //Allarmi e stati in basso
      sprintf(txt, "Light power: %d%%  ", Light_Percentage);
      SSD2119DisplayString(20, 210, txt, DISPLAY_COLOR_INFO_OK , DISPLAY_COLOR_RECTANGLE_FILL);
      break;

    case 4:
      //Modo funzionamento
      switch (Actual_Mode) {
        case 1: sprintf(txt, "Night  "); break;
        case 2: sprintf(txt, "Sunrise"); break;
        case 3: sprintf(txt, "Day    "); break;
        case 4: sprintf(txt, "Sunset "); break;
      }
      Bdrawstring (130, 155 , txt , DISPLAY_COLOR_TEMPERATURE, DISPLAY_COLOR_CENTRAL_FILL);
      break;
  }

  what ++;
  what = what % 5;

  /* //DEBUG - Visualizzo quale dei pulsanti è attivo.
    if (Buttons_Status==0) {
    sprintf(txt,  "No buttons ");
    } else if (Buttons_Status & BUTTON_ESC) {
    sprintf(txt,"Button ESC ");
    }else if (Buttons_Status & BUTTON_UP) {
    sprintf(txt,"Button UP  ");
    }else if (Buttons_Status & BUTTON_DOWN) {
    sprintf(txt,"Button DOWN");
    }else if (Buttons_Status & BUTTON_ENT) {
    sprintf(txt,"Button ENT ");
    }
    SSD2119DisplayString(200, 210, txt, DISPLAY_COLOR_INFO_OK ,DISPLAY_COLOR_RECTANGLE_FILL);
  */
}

void Display_MenuBackground () {
  Serial.print("<DISPLAY> Complete redraw of menu...  ");

  char txt[50];
  SSD2119DrawRectangle(2, 2, 318, 38, DISPLAY_COLOR_RECTANGLE);
  SSD2119DrawRectangleFill(3, 3, 317, 37, DISPLAY_COLOR_RECTANGLE_FILL);
  Bdrawstring (20, 5 , "Main menu", DISPLAY_COLOR_TITLE, DISPLAY_COLOR_RECTANGLE_FILL);

  SSD2119DrawRectangle(2, 40, 318, 200, DISPLAY_COLOR_RECTANGLE);
  //if (DISPLAY_COLOR_CENTRAL_FILL!=0)
  SSD2119DrawRectangleFill(3, 41, 317, 199, DISPLAY_COLOR_CENTRAL_FILL);

  SSD2119DrawRectangle(2, 202, 318, 238, DISPLAY_COLOR_RECTANGLE);
  SSD2119DrawRectangleFill(3, 203, 317, 237, DISPLAY_COLOR_RECTANGLE_FILL);
  sprintf(txt, "Press ESC for return to Homepage");
  SSD2119DisplayString(20, 210, txt, DISPLAY_COLOR_INFO_OK , DISPLAY_COLOR_RECTANGLE_FILL);

  Number_of_redraw = 0;

  Serial.println("OK!");
}

void Display_clear_menu() {
  for (int cont; cont < 5; cont++) {
    SSD2119DisplayString(30, 48 + (cont * DELTA_RIGHE), "                         " , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
  }
}

void Display_Show_Main_Menu () {
  char txt[50];

  //###################
  //PROFONDITA' 1 (main menu)
  //###################

  sprintf(txt, "Time set                 ");   Display_SubLevel1 (0, txt);
  sprintf(txt, "Light settings           ");   Display_SubLevel1 (1, txt);
  sprintf(txt, "Sunrise and Sunset       ");   Display_SubLevel1 (2, txt);
  sprintf(txt, "Turn on/off power plugs  ");   Display_SubLevel1 (3, txt);
  sprintf(txt, "Options                  ");   Display_SubLevel1 (4, txt);
}

void Display_SubLevel1 (byte index, const char* pcString) {
  if (Menu_Level_Main == index)
    SSD2119DisplayString(30, 48 + (index * DELTA_RIGHE), pcString, DISPLAY_COLOR_CENTRAL_FILL, DISPLAY_COLOR_SELECT_MENU);
  else
    SSD2119DisplayString(30, 48 + (index * DELTA_RIGHE), pcString, DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
}

void Display_Show_Sub_Menu() {
  char txt[50];

  //###################
  //PROFONDITA' 2 (sub menu)
  //###################

  //In base al valore del Menu_Level_Main, decido quale Sub Menù mostrare
  if (Menu_Level_Main == 0) {
    if (Menu_Level_Sub > 1) Menu_Level_Sub = 1;
    sprintf(txt, "Set Hour                 ");     Display_SubLevel2 (0, txt);
    sprintf(txt, "Set Minute               ");     Display_SubLevel2 (1, txt);
  }
  if (Menu_Level_Main == 1) {
    sprintf(txt, "PWM White cold           ");     Display_SubLevel2 (0, txt);
    sprintf(txt, "PWM White neutral        ");     Display_SubLevel2 (1, txt);
    sprintf(txt, "PWM White warm           ");     Display_SubLevel2 (2, txt);
    sprintf(txt, "PWM Red                  ");     Display_SubLevel2 (3, txt);
  }
  if (Menu_Level_Main == 2) {
    sprintf(txt, "Sunrise time             ");    Display_SubLevel2 (0, txt);
    sprintf(txt, "Sunrise duration         ");    Display_SubLevel2 (1, txt);
    sprintf(txt, "Sunset time              ");    Display_SubLevel2 (2, txt);
    sprintf(txt, "Sunset duration          ");    Display_SubLevel2 (3, txt);
  }
  if (Menu_Level_Main == 3) {
    sprintf(txt, "Heater                   ");     Display_SubLevel2 (0, txt);
    sprintf(txt, "Light                    ");     Display_SubLevel2 (1, txt);
    sprintf(txt, "Filter pump              ");     Display_SubLevel2 (2, txt);
    sprintf(txt, "Skimmer                  ");     Display_SubLevel2 (3, txt);
  }
  if (Menu_Level_Main == 4) {
    sprintf(txt, "Save settings in EEPROM  ");     Display_SubLevel2 (0, txt);
    sprintf(txt, "Reset T_min and T_max    ");     Display_SubLevel2 (1, txt);
    sprintf(txt, "Reload EEPROM            ");     Display_SubLevel2 (2, txt);
    sprintf(txt, "About                    ");     Display_SubLevel2 (3, txt);
  }
}

void Display_Show_Setting () {
  char txt[50];

  //###################
  //PROFONDITA' 3 (settings)
  //###################


  if (Menu_Level_Main == 0) {
    switch (Menu_Level_Sub) {
      case 0: Display_SubLevel3 ("Hour", &hours_RTC, 0, 23, 1 ); PCF8583_set_time();  break;
      case 1: Display_SubLevel3 ("Minute", &minutes_RTC, 0, 59, 1 ); PCF8583_set_time();  break;
    }
  } else if (Menu_Level_Main == 1) {
    switch (Menu_Level_Sub) {
      case 0: Display_SubLevel3 ("PWM White cold", &PWM_1_day, 0, 255, 1 );  break;
      case 1: Display_SubLevel3 ("PWM White neutral", &PWM_2_day, 0, 255, 1 );  break;
      case 2: Display_SubLevel3 ("PWM White hot", &PWM_3_day, 0, 255, 1 );  break;
      case 3: Display_SubLevel3 ("PWM Red", &PWM_4_day, 0, 255, 1 );  break;
    }
  } else if (Menu_Level_Main == 2) {
    switch (Menu_Level_Sub) {
      case 0: Display_SubLevel3 ("Sunrise time", &Sunrise_time , 300, 720, 2 );  break;
      case 1: Display_SubLevel3 ("Sunrise duration", &Sunrise_duration, 0, 120, 2  );  break;
      case 2: Display_SubLevel3 ("Sunset time", &Sunset_time, 780, 1320, 2  );  break;
      case 3: Display_SubLevel3 ("Sunset duration", &Sunset_duration, 0, 120, 2  );  break;
    }
  } else if (Menu_Level_Main == 3) {
    switch (Menu_Level_Sub) {
      case 0: Display_SubLevel3 ("Heater", &Heater, 0, 1, 3 );  break;
      case 1: Display_SubLevel3 ("Light", &Auxiliary, 0, 1, 3 );  break;
      case 2: Display_SubLevel3 ("Filter pump", &Pump_1, 0, 1, 3 );  break;
      case 3: Display_SubLevel3 ("Skimmer", &Pump_2, 0, 1, 3 );  break;
    }
  } else if (Menu_Level_Main == 4) {
    switch (Menu_Level_Sub) {
      case 0: 
        sprintf(txt, "%s", "Settings saved!");
        SSD2119DisplayString(30, 48 + (0 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
        Eeprom_Save ();
        Menu_Pointer--;
        break;
      case 1:
        sprintf(txt, "%s", "Tmin and Tmax resetted!");
        SSD2119DisplayString(30, 48 + (0 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
        Temperature_Min=Temperature_Real;
        Temperature_Max=Temperature_Real;
        Menu_Pointer--;
        break;
      case 2:
        sprintf(txt, "%s", "Settings reloaded!");
        SSD2119DisplayString(30, 48 + (0 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
        Eeprom_Read ();
        Menu_Pointer--;
        break;
      case 3:
        sprintf(txt, "%s", "About");
        SSD2119DisplayString(30, 48 + (0 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
        sprintf(txt, "%s", "");
        SSD2119DisplayString(30, 48 + (1 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
        sprintf(txt, "%s", "Compiled on:");
        SSD2119DisplayString(30, 48 + (2 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
        sprintf(txt, "%s - %s", __DATE__, __TIME__); //https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html
        SSD2119DisplayString(30, 48 + (3 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
        break;
    }
  }
}

void Display_SubLevel2 (byte index, const char* pcString) {
  if (Menu_Level_Sub == index)
    SSD2119DisplayString(30, 48 + (index * DELTA_RIGHE), pcString, DISPLAY_COLOR_CENTRAL_FILL, DISPLAY_COLOR_SELECT_MENU);
  else
    SSD2119DisplayString(30, 48 + (index * DELTA_RIGHE), pcString, DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
}

void Display_SubLevel3 (const char* pcString, int* value, int minimum, int maximum, byte kind ) {
  char txt[50];

  //kind=1 > Visualizzazione naturale
  //kind=2 > Visualizzazione con input=minuti e output=hh:mm
  //kind=3 > Visualizzazione con input=1/0 e output=ON/OFF

  //Calcolo il nuovo valore, se necessario
  *value = *value + delta_Buttons;
  delta_Buttons = 0;

  //Verifico che il valore non sia andato fuori range
  if (*value > maximum) *value = maximum;
  if (*value < minimum) *value = minimum;

  //Per prima cosa mostro il nuovo valore, perchè aumenta l'esperienza utente.
  if (kind==3) {
    if (*value == 0) {
      sprintf(txt, "%s ", "OFF");
    } else {
      sprintf(txt, "%s ", "ON");
    }
    SSD2119DisplayString(30, 48 + (2 * DELTA_RIGHE), txt , DISPLAY_COLOR_CENTRAL_FILL, DISPLAY_COLOR_SELECT_MENU);

    sprintf(txt, "%s", "ON or OFF");
    SSD2119DisplayString(30, 48 + (3 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
  } else if (kind==1) {
    sprintf(txt, "%d  ", *value);
    SSD2119DisplayString(30, 48 + (2 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);

    sprintf(txt, "Min: %d    Max: %d ", minimum, maximum);
    SSD2119DisplayString(30, 48 + (3 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);
  } else if (kind==2) {
    int ore;
    int minuti;

    ore=*value/60;
    minuti=*value - (ore*60);

    sprintf(txt, "%02d:%02d  ", ore, minuti);
    SSD2119DisplayString(30, 48 + (2 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);

    sprintf(txt, "%02d:%02d ... %02d:%02d", int(minimum/60), int(minimum-(60*(minimum/60))), int(maximum/60), int(maximum-(60*(maximum/60))));
    SSD2119DisplayString(30, 48 + (3 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);

  }

  //Poi mostro il titolo della schermata
  sprintf(txt, "%s", pcString);
  SSD2119DisplayString(30, 48 + (0 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);

  //... la scritta fissa "Value:"
  sprintf(txt, "%s", "Value:");
  SSD2119DisplayString(30, 48 + (1 * DELTA_RIGHE), txt , DISPLAY_COLOR_SELECT_MENU, DISPLAY_COLOR_CENTRAL_FILL);

}
