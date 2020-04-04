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

enum layer_number { _QWERTY = 0, _NAVI, _NUMB, _ADJUST };

enum custom_keycodes { QWERTY = SAFE_RANGE, RGBRST };

enum macro_keycodes {
    KC_SAMPLEMACRO,
};

#define KC______ KC_TRNS
#define KC_XXXXX KC_NO
#define KC_RST RESET
#define KC_TOG RGB_TOG
#define KC_HUI RGB_HUI
#define KC_HUD RGB_HUD
#define KC_SAI RGB_SAI
#define KC_SAD RGB_SAD
#define KC_VAI RGB_VAI
#define KC_VAD RGB_VAD

#define KC_NVSPC LT(_NAVI, KC_SPC)
#define KC_NBENT LT(_NUMB, KC_ENT)
#define KC_AJDEL LT(_ADJUST, KC_DEL)

#define KC_GUI_S LGUI_T(KC_S)
#define KC_ALT_D LALT_T(KC_D)
#define KC_CTL_F LCTL_T(KC_F)

#define KC_GUI_L LGUI_T(KC_L)
#define KC_ALT_K RALT_T(KC_K)
#define KC_CTL_J RCTL_T(KC_J)

#define KC_S_BSP LSFT_T(KC_BSPC)
#define KC_S_TAB RSFT_T(KC_TAB)

// ==== Tap Dance ====

enum { TD_UP_PGUP = 0, TD_DOWN_PGDN, TD_PRN_OPEN, TD_PRN_CLOSE };

typedef enum { SINGLE_TAP = 1, DOUBLE_TAP = 2, TRIPLE_TAP = 3 } td_state_t;

static td_state_t td_state;

int cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        return SINGLE_TAP;
    }
    if (state->count == 2) {
        return DOUBLE_TAP;
    }
    if (state->count == 3) {
        return TRIPLE_TAP;
    } else
        return 8;
}

void prn_open_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case SINGLE_TAP:
            register_code16(KC_LPRN);
            break;
        case DOUBLE_TAP:
            register_code16(KC_LBRC);
            break;
        case TRIPLE_TAP:
            register_code16(KC_LCBR);
            break;
    }
}

void prn_open_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP:
            unregister_code16(KC_LPRN);
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_LBRC);
            break;
        case TRIPLE_TAP:
            unregister_code16(KC_LCBR);
            break;
    }
}

void prn_close_finished(qk_tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case SINGLE_TAP:
            register_code16(KC_RPRN);
            break;
        case DOUBLE_TAP:
            register_code16(KC_RBRC);
            break;
        case TRIPLE_TAP:
            register_code16(KC_RCBR);
            break;
    }
}

void prn_close_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case SINGLE_TAP:
            unregister_code16(KC_RPRN);
            break;
        case DOUBLE_TAP:
            unregister_code16(KC_RBRC);
            break;
        case TRIPLE_TAP:
            unregister_code16(KC_RCBR);
            break;
    }
}

qk_tap_dance_action_t tap_dance_actions[] = {
    //  [TD_UP_PGUP] = ACTION_TAP_DANCE_DOUBLE(KC_UP, KC_PGUP),
    //  [TD_DOWN_PGDN] = ACTION_TAP_DANCE_DOUBLE(KC_DOWN, KC_PGDN),
    [TD_PRN_OPEN]  = ACTION_TAP_DANCE_FN_ADVANCED(NULL, prn_open_finished, prn_open_reset),
    [TD_PRN_CLOSE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, prn_close_finished, prn_close_reset)};

// #define KC_TD_UP TD(TD_UP_PGUP)
// #define KC_TD_DN TD(TD_DOWN_PGDN)
#define KC_TD_PO TD(TD_PRN_OPEN)
#define KC_TD_PC TD(TD_PRN_CLOSE)

// ==== Key Map ====

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT_kc(
//,-----------------------------------------.                ,-----------------------------------------.
        GRV, Q, W, E, R, T, Y, U, I, O, P, EQL,
//|------+------+------+------+------+------|                |------+------+------+------+------+------|
        TD_PO, A, GUI_S, ALT_D, CTL_F, G, H, CTL_J, ALT_K, GUI_L, SCLN, QUOT,  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        TD_PC, Z, X, C, V, B, N, M, COMM, DOT, SLSH, MINS,                     //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
        ESC, S_TAB, NBENT, NVSPC, S_BSP, AJDEL                                 //`--------------------'  `--------------------'
        ),

    [_NUMB] = LAYOUT_kc(                                                     //,-----------------------------------------.                ,-----------------------------------------.
        _____, _____, EXLM, AT, HASH, _____, 0, 1, 2, 3, _____, _____,       //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        _____, _____, DLR, PERC, CIRC, _____, _____, 4, 5, 6, _____, _____,  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        _____, _____, AMPR, ASTR, PIPE, BSLS, _____, 7, 8, 9, _____, _____,  //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
        _____, _____, _____, _____, _____, _____                             //`--------------------'  `--------------------'
        ),

    [_NAVI] = LAYOUT_kc(                                                                  //,-----------------------------------------.                ,-----------------------------------------.
        _____, _____, _____, _____, _____, _____, PGUP, HOME, UP, END, _____, _____,      //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        _____, _____, _____, _____, _____, _____, PGDN, LEFT, DOWN, RIGHT, _____, _____,  //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        _____, UNDO, CUT, COPY, PASTE, _____, _____, _____, _____, _____, _____, _____,   //|------+------+------+------+------+------+------|  |------+------+------+------+------+------+------|
        _____, _____, _____, _____, _____, _____                                          //`--------------------'  `--------------------'
        ),

    [_ADJUST] = LAYOUT_kc(                                                                   //,-----------------------------------------.                ,-----------------------------------------.
        RST, HUI, SAI, VAI, _____, _____, _____, F1, F2, F3, _____, _____,                   //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        TOG, HUD, SAD, VAD, _____, _____, _____, _____, _____, _____, _____, _____,          //|------+------+------+------+------+------|                |------+------+------+------+------+------|
        _____, _____, _____, _____, _____, _____, _____, _____, _____, _____, _____, _____,  //|------+------+------+-,-----+------+------+------|  |------+------+------+------+------+------+------|
        _____, _____, _____, _____, _____, _____                                             //`--------------------'  `--------------------'
        ),

};

// TODO: num and shift lock keys
// ==== Layer RGB Light ====

int active_layer;

uint32_t layer_state_set_user(uint32_t state) {
    switch (biton32(state)) {
        case _NUMB:
            active_layer = _NUMB;
            if (!isLeftHand) {
                rgblight_setrgb_at(RGB_PURPLE, 9);
                rgblight_setrgb_at(RGB_PURPLE, 10);
                rgblight_setrgb_at(RGB_PURPLE, 11);
                rgblight_setrgb_at(RGB_PURPLE, 12);
                rgblight_setrgb_at(RGB_PURPLE, 15);
                rgblight_setrgb_at(RGB_PURPLE, 16);
                rgblight_setrgb_at(RGB_PURPLE, 17);
                rgblight_setrgb_at(RGB_PURPLE, 18);
                rgblight_setrgb_at(RGB_PURPLE, 19);
                rgblight_setrgb_at(RGB_PURPLE, 20);
            } else {
                rgblight_setrgb_at(RGB_CYAN, 6);
                rgblight_setrgb_at(RGB_GREEN, 7);
                rgblight_setrgb_at(RGB_GREEN, 10);
                rgblight_setrgb_at(RGB_GREEN, 11);
                rgblight_setrgb_at(RGB_GREEN, 12);
                rgblight_setrgb_at(RGB_GREEN, 15);
                rgblight_setrgb_at(RGB_GREEN, 16);
                rgblight_setrgb_at(RGB_GREEN, 17);
                rgblight_setrgb_at(RGB_GREEN, 18);
                rgblight_setrgb_at(RGB_GREEN, 19);
                rgblight_setrgb_at(RGB_GREEN, 20);
            }
            break;
        case _NAVI:
            active_layer = _NAVI;
            if (!isLeftHand) {
                rgblight_setrgb_at(RGB_CYAN, 6);
                rgblight_setrgb_at(RGB_PINK, 8);
                rgblight_setrgb_at(RGB_PINK, 9);
                rgblight_setrgb_at(RGB_ORANGE, 10);
                rgblight_setrgb_at(RGB_PURPLE, 11);
                rgblight_setrgb_at(RGB_PURPLE, 16);
                rgblight_setrgb_at(RGB_PURPLE, 17);
                rgblight_setrgb_at(RGB_ORANGE, 18);
                rgblight_setrgb_at(RGB_PURPLE, 19);
            } else {
                rgblight_setrgb_at(RGB_ORANGE, 12);
                rgblight_setrgb_at(RGB_ORANGE, 15);
                rgblight_setrgb_at(RGB_ORANGE, 20);
                rgblight_setrgb_at(RGB_ORANGE, 21);
            }
            break;
        case _ADJUST:
            active_layer = _ADJUST;
            if (!isLeftHand) {
                rgblight_setrgb_at(RGB_CYAN, 14);
            } else {
                rgblight_setrgb_at(RGB_ORANGE, 16);
                rgblight_setrgb_at(RGB_ORANGE, 17);
                rgblight_setrgb_at(RGB_ORANGE, 18);
                rgblight_setrgb_at(RGB_ORANGE, 19);
                rgblight_setrgb_at(RGB_ORANGE, 22);
                rgblight_setrgb_at(RGB_ORANGE, 23);
                rgblight_setrgb_at(RGB_RED, 24);
                rgblight_setrgb_at(RGB_GOLD, 25);
            }
            break;
        default:
            active_layer = _QWERTY;
            rgblight_enable();
            if (isLeftHand) {
                for (int i = 0; i < 6; i++) {
                    rgblight_setrgb_at(0, 0, 0, i);
                }
            } else {
                for (int j = 0; j < 6; j++) {
                    rgblight_setrgb_at(0, 0, 0, j);
                }
            }
            break;
    }
    return state;
}

int RGB_current_mode;

void persistent_default_layer_set(uint16_t default_layer) {
    eeconfig_update_default_layer(default_layer);
    default_layer_set(default_layer);
}

// ==== OLED ====

static uint32_t oled_timer = 0;

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

static void render_left(void) {
    if (active_layer == _NUMB) {
        oled_write_P(PSTR("SYMB\n"), false);
        oled_write_P(PSTR("====\n"), false);
        oled_write_P(PSTR("!@#\n"), false);
        oled_write_P(PSTR("$%^\n"), false);
        oled_write_P(PSTR("&*|\\\n"), false);
    } else if (active_layer == _ADJUST) {
        oled_write_P(PSTR("ADJU\n"), false);
        oled_write_P(PSTR("====\n"), false);
        oled_write_P(PSTR("\n\n\n"), false);
    } else {
        oled_off();
    }
}

static void render_right(void) {
    if (active_layer == _NUMB) {
        oled_write_P(PSTR("NUMB\n"), false);
        oled_write_P(PSTR("====\n"), false);
        oled_write_P(PSTR("0123\n"), false);
        oled_write_P(PSTR(" 456\n"), false);
        oled_write_P(PSTR(" 789\n"), false);
    } else if (active_layer == _NAVI) {
        oled_write_P(PSTR("NAVI\n"), false);
        oled_write_P(PSTR("====\n"), false);
        oled_write_P(PSTR("Uh^e\n"), false);
        oled_write_P(PSTR("D<v> \n"), false);
    } else if (active_layer == _ADJUST) {
        oled_write_P(PSTR("ADJU\n"), false);
        oled_write_P(PSTR("====\n"), false);
        oled_write_P(PSTR("\n\n\n"), false);
    } else {
        oled_off();
    }
}

void oled_task_user(void) {
    if (timer_elapsed32(oled_timer) > 30000) {
        oled_off();
        return;
    }
    if (isLeftHand) {
        render_left();
    } else {
        render_right();
    }
}

// === user record ===

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        oled_timer = timer_read32();
        oled_on();
    }
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
