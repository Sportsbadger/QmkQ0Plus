#include QMK_KEYBOARD_H

// clang-format off


// ============================================================================
// Layers
// ============================================================================

enum layers {
    BASE,
    FUNC
};


// ============================================================================
// Custom keycodes
// ============================================================================

enum custom_keycodes {
    ALT_TAB = SAFE_RANGE,
    RGB_DEFAULT
};


// ============================================================================
// Tap Dance declarations
// ============================================================================

enum {
    TD_PASTE_PASTESPECIAL = 0,
    TD_FIND_REPLACE,
    TD_SAVE_SAVEAS,
    TD_COPY_CUT,
    TD_PLUS_SNIP,
    TD_NUKE,
    TD_U_FN_ESC,
    TD_RELOAD_HARDRELOAD
};


// ============================================================================
// U / Fn Tap Dance state
// ============================================================================

typedef enum {
    U_TD_NONE,
    U_TD_FN_HOLD
} u_td_state_t;

static u_td_state_t u_td_state = U_TD_NONE;


// ============================================================================
// Preferred RGB setup
// ============================================================================
//
// Green reactive lighting.
// ============================================================================

void set_default_rgb(void) {

    rgb_matrix_enable_noeeprom();

    rgb_matrix_mode_noeeprom(
        RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    );

    rgb_matrix_sethsv_noeeprom(HSV_GREEN);
}


// ============================================================================
// Startup
// ============================================================================

void keyboard_post_init_user(void) {

    set_default_rgb();
}


// ============================================================================
// Disable Keychron Num Lock RGB indicator
// ============================================================================
//
// Prevents the Windows Num Lock state from leaving the DEF CON LED
// permanently illuminated.
// ============================================================================

bool rgb_matrix_indicators_advanced_user(
    uint8_t led_min,
    uint8_t led_max
) {

    (void)led_min;
    (void)led_max;

    return false;
}


// ============================================================================
// Tap Dance callbacks
// ============================================================================


// ----------------------------------------------------------------------------
// Paste
//
// Single = Ctrl+V
// Double = Ctrl+Alt+V
// ----------------------------------------------------------------------------

void paste_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;

    if (state->count == 1) {

        tap_code16(LCTL(KC_V));

    } else if (state->count >= 2) {

        tap_code16(LALT(LCTL(KC_V)));
    }
}


// ----------------------------------------------------------------------------
// Geek Hack
//
// Single = Ctrl+F
// Double = Ctrl+H
// ----------------------------------------------------------------------------

void find_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;

    if (state->count == 1) {

        tap_code16(LCTL(KC_F));

    } else if (state->count >= 2) {

        tap_code16(LCTL(KC_H));
    }
}


// ----------------------------------------------------------------------------
// Beta
//
// Single = Ctrl+S
// Double = Ctrl+Shift+S
// ----------------------------------------------------------------------------

void save_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;

    if (state->count == 1) {

        tap_code16(LCTL(KC_S));

    } else if (state->count >= 2) {

        tap_code16(LCTL(LSFT(KC_S)));
    }
}


// ----------------------------------------------------------------------------
// Copy
//
// Single = Ctrl+C
// Double = Ctrl+X
// ----------------------------------------------------------------------------

void copy_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;

    if (state->count == 1) {

        tap_code16(LCTL(KC_C));

    } else if (state->count >= 2) {

        tap_code16(LCTL(KC_X));
    }
}


// ----------------------------------------------------------------------------
// Plus
//
// Single = Numpad +
// Double = Ctrl+F12
// ----------------------------------------------------------------------------

void plus_snip_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;

    if (state->count == 1) {

        tap_code(KC_PPLS);

    } else if (state->count >= 2) {

        tap_code16(LCTL(KC_F12));
    }
}


// ----------------------------------------------------------------------------
// Nuke
//
// Single = Ctrl+W
//          Close current tab/document
//
// Double = Alt+F4
//          Close application/window
// ----------------------------------------------------------------------------

void nuke_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;

    if (state->count == 1) {

        tap_code16(LCTL(KC_W));

    } else if (state->count >= 2) {

        tap_code16(LALT(KC_F4));
    }
}


// ----------------------------------------------------------------------------
// Gamma / Y
//
// Single = Ctrl+R
//          Reload page
//
// Double = Ctrl+Shift+R
//          Hard reload / bypass cache
// ----------------------------------------------------------------------------

void reload_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;

    if (state->count == 1) {

        tap_code16(LCTL(KC_R));

    } else if (state->count >= 2) {

        tap_code16(LCTL(LSFT(KC_R)));
    }
}


// ----------------------------------------------------------------------------
// U
//
// Single = No action
// Hold   = FUNC layer
// Double = Escape
// ----------------------------------------------------------------------------

void u_fn_esc_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    // Double tap = Escape
    if (state->count >= 2) {

        tap_code(KC_ESC);

        u_td_state = U_TD_NONE;

        return;
    }


    // Hold = Fn layer
    if (
        state->count == 1 &&
        state->pressed
    ) {

        layer_on(FUNC);

        u_td_state = U_TD_FN_HOLD;

        return;
    }


    // Normal single tap = nothing
    u_td_state = U_TD_NONE;
}


// ----------------------------------------------------------------------------
// Release U / Fn
// ----------------------------------------------------------------------------

void u_fn_esc_reset(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)state;
    (void)user_data;

    if (u_td_state == U_TD_FN_HOLD) {

        layer_off(FUNC);
    }

    u_td_state = U_TD_NONE;
}


// ============================================================================
// Tap Dance definitions
// ============================================================================

tap_dance_action_t tap_dance_actions[] = {

    [TD_PASTE_PASTESPECIAL] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            paste_finished,
            NULL
        ),

    [TD_FIND_REPLACE] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            find_finished,
            NULL
        ),

    [TD_SAVE_SAVEAS] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            save_finished,
            NULL
        ),

    [TD_COPY_CUT] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            copy_finished,
            NULL
        ),

    [TD_PLUS_SNIP] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            plus_snip_finished,
            NULL
        ),

    [TD_NUKE] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            nuke_finished,
            NULL
        ),

    [TD_U_FN_ESC] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            u_fn_esc_finished,
            u_fn_esc_reset
        ),

    [TD_RELOAD_HARDRELOAD] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            reload_finished,
            NULL
        )
};


// ============================================================================
// Custom key handling
// ============================================================================

bool process_record_user(
    uint16_t keycode,
    keyrecord_t *record
) {

    switch (keycode) {


        // --------------------------------------------------------------------
        // Alt-Tab
        // --------------------------------------------------------------------

        case ALT_TAB:

            if (record->event.pressed) {

                register_code(KC_LALT);
                tap_code(KC_TAB);

            } else {

                unregister_code(KC_LALT);
            }

            return false;


        // --------------------------------------------------------------------
        // Restore preferred RGB
        // --------------------------------------------------------------------

        case RGB_DEFAULT:

            if (record->event.pressed) {

                set_default_rgb();
            }

            return false;
    }


    return true;
}


// ============================================================================
// Keymaps
// ============================================================================

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {


    // =========================================================================
    // BASE
    // =========================================================================
    //
    // TOP ROW:
    //
    // Encoder     Alpha        Beta          Gamma/Y       U
    //
    // Mute        Ctrl+N       Save          Reload        Hold = Fn
    //                           2x Save As     2x Hard       2x Escape
    //                                         Reload
    //
    //
    // SECOND ROW:
    //
    // Alt-Tab     Select All   Mute          DEF CON       Nuke
    //
    // Alt-Tab     Ctrl+A       Ctrl+Shift+M  Win+D         Ctrl+W
    //                                                       2x Alt+F4
    //
    // =========================================================================

    [BASE] = LAYOUT_numpad_6x5(


        // ---------------------------------------------------------------------
        // Top row
        // ---------------------------------------------------------------------

        KC_MUTE,                         // Encoder press = system Mute

        LCTL(KC_N),                      // Alpha = New

        TD(TD_SAVE_SAVEAS),              // Beta
                                         // Single = Save
                                         // Double = Save As

        TD(TD_RELOAD_HARDRELOAD),        // Gamma / Y
                                         // Single = Reload
                                         // Double = Hard Reload

        TD(TD_U_FN_ESC),                 // U
                                         // Hold   = Fn
                                         // Double = Escape


        // ---------------------------------------------------------------------
        // Second row
        //
        // Alt-Tab | Select All | Mute | DEF CON | Nuke
        // ---------------------------------------------------------------------

        ALT_TAB,

        LCTL(KC_A),                      // Select All

        LCTL(LSFT(KC_M)),                // Ctrl+Shift+M

        LGUI(KC_D),                      // DEF CON = Show Desktop

        TD(TD_NUKE),                     // Nuke
                                         // Single = Ctrl+W
                                         // Double = Alt+F4


        // ---------------------------------------------------------------------
        // Geek Hack / 7 / 8 / 9 / +
        // ---------------------------------------------------------------------

        TD(TD_FIND_REPLACE),             // Find / Replace

        KC_P7,

        KC_P8,

        KC_P9,

        TD(TD_PLUS_SNIP),                // +
                                         // Single = +
                                         // Double = Ctrl+F12


        // ---------------------------------------------------------------------
        // Undo / 4 / 5 / 6
        // ---------------------------------------------------------------------

        LCTL(KC_Z),

        KC_P4,

        KC_P5,

        KC_P6,


        // ---------------------------------------------------------------------
        // Copy / 1 / 2 / 3 / Enter
        // ---------------------------------------------------------------------

        TD(TD_COPY_CUT),                 // Copy / Cut

        KC_P1,

        KC_P2,

        KC_P3,

        KC_PENT,


        // ---------------------------------------------------------------------
        // Paste / 0 / Decimal
        // ---------------------------------------------------------------------

        TD(TD_PASTE_PASTESPECIAL),       // Paste / Paste Special

        KC_P0,

        KC_PDOT
    ),



    // =========================================================================
    // FUNCTION / RGB
    // =========================================================================
    //
    // Hold U.
    //
    // Fn + Nuke = Bootloader
    //
    // Delete is currently available from:
    //
    //     Fn + Mute-position
    //     Fn + Decimal
    //
    // =========================================================================

    [FUNC] = LAYOUT_numpad_6x5(


        // ---------------------------------------------------------------------
        // Top row
        // ---------------------------------------------------------------------

        KC_MUTE,

        KC_MPRV,

        KC_MPLY,

        KC_MNXT,

        _______,


        // ---------------------------------------------------------------------
        // Second row
        // ---------------------------------------------------------------------

        RM_TOGG,                         // RGB On / Off

        RM_NEXT,                         // Next RGB mode

        KC_DEL,                          // Delete

        RM_PREV,                         // Previous RGB mode

        QK_BOOT,                         // Bootloader


        // ---------------------------------------------------------------------
        // 7 / 8 / 9 / +
        // ---------------------------------------------------------------------

        KC_NO,

        RM_HUED,

        RM_VALU,

        RM_HUEU,

        RM_NEXT,


        // ---------------------------------------------------------------------
        // 4 / 5 / 6
        // ---------------------------------------------------------------------

        KC_NO,

        RM_SATD,

        RGB_DEFAULT,

        RM_SATU,


        // ---------------------------------------------------------------------
        // 1 / 2 / 3 / Enter
        // ---------------------------------------------------------------------

        KC_NO,

        RM_SPDD,

        RM_VALD,

        RM_SPDU,

        RM_PREV,


        // ---------------------------------------------------------------------
        // Bottom
        // ---------------------------------------------------------------------

        KC_NO,

        RM_TOGG,

        KC_DEL
    )
};


// ============================================================================
// Rotary encoder
// ============================================================================

#if defined(ENCODER_MAP_ENABLE)

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {

    [BASE] = {
        ENCODER_CCW_CW(
            KC_VOLD,
            KC_VOLU
        )
    },

    [FUNC] = {
        ENCODER_CCW_CW(
            RM_VALD,
            RM_VALU
        )
    }
};

#endif
