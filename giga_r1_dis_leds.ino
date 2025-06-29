#include "Arduino_H7_Video.h"
#include "Arduino_GigaDisplayTouch.h"
#include "lvgl.h"
#include <FastLED.h>

Arduino_H7_Video Display(800, 480, GigaDisplayShield);
Arduino_GigaDisplayTouch TouchDetector;

// LED setup
#define LED_PIN     8
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_COL     16
#define NUM_ROW     16
#define NUM_LEDS    NUM_ROW * NUM_COL

CRGB leds[NUM_LEDS];

// Parametri controlabili
int a = 0;  // roșu
int b = 0;  // verde
int c = 0;  // albastru
int d = 25; // delay
int e = 1;  // numărul efectului
int f = 50; // luminozitate
int g = 50; // fadeToBlack
int h = 0;  // rezervat

// Structură pentru slider
struct SliderContext {
  int* param;
  lv_obj_t* valueLabel;
  int min;
  int max;
};

// Callback pentru slider
static void slider_event_cb(lv_event_t * e) {
  SliderContext* ctx = (SliderContext*)lv_event_get_user_data(e);
  lv_obj_t* slider = (lv_obj_t*)lv_event_get_target(e);

  *(ctx->param) = lv_slider_get_value(slider);

  char buf[32];
  sprintf(buf, "Valoare: %d (%d - %d)", *(ctx->param), ctx->min, ctx->max);
  lv_label_set_text(ctx->valueLabel, buf);
}

void setup() {
  Serial.begin(9600);

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(200);

  Display.begin();
  TouchDetector.begin();

  // Layout grid
  static lv_coord_t col_dsc[] = {185, 185, 185, 185, LV_GRID_TEMPLATE_LAST};
  static lv_coord_t row_dsc[] = {220, 220, LV_GRID_TEMPLATE_LAST};

  lv_obj_t* cont = lv_obj_create(lv_scr_act());
  lv_obj_set_grid_dsc_array(cont, col_dsc, row_dsc);
  lv_obj_set_size(cont, Display.width(), Display.height());
  lv_obj_center(cont);

  // Avem 8 slidere
  static SliderContext ctx[8];

  // Funcție pentru adăugare slider
  auto add_slider = [&](int col, int row, int* param, const char* label_text, int index, int min, int max) {
    lv_obj_t* cell = lv_obj_create(cont);
    lv_obj_set_grid_cell(cell, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

    lv_obj_t* label = lv_label_create(cell);
    lv_label_set_text(label, label_text);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 5);

    lv_obj_t* slider = lv_slider_create(cell);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, *param, LV_ANIM_OFF);
    lv_obj_set_width(slider, 130);
    lv_obj_align(slider, LV_ALIGN_CENTER, 0, 0);
    //lv_obj_set_style_bg_opa(slider, LV_OPA_TRANSP, LV_PART_INDICATOR); // ascunde bara activă
    //lv_obj_set_style_bg_opa(slider, LV_OPA_TRANSP, LV_PART_MAIN);      // ascunde fundalul

    lv_obj_t* valLabel = lv_label_create(cell);
    char buf[32];
    sprintf(buf, "Valoare: %d (%d - %d)", *param, min, max);
    lv_label_set_text(valLabel, buf);
    lv_obj_align(valLabel, LV_ALIGN_BOTTOM_MID, 0, -5);

    ctx[index].param = param;
    ctx[index].valueLabel = valLabel;
    ctx[index].min = min;
    ctx[index].max = max;

    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, &ctx[index]);
  };

  // Adăugare slidere în grilă 4x2
  add_slider(0, 0, &a, "Rosu",       0, 0, 255);
  add_slider(1, 0, &b, "Verde",      1, 0, 255);
  add_slider(2, 0, &c, "Albastru",   2, 0, 255);
  add_slider(3, 0, &d, "Intarziere", 3, 5, 100);

  add_slider(0, 1, &e, "Efect",      4, 1, 8);
  add_slider(1, 1, &f, "Luminoz.",   5, 0, 255);
  add_slider(2, 1, &g, "FadeBlack",  6, 0, 255);
  add_slider(3, 1, &h, "Viteza",     7, 0, 255);

  FastLED.clear();

  Serial.println(" ");
  Serial.println("--------------------------------------------------------------------------");
  Serial.println(" ");
  Serial.println("DESCRIERE PROGRAM : Program prin care am facut platforma pentru efecte cu leduri controlate prin intermedul touch diplay");
  Serial.println("Placa : Giga R1 wifi ");
  Serial.println("Port serial setat la : 9600");
  Serial.println("Locatia programului (calea): %/Arduino/Arduino giga r1/giga_r1_dis_leds.ino");
  Serial.print(" Nr. Leduri = ");Serial.println(NUM_LEDS);
  //Serial.print(" Delay 1 = ");Serial.println(d1);
  //Serial.print(" Delay 2 = ");Serial.println(d2);
  //Serial.print(" Interval = ");Serial.println(interval);
  Serial.println("Revizia : ");
  Serial.println("--------------------------------------------------------------------------");
  Serial.println(" ");
}

/*
variabile 
a=rosu  | b=verde | c=albastru | d=delayTime | e=numar efect | f= stralucire | g= fade | h= ??? |
*/

void loop() {
  lv_timer_handler();  // actualizează LVGL

  switch (e) {
    case 1: efect1(a, b, c, d, f,g); break;            // cometa pe linie sens +
    case 2: efect2(a, b, c, d, f,g); break;            // cometa pe linie sens - 
    case 3: efectZigZag(a, b, c, d,f,g); break;          // cometa pe coloana sens + 
    case 4: efectZigZagInvers(a, b, c, d,f,g); break;    // cometa pe coloana sens - 
    case 5: efect3(a, b, c, d, f); break;            // respiratie
    case 6: efectRainbow(d,g); break;               // curcubeu
    case 7: efectValuri(a,b,c,d,f); break;
    case 8: efectAprind(d); break;
  }
}


//---------------------------------------------------------------------- efect 1
void efect1(int red, int green, int blue, int delayTime, int bri,int fade) {
  static uint16_t index = 0;
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate > delayTime) {
    leds[index] = CRGB(red, green, blue);
    FastLED.setBrightness(bri);
    FastLED.show();
    fadeToBlackBy(leds, NUM_LEDS, fade);
    index++;
    if (index >= NUM_LEDS) index = 0;
    lastUpdate = millis();
  }
}

//---------------------------------------------------------------------- efect 2
void efect2(int red, int green, int blue, int delayTime, int bri,int fade) {
  static uint16_t index = NUM_LEDS;
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate > delayTime) {
    leds[index] = CRGB(red, green, blue);
    FastLED.setBrightness(bri);
    FastLED.show();
    fadeToBlackBy(leds, NUM_LEDS, fade);
    index--;
    if (index <= 0) index = NUM_LEDS - 1;
    lastUpdate = millis();
  }
}

//---------------------------------------------------------------------- efect 3 - 4
// Conversie coordonate (x,y) în index pentru matrice zig-zag
int XY(int x, int y) {
  if (y % 2 == 0) {
    return y * NUM_ROW + x;
  } else {
    return y * NUM_ROW + (NUM_ROW - 1 - x);
  }
}

//---------------------------------------------------------------------- efect 3
void efectZigZag(int r, int g, int b, int delayTime,int bri,int fade) {
  static unsigned long lastUpdate = 0;
  static int x = 0;
  static int y = 0;

  if (millis() - lastUpdate > delayTime) {
    int py = (x % 2 == 0) ? y : NUM_COL - 1 - y;
    leds[XY(x, py)] = CRGB(r, g, b);
    FastLED.setBrightness(bri);
    FastLED.show();
    fadeToBlackBy(leds, NUM_LEDS, fade);

    y++;
    if (y >= NUM_COL) {
      y = 0;
      x++;
      if (x >= NUM_ROW) {
        x = 0; // reîncepe efectul
      }
    }
    lastUpdate = millis();
  }
}

//---------------------------------------------------------------------- efect 4
void efectZigZagInvers(int r, int g, int b, int delayTime, int bri, int fade) {
  static unsigned long lastUpdate = 0;
  static int x = NUM_ROW - 1;
  static int y = NUM_COL - 1;

  if (millis() - lastUpdate > delayTime) {
    int py = (x % 2 == 0) ? y : NUM_COL - 1 - y;
    leds[XY(x, py)] = CRGB(r, g, b);
    FastLED.setBrightness(bri);
    FastLED.show();
    fadeToBlackBy(leds, NUM_LEDS, fade);
    y--;
    if (y < 0) {
      y = NUM_COL - 1;
      x--;
      if (x < 0) {
        x = NUM_ROW - 1;  // reîncepe efectul de la capăt
      }
    }
    lastUpdate = millis();
  }
}

//---------------------------------------------------------------------- efect 5
void efect3(int red, int green, int blue, int delayTime, int bri) {
  static int brightness = 0;
  static int direction = 1;
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate > delayTime) {
    brightness += direction * 5;
    if (brightness >= bri) {
      brightness = bri;
      direction = -1;
    }
    if (brightness <= 0) {
      brightness = 0;
      direction = 1;
    }

    fill_solid(leds, NUM_LEDS, CRGB(red, green, blue));
    FastLED.setBrightness(brightness);
    FastLED.show();
    lastUpdate = millis();
  }
}

//---------------------------------------------------------------------- efect 6
/*
void efectRainbow(uint8_t delayTime, uint8_t bri) {
  static uint8_t hue = 0;
  static unsigned long lastUpdate = 0;
  static int i=0;

  if (millis() - lastUpdate > delayTime) {  
  leds[i]=CHSV(hue++,250,250);
  FastLED.setBrightness(bri);
  FastLED.show();
  i++;
  if(i == NUM_LEDS){i=0;}
  lastUpdate = millis();
  }
}
*/

void efectRainbow(uint8_t delayTime, int fade) {
  static unsigned long lastUpdate = 0;
  static int x = 0, y = 0, hue=0;
  static int step = 0; // 0 = umple linii, 1 = umple coloane
  static bool done = false;

  if (millis() - lastUpdate > delayTime && !done) {
    if (step == 0) {
      // Umplere pe linii
      leds[XY(x, y)] = CHSV(hue++,250,250);
      FastLED.show();
      fadeToBlackBy(leds,NUM_LEDS,fade);

      x++;
      if (x >= NUM_ROW) {
        x = 0;
        y++;
        if (y >= NUM_COL) {
          step = 1; // Treci la umplere pe coloane
          x = 0;
          y = 0;
        }
      }
    } else if (step == 1) {
      // Umplere pe coloane
      leds[XY(x, y)] = CHSV(hue++,250,250);
      FastLED.show();
      fadeToBlackBy(leds,NUM_LEDS,fade);

      y++;
      if (y >= NUM_COL) {
        y = 0;
        x++;
        if (x >= NUM_ROW) {
          step=0;
          x=0;
          y=0;
        }
      }
    }
    lastUpdate = millis();
  }
}

//---------------------------------------------------------------------- efect 7
void efectValuri(int r, int g, int b, int delayTime, float viteza) {
  static unsigned long lastUpdate = 0;
  static int t = 0;

  if (millis() - lastUpdate >= delayTime) {
    FastLED.clear();
    for (int x = 0; x < NUM_ROW; x++) {
      float yVal = (sin((x + t * viteza) * 0.4) + 1.0) * (NUM_COL / 2.0 - 1);
      int y = round(yVal);
      leds[XY(x, y)] = CRGB(r, g, b);
    }
    FastLED.show();
    t++;
    lastUpdate = millis();
  }
}
