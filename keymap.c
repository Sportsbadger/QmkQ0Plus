#include QMK_KEYBOARD_H
#include "quantum.h"

// clang-format off

enum layers {
    BASE,
    FUNC
};

enum custom_keycodes {
    ALT_TAB = SAFE_RANGE
};
3........
//Tap Dance Declarations

enum {
  TD_PASTE_PASTESPECIAL = 0,
  TD_FIND_REPLACE
};

//Tap Dance Definitions
tap_dance_action_t tap_dance_actions[] = {
  //Tap once for Esc, twice for Caps Lock
  [TD_PASTE_PASTESPECIAL]  = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_V), LALT(LCTL(KC_V))),
  [TD_FIND_REPLACE]  = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_F), LCTL(KC_H))
// Other declarations would go here, separated by commas, if you have them
};




bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ALT_TAB:
            if (record->event.pressed) {
                // When keycode ALT_TAB is pressed
                register_code(KC_LALT);
                tap_code(KC_TAB);
                // Keeping ALT pressed until the key is released
            } else {
                // When keycode is released, ALT is released
                unregister_code(KC_LALT);
            }
            break;
    }
    return true;
}





const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_numpad_6x5(
        KC_MUTE,    LCTL(KC_N), KC_NO,LCTL(KC_A),    MO(FUNC),
        ALT_TAB,  LWIN(KC_D), LALT(KC_F4),    TD(TD_FIND_REPLACE),    LCTL(KC_M),
        LCTL(KC_Z), KC_P7,      KC_P8,          KC_P9,         KC_PPLS,
        LCTL(KC_X), KC_P4,      KC_P5,          KC_P6,
        LCTL(KC_C), KC_P1,      KC_P2,          KC_P3,         KC_PENT,
        TD(TD_PASTE_PASTESPECIAL), KC_P0,                      KC_PDOT),

    [FUNC] = LAYOUT_numpad_6x5(
        RGB_TOG, _______,  KC_MUTE, KC_VOLD,  KC_VOLU,
        _______, RGB_MOD,  RGB_VAI, RGB_HUI,  KC_DEL,
        _______, RGB_RMOD, RGB_VAD, RGB_HUD,  _______,
        _______, RGB_SAI,  RGB_SPI, KC_MPRV,
        _______, RGB_SAD,  RGB_SPD, KC_MPLY,  _______,
        _______, RGB_TOG,           KC_MNXT),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [FUNC] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
};
#endif // ENCODER_MAP_ENABLE
