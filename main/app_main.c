#include <stdint.h>        // unit16_t
#include <string.h>        // strlen
#include <stdbool.h>       // false and true def
#include "8bkc-hal.h"      // kchalinit, etc. includes nvs functions
#include "8bkc-ugui.h"     // kcugui_cls
#include "powerbtn_menu.h" // Power Button menu stuff powerbtn_menu_show, constants, etc
#include "ugui.h"

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

void draw_title_screen() {
  char* title = "Snake\n";
  size_t title_length = strlen(title); 
  uint8_t font_width = 6;
  uint16_t offset = (KC_SCREEN_W - (title_length * font_width)) / 2;
  
  kcugui_cls();
  UG_FontSelect(&FONT_6X8);
  UG_SetForecolor(C_LAWN_GREEN);
  UG_PutString(offset, 0, title);
}

void app_main() {
  kchal_init();  // Initialize the PocketSprite SDK.
  kcugui_init(); // Initialize ugui
  draw_title_screen();
  while (1) {
    int btn = get_keydown();
    if (btn & KC_BTN_POWER) {
      do_powerbtn_menu();
    }
  }
}
