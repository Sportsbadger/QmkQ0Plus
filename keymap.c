/* Copyright 2023 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "quantum.h"

// clang-format off

enum layers {
    BASE,
    FUNC
};

enum custom_keycodes {
    M_ALT_TAB = SAFE_RANGE
};

//Tap Dance Declarations

enum {
  TD_ESC_CAPS = 0
};

//Tap Dance Definitions
tap_dance_action_t tap_dance_actions[] = {
  //Tap once for Esc, twice for Caps Lock
  [TD_ESC_CAPS]  = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_CAPS)
// Other declarations would go here, separated by commas, if you have them
};


static bool is_alt_tab_active = false;
static uint16_t alt_tab_timer;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case M_ALT_TAB:
            if (record->event.pressed) {
                // Key pressed
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                tap_code(KC_TAB);
            } else {
                // Key released
                if (timer_elapsed(alt_tab_timer) > 500) { // Adjust the timing to your liking
                    unregister_code(KC_LALT);
                    is_alt_tab_active = false;
                }
            }
            break; // This break corresponds to the switch statement
    } // This closing bracket ends the switch statement block
    return true;
}




const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE] = LAYOUT_numpad_6x5(
        KC_MUTE,    LCTL(KC_N), TD(TD_ESC_CAPS),LCTL(KC_A),    MO(FUNC),
        M_ALT_TAB,  LWIN(KC_D), LALT(KC_F4),    LCTL(KC_F),    LCTL(KC_M),
        LCTL(KC_Z), KC_P7,      KC_P8,          KC_P9,         KC_PPLS,
        LCTL(KC_X), KC_P4,      KC_P5,          KC_P6,
        LCTL(KC_C), KC_P1,      KC_P2,          KC_P3,         KC_PENT,
        LCTL(KC_V), KC_P0,                      KC_PDOT),

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
