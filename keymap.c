#include QMK_KEYBOARD_H

// clang-format off


// ============================================================================
// Layers
// ============================================================================
//
// WIN  = Windows base layer
// MAC  = macOS base layer
// FUNC = Shared function / RGB layer
//
// Fn + Paste toggles WIN <-> MAC and stores the choice in EEPROM.
//
// RGB colour indicates the current OS:
//
//     Windows = Green
//     macOS   = Orange
//
// ============================================================================

enum layers {
    WIN,
    MAC,
    FUNC
};


// ============================================================================
// Custom keycodes
// ============================================================================

enum custom_keycodes {

    APP_SWITCH = SAFE_RANGE,

    TEAMS_MUTE,

    RGB_DEFAULT,

    OS_TOGGLE
};


// ============================================================================
// Tap Dance declarations
// ============================================================================

enum {

    TD_NEW_NEWTAB = 0,

    TD_SAVE_F12,

    TD_RELOAD_HARDRELOAD,

    TD_U_FN_ESC,

    TD_SELECTALL_HOME,

    TD_EXCEL_SELECT,

    TD_DEFCON,

    TD_NUKE,

    TD_FIND_REPLACE,

    TD_UNDO_REDO,

    TD_COPY_CUT,

    TD_PASTE_PASTESPECIAL,

    TD_PLUS_SNIP
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
// App Switch modifier state
// ============================================================================
//
// Windows = Alt+Tab
// macOS   = Command+Tab
//
// We remember which modifier was pressed so that the correct one is released
// when the physical key is released.
// ============================================================================

static uint16_t app_switch_modifier = KC_NO;


// ============================================================================
// OS mode helpers
// ============================================================================

// ----------------------------------------------------------------------------
// Returns true if the persistent default layer is MAC.
// ----------------------------------------------------------------------------

bool is_mac_mode(void) {

    return get_highest_layer(default_layer_state) == MAC;
}


// ----------------------------------------------------------------------------
// Return the primary shortcut modifier.
//
// Windows = Ctrl
// macOS   = Command
// ----------------------------------------------------------------------------

uint8_t primary_modifier(void) {

    if (is_mac_mode()) {

        return MOD_BIT(KC_LGUI);
    }

    return MOD_BIT(KC_LCTL);
}


// ----------------------------------------------------------------------------
// Send a basic key while holding the supplied modifier(s).
// ----------------------------------------------------------------------------

void tap_with_mods(
    uint8_t mods,
    uint16_t keycode
) {

    register_mods(mods);

    tap_code(keycode);

    unregister_mods(mods);
}


// ----------------------------------------------------------------------------
// Send Primary Modifier + key.
//
// Windows = Ctrl+key
// macOS   = Command+key
// ----------------------------------------------------------------------------

void tap_primary(uint16_t keycode) {

    tap_with_mods(
        primary_modifier(),
        keycode
    );
}


// ----------------------------------------------------------------------------
// Send Primary Modifier + Shift + key.
//
// Windows = Ctrl+Shift+key
// macOS   = Command+Shift+key
// ----------------------------------------------------------------------------

void tap_primary_shift(uint16_t keycode) {

    tap_with_mods(
        primary_modifier() | MOD_BIT(KC_LSFT),
        keycode
    );
}


// ============================================================================
// Preferred RGB setup
// ============================================================================
//
// RGB mode:
//
//     Solid Reactive Multiwide
//
// Colour indicates current operating system:
//
//     WIN = Green
//     MAC = Orange
//
// The no-EEPROM versions are intentional. The OS mode itself is persistent,
// but changing RGB colour does not repeatedly write RGB state to EEPROM.
// ============================================================================

void set_default_rgb(void) {

    // Ensure RGB Matrix is enabled.
    rgb_matrix_enable_noeeprom();


    // Reactive effect:
    // pressed key and surrounding keys illuminate temporarily.
    rgb_matrix_mode_noeeprom(
        RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
    );


    // ------------------------------------------------------------------------
    // OS-specific colour
    // ------------------------------------------------------------------------

    if (is_mac_mode()) {

        // macOS = Orange
        rgb_matrix_sethsv_noeeprom(
            HSV_ORANGE
        );

    } else {

        // Windows = Green
        rgb_matrix_sethsv_noeeprom(
            HSV_GREEN
        );
    }
}


// ============================================================================
// Startup
// ============================================================================
//
// Restore the preferred reactive lighting on startup.
//
// The stored default layer determines whether this starts Green or Orange.
// ============================================================================

void keyboard_post_init_user(void) {

    set_default_rgb();
}


// ============================================================================
// Disable Keychron Num Lock RGB indicator
// ============================================================================
//
// The stock Q0 firmware can illuminate one key permanently when Windows
// reports Num Lock enabled.
//
// Returning false prevents that additional indicator from being applied.
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
// TAP DANCE CALLBACKS
// ============================================================================


// ----------------------------------------------------------------------------
// Alpha
//
// WINDOWS
//
//     Single = Ctrl+N
//              New
//
//     Double = Ctrl+T
//              New Tab
//
// MAC
//
//     Single = Cmd+N
//              New
//
//     Double = Cmd+T
//              New Tab
// ----------------------------------------------------------------------------

void new_newtab_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    if (state->count == 1) {

        tap_primary(KC_N);

    } else if (state->count >= 2) {

        tap_primary(KC_T);
    }
}


// ----------------------------------------------------------------------------
// Beta
//
// WINDOWS / MAC
//
//     Single = Save
//
//              Windows = Ctrl+S
//              macOS   = Cmd+S
//
//     Double = F12
//              F12
//
// Deliberately sends two separate F12 keypresses.
// ----------------------------------------------------------------------------

void save_f12_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    if (state->count == 1) {

        tap_primary(KC_S);

    } else if (state->count >= 2) {

        tap_code(KC_F12);

        tap_code(KC_F12);
    }
}


// ----------------------------------------------------------------------------
// Gamma / Y
//
// WINDOWS
//
//     Single = Ctrl+R
//              Reload
//
//     Double = Ctrl+Shift+R
//              Hard Reload
//
// MAC
//
//     Single = Cmd+R
//              Reload
//
//     Double = Cmd+Shift+R
//              Hard Reload
// ----------------------------------------------------------------------------

void reload_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    if (state->count == 1) {

        tap_primary(KC_R);

    } else if (state->count >= 2) {

        tap_primary_shift(KC_R);
    }
}


// ----------------------------------------------------------------------------
// Select All
//
// WINDOWS
//
//     Single = Ctrl+A
//
// MAC
//
//     Single = Cmd+A
//
// BOTH
//
//     Double = Ctrl+Home
//
// In Excel Ctrl+Home moves to the start of the worksheet / A1.
// ----------------------------------------------------------------------------

void selectall_home_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    if (state->count == 1) {

        tap_primary(KC_A);

    } else if (state->count >= 2) {

        tap_with_mods(
            MOD_BIT(KC_LCTL),
            KC_HOME
        );
    }
}


// ----------------------------------------------------------------------------
// Physical Mute-labelled key
//
// The keycap says Mute, but actual Teams Mute is now on encoder press.
//
// WINDOWS
//
//     Single = Ctrl+Shift+Down
//     Double = Ctrl+Shift+Right
//
// MAC
//
//     Single = Cmd+Shift+Down
//     Double = Cmd+Shift+Right
//
// Intended primarily for Excel selection expansion.
// ----------------------------------------------------------------------------

void excel_select_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    if (state->count == 1) {

        tap_primary_shift(KC_DOWN);

    } else if (state->count >= 2) {

        tap_primary_shift(KC_RGHT);
    }
}


// ----------------------------------------------------------------------------
// DEF CON
//
// Single = Delete
//
// Double:
//
//     Windows = Win+D
//               Show Desktop
//
//     macOS   = F11
//               Show Desktop
// ----------------------------------------------------------------------------

void defcon_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    // ------------------------------------------------------------------------
    // Single tap = Delete
    // ------------------------------------------------------------------------

    if (state->count == 1) {

        tap_code(KC_DEL);

        return;
    }


    // ------------------------------------------------------------------------
    // Double tap = Show Desktop
    // ------------------------------------------------------------------------

    if (state->count >= 2) {

        if (is_mac_mode()) {

            // macOS Show Desktop
            tap_code(KC_F11);

        } else {

            // Windows Show Desktop
            tap_with_mods(
                MOD_BIT(KC_LGUI),
                KC_D
            );
        }
    }
}


// ----------------------------------------------------------------------------
// Nuke
//
// WINDOWS
//
//     Single = Ctrl+W
//              Close current tab/document
//
//     Double = Alt+F4
//              Close application/window
//
// MAC
//
//     Single = Cmd+W
//              Close current tab/window
//
//     Double = Cmd+Q
//              Quit application
// ----------------------------------------------------------------------------

void nuke_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    if (state->count == 1) {

        tap_primary(KC_W);

    } else if (state->count >= 2) {

        if (is_mac_mode()) {

            // macOS Quit
            tap_with_mods(
                MOD_BIT(KC_LGUI),
                KC_Q
            );

        } else {

            // Windows Close Application
            tap_with_mods(
                MOD_BIT(KC_LALT),
                KC_F4
            );
        }
    }
}


// ----------------------------------------------------------------------------
// Geek Hack
//
// WINDOWS
//
//     Single = Ctrl+F
//              Find
//
//     Double = Ctrl+H
//              Replace
//
// MAC
//
//     Single = Cmd+F
//              Find
//
//     Double = Cmd+Shift+H
//              Replace in Excel for Mac
// ----------------------------------------------------------------------------

void find_replace_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    if (state->count == 1) {

        tap_primary(KC_F);

        return;
    }


    if (state->count >= 2) {

        if (is_mac_mode()) {

            tap_with_mods(
                MOD_BIT(KC_LGUI) |
                MOD_BIT(KC_LSFT),
                KC_H
            );

        } else {

            tap_with_mods(
                MOD_BIT(KC_LCTL),
                KC_H
            );
        }
    }
}


// ----------------------------------------------------------------------------
// Undo / Redo
//
// WINDOWS
//
//     Single = Ctrl+Z
//     Double = Ctrl+Y
//
// MAC
//
//     Single = Cmd+Z
//     Double = Cmd+Shift+Z
// ----------------------------------------------------------------------------

void undo_redo_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    if (state->count == 1) {

        tap_primary(KC_Z);

        return;
    }


    if (state->count >= 2) {

        if (is_mac_mode()) {

            // macOS Redo
            tap_with_mods(
                MOD_BIT(KC_LGUI) |
                MOD_BIT(KC_LSFT),
                KC_Z
            );

        } else {

            // Windows Redo
            tap_with_mods(
                MOD_BIT(KC_LCTL),
                KC_Y
            );
        }
    }
}


// ----------------------------------------------------------------------------
// Copy / Cut
//
// WINDOWS
//
//     Single = Ctrl+C
//     Double = Ctrl+X
//
// MAC
//
//     Single = Cmd+C
//     Double = Cmd+X
// ----------------------------------------------------------------------------

void copy_cut_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    if (state->count == 1) {

        tap_primary(KC_C);

    } else if (state->count >= 2) {

        tap_primary(KC_X);
    }
}


// ----------------------------------------------------------------------------
// Paste / Paste Special
//
// WINDOWS
//
//     Single = Ctrl+V
//     Double = Ctrl+Alt+V
//
// MAC
//
//     Single = Cmd+V
//     Double = Cmd+Ctrl+V
// ----------------------------------------------------------------------------

void paste_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    if (state->count == 1) {

        tap_primary(KC_V);

        return;
    }


    if (state->count >= 2) {

        if (is_mac_mode()) {

            // macOS Paste Special
            tap_with_mods(
                MOD_BIT(KC_LGUI) |
                MOD_BIT(KC_LCTL),
                KC_V
            );

        } else {

            // Windows Paste Special
            tap_with_mods(
                MOD_BIT(KC_LCTL) |
                MOD_BIT(KC_LALT),
                KC_V
            );
        }
    }
}


// ----------------------------------------------------------------------------
// Plus
//
// Single = Numpad +
//
// Double:
//
//     Windows = Ctrl+F12
//               Custom snipping application
//
//     macOS   = Cmd+Shift+4
//               Native selectable-area screenshot
// ----------------------------------------------------------------------------

void plus_snip_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    if (state->count == 1) {

        tap_code(KC_PPLS);

        return;
    }


    if (state->count >= 2) {

        if (is_mac_mode()) {

            // macOS screenshot selection
            tap_with_mods(
                MOD_BIT(KC_LGUI) |
                MOD_BIT(KC_LSFT),
                KC_4
            );

        } else {

            // Windows custom snip application
            tap_with_mods(
                MOD_BIT(KC_LCTL),
                KC_F12
            );
        }
    }
}


// ----------------------------------------------------------------------------
// U
//
// Single = No action
//
// Hold = FUNC layer
//
// Double = Escape
// ----------------------------------------------------------------------------

void u_fn_esc_finished(
    tap_dance_state_t *state,
    void *user_data
) {

    (void)user_data;


    // ------------------------------------------------------------------------
    // Double tap = Escape
    // ------------------------------------------------------------------------

    if (state->count >= 2) {

        tap_code(KC_ESC);

        u_td_state = U_TD_NONE;

        return;
    }


    // ------------------------------------------------------------------------
    // Hold = Function layer
    // ------------------------------------------------------------------------

    if (
        state->count == 1 &&
        state->pressed
    ) {

        layer_on(FUNC);

        u_td_state = U_TD_FN_HOLD;

        return;
    }


    // ------------------------------------------------------------------------
    // Normal single tap = no action
    // ------------------------------------------------------------------------

    u_td_state = U_TD_NONE;
}


// ----------------------------------------------------------------------------
// U release
//
// Turn FUNC off again when U is released.
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


    [TD_NEW_NEWTAB] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            new_newtab_finished,
            NULL
        ),


    [TD_SAVE_F12] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            save_f12_finished,
            NULL
        ),


    [TD_RELOAD_HARDRELOAD] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            reload_finished,
            NULL
        ),


    [TD_U_FN_ESC] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            u_fn_esc_finished,
            u_fn_esc_reset
        ),


    [TD_SELECTALL_HOME] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            selectall_home_finished,
            NULL
        ),


    [TD_EXCEL_SELECT] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            excel_select_finished,
            NULL
        ),


    [TD_DEFCON] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            defcon_finished,
            NULL
        ),


    [TD_NUKE] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            nuke_finished,
            NULL
        ),


    [TD_FIND_REPLACE] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            find_replace_finished,
            NULL
        ),


    [TD_UNDO_REDO] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            undo_redo_finished,
            NULL
        ),


    [TD_COPY_CUT] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            copy_cut_finished,
            NULL
        ),


    [TD_PASTE_PASTESPECIAL] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            paste_finished,
            NULL
        ),


    [TD_PLUS_SNIP] =
        ACTION_TAP_DANCE_FN_ADVANCED(
            NULL,
            plus_snip_finished,
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
        // Application Switch
        //
        // Windows = Alt+Tab
        // macOS   = Cmd+Tab
        //
        // Modifier remains held while the physical key is held.
        // --------------------------------------------------------------------

        case APP_SWITCH:

            if (record->event.pressed) {

                if (is_mac_mode()) {

                    app_switch_modifier = KC_LGUI;

                } else {

                    app_switch_modifier = KC_LALT;
                }


                register_code(
                    app_switch_modifier
                );


                tap_code(
                    KC_TAB
                );

            } else {

                if (app_switch_modifier != KC_NO) {

                    unregister_code(
                        app_switch_modifier
                    );
                }


                app_switch_modifier = KC_NO;
            }


            return false;


        // --------------------------------------------------------------------
        // Teams Mute
        //
        // Windows = Ctrl+Shift+M
        // macOS   = Cmd+Shift+M
        // --------------------------------------------------------------------

        case TEAMS_MUTE:

            if (record->event.pressed) {

                tap_primary_shift(
                    KC_M
                );
            }

            return false;


        // --------------------------------------------------------------------
        // Restore preferred reactive RGB
        //
        // Windows = Green
        // macOS   = Orange
        // --------------------------------------------------------------------

        case RGB_DEFAULT:

            if (record->event.pressed) {

                set_default_rgb();
            }

            return false;


        // --------------------------------------------------------------------
        // Toggle Windows / macOS mode
        //
        // Fn + Paste
        //
        // Mode is persisted to EEPROM.
        //
        // RGB changes immediately:
        //
        //     Windows = Green
        //     macOS   = Orange
        // --------------------------------------------------------------------

        case OS_TOGGLE:

            if (record->event.pressed) {

                if (is_mac_mode()) {

                    // --------------------------------------------------------
                    // Switch from macOS to Windows
                    // --------------------------------------------------------

                    set_single_persistent_default_layer(
                        WIN
                    );

                } else {

                    // --------------------------------------------------------
                    // Switch from Windows to macOS
                    // --------------------------------------------------------

                    set_single_persistent_default_layer(
                        MAC
                    );
                }


                // ------------------------------------------------------------
                // Immediately indicate the newly-selected OS with RGB.
                //
                // Windows = Green
                // macOS   = Orange
                // ------------------------------------------------------------

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
    // WINDOWS BASE LAYER
    // =========================================================================
    //
    // RGB = GREEN
    //
    //
    // TOP ROW
    //
    // Encoder       Alpha         Beta          Gamma/Y       U
    //
    // Teams Mute    New           Save          Reload        Hold = Fn
    //               2x New Tab    2x F12,F12    2x Hard       2x Esc
    //                                           Reload
    //
    //
    // SECOND ROW
    //
    // App Switch    Select All    Excel Select   DEF CON      Nuke
    //
    // Alt+Tab       Ctrl+A        Ctrl+Shift+↓   Delete       Ctrl+W
    //               2x Ctrl+Home  2x Ctrl+Shft+→ 2x Win+D     2x Alt+F4
    //
    //
    // LEFT COLUMN
    //
    // Geek Hack = Find / Replace
    // Undo      = Undo / Redo
    // Copy      = Copy / Cut
    // Paste     = Paste / Paste Special
    //
    // =========================================================================

    [WIN] = LAYOUT_numpad_6x5(


        // ---------------------------------------------------------------------
        // Top row
        // ---------------------------------------------------------------------

        TEAMS_MUTE,                     // Encoder press
                                        // Ctrl+Shift+M

        TD(TD_NEW_NEWTAB),              // Alpha
                                        // Tap = Ctrl+N
                                        // 2x  = Ctrl+T

        TD(TD_SAVE_F12),                // Beta
                                        // Tap = Ctrl+S
                                        // 2x  = F12 twice

        TD(TD_RELOAD_HARDRELOAD),       // Gamma / Y
                                        // Tap = Ctrl+R
                                        // 2x  = Ctrl+Shift+R

        TD(TD_U_FN_ESC),                // U
                                        // Hold = FUNC
                                        // 2x   = Escape


        // ---------------------------------------------------------------------
        // Second row
        // ---------------------------------------------------------------------

        APP_SWITCH,                     // Alt+Tab

        TD(TD_SELECTALL_HOME),          // Tap = Ctrl+A
                                        // 2x  = Ctrl+Home

        TD(TD_EXCEL_SELECT),            // Tap = Ctrl+Shift+Down
                                        // 2x  = Ctrl+Shift+Right

        TD(TD_DEFCON),                  // Tap = Delete
                                        // 2x  = Win+D

        TD(TD_NUKE),                    // Tap = Ctrl+W
                                        // 2x  = Alt+F4


        // ---------------------------------------------------------------------
        // Geek Hack / 7 / 8 / 9 / +
        // ---------------------------------------------------------------------

        TD(TD_FIND_REPLACE),            // Ctrl+F / Ctrl+H

        KC_P7,

        KC_P8,

        KC_P9,

        TD(TD_PLUS_SNIP),               // Tap = +
                                        // 2x  = Ctrl+F12


        // ---------------------------------------------------------------------
        // Undo / 4 / 5 / 6
        // ---------------------------------------------------------------------

        TD(TD_UNDO_REDO),               // Ctrl+Z / Ctrl+Y

        KC_P4,

        KC_P5,

        KC_P6,


        // ---------------------------------------------------------------------
        // Copy / 1 / 2 / 3 / Enter
        // ---------------------------------------------------------------------

        TD(TD_COPY_CUT),                // Ctrl+C / Ctrl+X

        KC_P1,

        KC_P2,

        KC_P3,

        KC_PENT,


        // ---------------------------------------------------------------------
        // Paste / 0 / Decimal
        // ---------------------------------------------------------------------

        TD(TD_PASTE_PASTESPECIAL),      // Ctrl+V / Ctrl+Alt+V

        KC_P0,

        KC_PDOT
    ),



    // =========================================================================
    // MAC BASE LAYER
    // =========================================================================
    //
    // RGB = ORANGE
    //
    // Physical layout is identical to Windows.
    //
    // Shortcuts are translated to their macOS equivalents.
    // =========================================================================

    [MAC] = LAYOUT_numpad_6x5(


        // ---------------------------------------------------------------------
        // Top row
        // ---------------------------------------------------------------------

        TEAMS_MUTE,                     // Cmd+Shift+M

        TD(TD_NEW_NEWTAB),              // Cmd+N / Cmd+T

        TD(TD_SAVE_F12),                // Cmd+S / F12 twice

        TD(TD_RELOAD_HARDRELOAD),       // Cmd+R / Cmd+Shift+R

        TD(TD_U_FN_ESC),                // Hold = Fn
                                        // 2x   = Esc


        // ---------------------------------------------------------------------
        // Second row
        // ---------------------------------------------------------------------

        APP_SWITCH,                     // Cmd+Tab

        TD(TD_SELECTALL_HOME),          // Cmd+A / Ctrl+Home

        TD(TD_EXCEL_SELECT),            // Cmd+Shift+Down
                                        // Cmd+Shift+Right

        TD(TD_DEFCON),                  // Delete / F11

        TD(TD_NUKE),                    // Cmd+W / Cmd+Q


        // ---------------------------------------------------------------------
        // Geek Hack / 7 / 8 / 9 / +
        // ---------------------------------------------------------------------

        TD(TD_FIND_REPLACE),            // Cmd+F / Cmd+Shift+H

        KC_P7,

        KC_P8,

        KC_P9,

        TD(TD_PLUS_SNIP),               // + / Cmd+Shift+4


        // ---------------------------------------------------------------------
        // Undo / 4 / 5 / 6
        // ---------------------------------------------------------------------

        TD(TD_UNDO_REDO),               // Cmd+Z / Cmd+Shift+Z

        KC_P4,

        KC_P5,

        KC_P6,


        // ---------------------------------------------------------------------
        // Copy / 1 / 2 / 3 / Enter
        // ---------------------------------------------------------------------

        TD(TD_COPY_CUT),                // Cmd+C / Cmd+X

        KC_P1,

        KC_P2,

        KC_P3,

        KC_PENT,


        // ---------------------------------------------------------------------
        // Paste / 0 / Decimal
        // ---------------------------------------------------------------------

        TD(TD_PASTE_PASTESPECIAL),      // Cmd+V / Cmd+Ctrl+V

        KC_P0,

        KC_PDOT
    ),



    // =========================================================================
    // SHARED FUNCTION / RGB LAYER
    // =========================================================================
    //
    // Hold U to access.
    //
    //
    // SPECIAL:
    //
    //     Fn + Geek Hack = Right Arrow
    //
    //         Collapse / clear a selected text range to its right-hand end.
    //
    //
    //     Fn + Paste = Windows / macOS toggle
    //
    //         Windows -> Green
    //         macOS   -> Orange
    //
    //
    //     Fn + Nuke = QK_BOOT
    //
    //         Enter bootloader for firmware flashing.
    //
    //
    //     Fn + 5 = Restore default RGB
    //
    //         Windows = Green Reactive
    //         macOS   = Orange Reactive
    //
    // =========================================================================

    [FUNC] = LAYOUT_numpad_6x5(


        // ---------------------------------------------------------------------
        // Top row
        // ---------------------------------------------------------------------

        TEAMS_MUTE,

        KC_MPRV,                        // Alpha = Previous Track

        KC_MPLY,                        // Beta = Play / Pause

        KC_MNXT,                        // Gamma = Next Track

        _______,                        // U


        // ---------------------------------------------------------------------
        // Second row
        // ---------------------------------------------------------------------

        RM_TOGG,                        // RGB On / Off

        RM_NEXT,                        // Next RGB mode

        KC_DEL,                         // Delete

        RM_PREV,                        // Previous RGB mode

        QK_BOOT,                        // Bootloader


        // ---------------------------------------------------------------------
        // Geek Hack / 7 / 8 / 9 / +
        // ---------------------------------------------------------------------

        KC_RGHT,                        // Collapse selection to end

        RM_HUED,                        // 7 = Hue down

        RM_VALU,                        // 8 = Brightness up

        RM_HUEU,                        // 9 = Hue up

        RM_NEXT,                        // + = Next RGB mode


        // ---------------------------------------------------------------------
        // Undo / 4 / 5 / 6
        // ---------------------------------------------------------------------

        KC_NO,

        RM_SATD,                        // 4 = Saturation down

        RGB_DEFAULT,                    // 5 = Restore OS colour
                                        // WIN = Green
                                        // MAC = Orange

        RM_SATU,                        // 6 = Saturation up


        // ---------------------------------------------------------------------
        // Copy / 1 / 2 / 3 / Enter
        // ---------------------------------------------------------------------

        KC_NO,

        RM_SPDD,                        // 1 = Speed down

        RM_VALD,                        // 2 = Brightness down

        RM_SPDU,                        // 3 = Speed up

        RM_PREV,                        // Enter = Previous RGB mode


        // ---------------------------------------------------------------------
        // Paste / 0 / Decimal
        // ---------------------------------------------------------------------

        OS_TOGGLE,                      // Fn + Paste
                                        // Toggle WIN <-> MAC

        RM_TOGG,                        // 0 = RGB On / Off

        KC_DEL                          // Decimal = Delete
    )
};


// ============================================================================
// Rotary encoder
// ============================================================================
//
// BASE:
//
//     Turn left  = Volume Down
//     Turn right = Volume Up
//
//     Press:
//
//         Windows = Ctrl+Shift+M
//         macOS   = Cmd+Shift+M
//
// FUNC:
//
//     Turn left  = RGB Brightness Down
//     Turn right = RGB Brightness Up
//
// ============================================================================

#if defined(ENCODER_MAP_ENABLE)

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {


    [WIN] = {

        ENCODER_CCW_CW(
            KC_VOLD,
            KC_VOLU
        )
    },


    [MAC] = {

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
