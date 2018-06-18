#include <stdbool.h>       // false and true def
#include "8bkc-hal.h"      // kchalinit, etc. includes nvs functions
#include "8bkc-ugui.h"     // kcugui_cls
#include "powerbtn_menu.h" // Power Button menu stuff powerbtn_menu_show, constants, etc

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

void app_main() {
  kchal_init();  // Initialize the PocketSprite SDK.
  kcugui_init(); // Initialize ugui
  kcugui_cls();  // Initialize framebuffer to black
  while (1) {
    int btn = get_keydown();
    if (btn & KC_BTN_POWER) {
      do_powerbtn_menu();
    }
  }
}
