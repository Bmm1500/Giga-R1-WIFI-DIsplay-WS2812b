# 🎛️ Giga LED Touch Display

Control interactiv pentru o matrice LED WS2812B 16x16 cu Arduino GIGA R1 WiFi și GIGA Display Shield, folosind o interfață touchscreen cu 8 slidere pentru parametri personalizabili.

![Arduino GIGA R1 + WS2812B + Touchscreen](https://upload.wikimedia.org/wikipedia/commons/thumb/b/bf/WS2812B_RGB_LED.jpg/320px-WS2812B_RGB_LED.jpg)

## 🧠 Descriere

Proiectul permite controlul în timp real al unei matrice de LED-uri printr-o interfață tactilă folosind biblioteca **LVGL**. Sunt disponibile mai multe efecte vizuale (curcubeu, spirală, flăcări etc.) și opțiuni de reglaj live pentru:

- Culoare roșu / verde / albastru (RGB)
- Luminozitate globală
- Delay (viteză)
- Fade (efect de atenuare)
- Tipul de efect (slider pentru selecție rapidă)
- Setare automată sau manuală

## 🧰 Hardware folosit

- ✅ [Arduino GIGA R1 WiFi](https://store.arduino.cc/products/arduino-giga-r1-wifi)
- ✅ [GIGA Display Shield](https://store.arduino.cc/products/giga-display-shield)
- ✅ Matrice LED WS2812B 16x16 (256 LED-uri)
- ✅ Sursă de alimentare externă pentru LED-uri (5V, minim 10A)
- ✅ Fire de conexiune, rezistor 330Ω și condensator 1000uF (pentru protecție)

## 🧾 Biblioteci necesare

Instalează din Library Manager sau manual:

- [`FastLED`](https://github.com/FastLED/FastLED)
- [`lvgl`](https://github.com/lvgl/lvgl)
- `Arduino_GigaDisplayTouch` *(pentru touchscreen-ul GIGA)*
- `Arduino_GigaDisplay_GFX` *(pentru desenare pe ecran)*

## 🖥️ Funcționalități

- Interfață touchscreen cu 8 slidere pentru control:
  - R, G, B
  - Delay
  - Fade ( valoarea fadeToBlackBy(leds,NUM_LEDS,fade))
  - Luminozitate
  - Auto mode
  - Selectare efect  
- Efecte disponibile:
  - 🌈 Rainbow
  - umplere pe linii si coloane
- Actualizare live fără a opri codul
- Reset rapid la parametrii default

## 🧮 Wiring (conectare)

```text
WS2812B DIN → Arduino GIGA Pin 8
WS2812B GND → GND Arduino
WS2812B +5V → Sursă 5V externă (NU din Arduino)
