#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
#    include "lufa.h"
#    include "split_util.h"
#endif
#ifdef SSD1306OLED
#    include "ssd1306.h"
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

enum layer_number { _QWERTY = 0, _MD, _NU, _MV };

enum custom_keycodes { QWERTY = SAFE_RANGE, RGBRST };

enum macro_keycodes {
    KC_SAMPLEMACRO,
};

#define KC______ KC_TRNS
#define KC_XXXXX KC_NO
#define KC_RST RESET

#define KC_SPC_MV LT(_MV, KC_SPC)
#define KC_ENT_NU LT(_NU, KC_ENT)
#define KC_ESC_MD LT(_MD, KC_ESC)
#define KC_DEL_MD LT(_MD, KC_DEL)
#define KC_BSP_SF LSFT_T(KC_BSPC)
#define KC_TAB_SF LSFT_T(KC_TAB)

#define KC_GUI_X LGUI_T(KC_X)
#define KC_ALT_C LALT_T(KC_C)
#define KC_CTL_V LCTL_T(KC_V)

#define KC_GUI_DT LGUI_T(KC_COMM)
#define KC_ALT_CM LALT_T(KC_DOT)
#define KC_CTL_M LCTL_T(KC_M)

// ==== Key Map ====

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_kc(
//,-----------------------------------------.                ,-----------------------------------------.
      GRV,     Q,     W,     E,     R,     T,                      Y,     U,     I,     O,     P,   EQL,
//|------+------+------+------+------+------|                |------+------+------+------+------+------|
     LBRC,     A,     S,     D,     F,     G,                      H,     J,     K,     L,  SCLN,  QUOT,
//|------+------+------+------+------+------|                |------+------+------+------+------+------|
     RBRC,     Z,     X,     C,     V,     B,                      N,     M,  COMM,   DOT,  SLSH,  MINS,
//|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                               ESC_MD,TAB_SF,ENT_NU,   SPC_MV,BSP_SF,DEL_MD
//                            `--------------------'  `--------------------'
    ),
    [_MD] = LAYOUT_kc(
//,-----------------------------------------.                ,-----------------------------------------.
    _____, _____, _____, _____, _____, _____,                  _____, _____, _____, _____, _____, _____,
//|------+------+------+------+------+------|                |------+------+------+------+------+------|
    _____, _____, _____, _____, _____, _____,                  _____, _____, _____, _____, _____, _____,
//|------+------+------+------+------+------|                |------+------+------+------+------+------|
    _____, _____, GUI_X, ALT_C, CTL_V, _____,                  _____, CTL_M,ALT_CM,GUI_DT, _____, _____,
//|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                _____, _____, _____,    _____, _____, _____
//                            `--------------------'  `--------------------'
    ),
    [_NU] = LAYOUT_kc(
//,-----------------------------------------.                ,-----------------------------------------.
     PIPE,     1,     2,     3,     4,     5,                      6,     7,     8,     9,     0,  BSLS,
//|------+------+------+------+------+------|                |------+------+------+------+------+------|
     PIPE,  EXLM,    AT,  HASH,   DLR,  PERC,                   CIRC,  AMPR,  ASTR,  LPRN,  RPRN,  BSLS,
//|------+------+------+------+------+------|                |------+------+------+------+------+------|
    _____, _____, _____, _____, _____, _____,                  _____, _____, _____, _____, _____, _____,
//|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                _____, _____, _____,    _____, _____, _____
//                            `--------------------'  `--------------------'
    ),
    [_MV] = LAYOUT_kc(
//,-----------------------------------------.                ,-----------------------------------------.
    _____,    F1,    F2,    F3,    F4,    F5,                     F6,    F7,    F8,    F9,   F10, _____,
//|------+------+------+------+------+------|                |------+------+------+------+------+------|
    _____, _____, _____, _____, _____, _____,                   LEFT,  DOWN,    UP, RIGHT, _____, _____,
//|------+------+------+------+------+------|                |------+------+------+------+------+------|
      RST, _____, _____, _____, _____, _____,                  _____, _____, _____, _____, _____, _____,
//|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
                                _____, _____, _____,    _____, _____, _____
//                            `--------------------'  `--------------------'
    )
};

// === user record ===

void persistent_default_layer_set(uint16_t default_layer) {
    eeconfig_update_default_layer(default_layer);
    default_layer_set(default_layer);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QWERTY:
            if (record->event.pressed) {
                persistent_default_layer_set(1UL << _QWERTY);
            }
            return false;
            break;
    }
    return true;
}
