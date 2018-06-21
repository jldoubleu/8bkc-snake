#include <stdint.h>        // unit16_t
#include <string.h>        // strlen
#include <stdbool.h>       // false and true def
#include "8bkc-hal.h"      // kchalinit, etc. includes nvs functions
#include "8bkc-ugui.h"     // kcugui_cls
#include "powerbtn_menu.h" // Power Button menu stuff powerbtn_menu_show, constants, etc
#include "ugui.h"

#define TITLE "Snake"
#define START_MSG "Press Start"

static void do_powerbtn_menu() {
  int i = powerbtn_menu_show(kcugui_get_fb());
  if (i == POWERBTN_MENU_EXIT) {
    kchal_exit_to_chooser();
  }
  if (i == POWERBTN_MENU_POWERDOWN) {
    kchal_power_down();
  }
}

//Get bitmap of keys, but only put an 1 for a key that wasn't pressed the previous call but
//is pressed now.
int get_keydown() {
  static int oldBtns = 0xffff; // <-- static int be careful
  int newBtns = kchal_get_keys();
  int ret = (oldBtns ^ newBtns) & newBtns;
  oldBtns = newBtns;
  return ret;
}

uint16_t centered_text_offset(char* text, uint8_t font_width) {
  size_t text_length = strlen(text) + 1; 
  return (KC_SCREEN_W / 2) - ((text_length / 2) * font_width);
}

void draw_title_screen() {
  uint8_t font_width = 6;
  
  kcugui_cls();
  UG_FontSelect(&FONT_6X8);
  UG_SetForecolor(C_LAWN_GREEN);
  UG_PutString(0, 0, TITLE);
  UG_PutString(centered_text_offset(TITLE, font_width), 0, TITLE);
  // Start message is a little too long, cut out the spaces
  UG_FontSetHSpace(0);
  UG_SetForecolor(C_TOMATO);
  UG_PutString(centered_text_offset(START_MSG, font_width), KC_SCREEN_H / 2, START_MSG);
  kcugui_flush();
}

void do_title_screen() {
  draw_title_screen();
  while (1) {
    int btn = get_keydown();
    if (btn & KC_BTN_POWER) {
      do_powerbtn_menu();
    }
    if(btn & KC_BTN_START) {
      // start the game on start;
      break;
    }
  }
}

void app_main() {
  kchal_init();  // Initialize the PocketSprite SDK.
  kcugui_init(); // Initialize ugui

  while (1) {
    do_title_screen();
    kchal_exit_to_chooser();
  }
}
