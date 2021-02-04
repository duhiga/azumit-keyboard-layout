/*
 * Azumit key layout. Dvorak. Selectable qwerty layer. Thumb shift. Symbols and numbers are under row 1 and 2 respectively, on the sym layer.
 */

#include QMK_KEYBOARD_H
#include "muse.h"

// define macro nicknames
#define AUTO_FORMAT S(A(KC_F))   // vscode auto format
#define COMMENT C(KC_SLSH)       // vscode comment line
#define LOCK LGUI(KC_L)          // windows lock desktop
#define DTOP_L LGUI(C(KC_LEFT))  // windows move active desktop to the left
#define DTOP_R LGUI(C(KC_RIGHT)) // windows move active desktop to the right
#define CTR_ALT_DEL C(A(KC_DEL)) // ctrl + alt + del

enum planck_layers
{
  _DVORAK,
  _QWERTY,
  _SYM,
  _NAV,
  _FUNC
};

enum planck_keycodes
{
  DVORAK = SAFE_RANGE,
  NAV,
  FUNC,
  SYM,
  QWERTY,
  ARROW,
  SMILEY,
};

// tapdance keycodes
enum td_keycodes
{
  ALT_LCBR, // 'LALT' when held, '{' when tapped.
  ALT_RCBR, // 'RALT' when held, '}' when tapped.
  ENT_SYM_FUNC // 
};

// define a type containing as many tapdance states as you need
typedef enum
{
  SINGLE_TAP,
  SINGLE_HOLD,
  DOUBLE_SINGLE_TAP
} td_state_t;

// create a global instance of the tapdance state type
static td_state_t td_state;

// declare your tapdance functions:

// function to determine the current tapdance state
int cur_dance(qk_tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void altlbcr_finished(qk_tap_dance_state_t *state, void *user_data);
void altlbcr_reset(qk_tap_dance_state_t *state, void *user_data);
void altrbcr_finished(qk_tap_dance_state_t *state, void *user_data);
void altrbcr_reset(qk_tap_dance_state_t *state, void *user_data);
void entsymfunc_finished(qk_tap_dance_state_t *state, void *user_data);
void entsymfunc_reset(qk_tap_dance_state_t *state, void *user_data);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    /* Dvorak
    * ,-----------------------------------------------------------------------------------.
    * | Esc  |   '  |   ,  |   .  |   P  |   Y  |   F  |   G  |   C  |   R  |   L  |   /  |
    * |------+------+------+------+------+-------------+------+------+------+------+------|
    * | Tab  |   A  |   O  |   E  |   U  |   I  |   D  |   H  |   T  |   N  |   S  |   -  |
    * |------+------+------+------+------+------|------+------+------+------+------+------|
    * |   (  |   ;  |   Q  |   J  |   K  |   X  |   B  |   M  |   W  |   V  |   Z  |   )  |
    * |------+------+------+------+------+------+------+------+------+------+------+------|
    * |[ Ctrl| { Alt|  Gui |  Del |BspNav| Shift| Space|EntSym| / Nav| = Fnc| } Alt|] Ctrl|
    * `-----------------------------------------------------------------------------------'
    */
    [_DVORAK] = LAYOUT_planck_grid(
        KC_ESC, KC_QUOT, KC_COMM, KC_DOT, KC_P, KC_Y, KC_F, KC_G, KC_C, KC_R, KC_L, KC_SLSH,
        KC_TAB, KC_A, KC_O, KC_E, KC_U, KC_I, KC_D, KC_H, KC_T, KC_N, KC_S, KC_MINS,
        KC_LPRN, KC_SCLN, KC_Q, KC_J, KC_K, KC_X, KC_B, KC_M, KC_W, KC_V, KC_Z, KC_RPRN,
        LCTL_T(KC_LBRC), TD(ALT_LCBR), KC_LGUI, LT(_SYM, KC_DEL), LT(_NAV, KC_BSPC), KC_LSFT, KC_SPC, LT(_SYM, KC_ENT), LT(_NAV, KC_BSLS), LT(_FUNC, KC_EQL), TD(ALT_RCBR), RCTL_T(KC_RBRC)),
    /* Qwerty
      ,-----------------------------------------------------------------------------------.
    * | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  |Enter |
    * |------+------+------+------+------+-------------+------+------+------+------+------|
    * | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  '   |
    * |------+------+------+------+------+------|------+------+------+------+------+------|
    * | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  | Del  |
    * |------+------+------+------+------+------+------+------+------+------+------+------|
    * | Ctrl | Alt  | Gui  |   -  | Nav  |Space |Space | Sym  | Left | Down |  Up  |Right |
    * `-----------------------------------------------------------------------------------'
    */
    [_QWERTY] = LAYOUT_planck_grid(
        KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_DEL,
        KC_ESC, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
        KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_ENT,
        KC_LCTL, KC_LALT, KC_LGUI, KC_LGUI, NAV, KC_SPC, KC_SPC, SYM, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT),
    /* Symbol
    * |------+------+------+------+------+------+------+------+------+------+------+------|
    * |   `  |   1  |   2  |   3  |   4  |   5  |   6  |   7  |   8  |   9  |   0  |   =  |
    * |------+------+------+------+------+-------------+------+------+------+------+------|
    * |   ~  |   !  |   @  |   #  |   $  |   %  |   ^  |   &  |   *  |   (  |   )  |   +  |
    * |------+------+------+------+------+------|------+------+------+------+------+------|
    * |Shift |   :  |  =>  |   J  |   .  |   X  |  :)  |   .  |   [  |   ]  |   :  |   )  |
    * |------+------+------+------+------+------+------+------+------+------+------+------|
    * |[ Ctrl| { Alt|  Gui |  Del |BspNav| Shift| Space|EntSym| / Nav| = Fnc| } Alt|] Ctrl|
    * `-----------------------------------------------------------------------------------'
    */
    [_SYM] = LAYOUT_planck_grid(
        KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, _______,
        KC_TILD, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PPLS,
        KC_LSFT, KC_COLN, ARROW, _______, KC_DOT, _______, SMILEY, KC_DOT, KC_LBRC, KC_RBRC, KC_COLN, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),
    /* Navigation
    * ,-----------------------------------------------------------------------------------.
    * |CtShf1| Home | CLeft|CRight|  End |   Y  |   F  |   G  |   7  |   8  |   9  |  -   |
    * |------+------+------+------+------+-------------+------+------+------+------+------|
    * |PrtScr| Left | Down |  Up  | Right|   G  |   D  |   H  |   4  |   5  |   6  |  +   |
    * |------+------+------+------+------+------|------+------+------+------+------+------|
    * |Shift |Ctrl1 |Ctrl2 |Ctrl3 |Ctrl4 |Macro1|Macro2|   M  |   1  |   2  |   3  |  *   |
    * |------+------+------+------+------+------+------+------+------+------+------+------|
    * |[ Ctrl| { Alt|  Gui |DelNav|SymBsp| Shift| Space|Fn Ent| / Nav|   0  |   .  |  Ent |
    * `-----------------------------------------------------------------------------------'
    */
    [_NAV] = LAYOUT_planck_grid(
        C(S(KC_1)), KC_HOME, C(KC_LEFT), C(KC_RIGHT), KC_END, _______, _______, _______, KC_7, KC_8, KC_9, KC_PMNS,
        KC_PSCR, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______, _______, _______, KC_4, KC_5, KC_6, KC_PPLS,
        KC_LSFT, C(KC_1), C(KC_2), C(KC_3), C(KC_4), DM_PLY1, DM_PLY2, _______, KC_1, KC_2, KC_3, KC_PAST,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_0, KC_DOT, KC_ENT),
    /* Function 
    * ,-----------------------------------------------------------------------------------.
    * | Esc  |<-DTOP|DTOP->|  F4  |  F5  |Format|Commnt|  F12 |  CAD | Vol- | Vol+ | Lock |
    * |------+------+------+------+------+-------------+------+------+------+------+------|
    * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |
    * |------+------+------+------+------+------|------+------+------+------+------+------|
    * |   (  |Ctrl1 |Ctrl2 |Ctrl3 |Ctrl4 |   X  |   B  |Reset |Music |MusMod|Dvorak|Qwerty|
    * |------+------+------+------+------+------+------+------+------+------+------+------|
    * |[ Ctrl| { Alt|  Gui |DelNav|SymBsp| Shift| Space|Fn Ent|Rec St| Func |Rec M1|Rec M2|
    * `-----------------------------------------------------------------------------------'
    */
    [_FUNC] = LAYOUT_planck_grid(
        KC_ESC, DTOP_L, DTOP_R, KC_F4, KC_F5, AUTO_FORMAT, COMMENT, KC_F12, CTR_ALT_DEL, KC_VOLD, KC_VOLU, LOCK,
        KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
        _______, C(KC_1), C(KC_2), C(KC_3), C(KC_4), _______, _______, RESET, MU_TOG, MU_MOD, DVORAK, QWERTY,
        _______, _______, _______, _______, _______, _______, _______, _______, DM_RSTP, _______, DM_REC1, DM_REC2)
};
// determine the tapdance state to return
int cur_dance(qk_tap_dance_state_t *state)
{
  if (state->count == 1)
  {
    if (state->interrupted || !state->pressed)
    {
      return SINGLE_TAP;
    }
    else
    {
      return SINGLE_HOLD;
    }
  }
  if (state->count == 2)
  {
    return DOUBLE_SINGLE_TAP;
  }
  else
  {
    return 3;
  } // any number higher than the maximum state value you return above
}

//---------------------------------------//
//-------------- ALT_LCBR ---------------//
//---------------------------------------//
// Config for left curly bracket (alt if held)
void altlbcr_finished(qk_tap_dance_state_t *state, void *user_data)
{
  td_state = cur_dance(state);
  switch (td_state)
  {
  case SINGLE_TAP:
    register_code16(KC_LCBR);
    break;
  case SINGLE_HOLD:
    register_mods(MOD_BIT(KC_LALT)); // for a layer-tap key, use `layer_on(_MY_LAYER)` here
    break;
  case DOUBLE_SINGLE_TAP: // allow nesting of 2 parens `((` within tapping term
    tap_code16(KC_LCBR);
    register_code16(KC_LCBR);
  }
}

void altlbcr_reset(qk_tap_dance_state_t *state, void *user_data)
{
  switch (td_state)
  {
  case SINGLE_TAP:
    unregister_code16(KC_LCBR);
    break;
  case SINGLE_HOLD:
    unregister_mods(MOD_BIT(KC_LALT)); // for a layer-tap key, use `layer_off(_MY_LAYER)` here
    break;
  case DOUBLE_SINGLE_TAP:
    unregister_code16(KC_LALT);
    unregister_code16(KC_LCBR);
  }
}
//---------------------------------------//
//-------------- ALT_RCBR ---------------//
//---------------------------------------//
// Config for right curly bracket (alt if held)
void altrbcr_finished(qk_tap_dance_state_t *state, void *user_data)
{
  td_state = cur_dance(state);
  switch (td_state)
  {
  case SINGLE_TAP:
    register_code16(KC_RCBR);
    break;
  case SINGLE_HOLD:
    register_mods(MOD_BIT(KC_RALT)); // for a layer-tap key, use `layer_on(_MY_LAYER)` here
    break;
  case DOUBLE_SINGLE_TAP: // allow nesting of 2 parens `((` within tapping term
    tap_code16(KC_RCBR);
    register_code16(KC_RCBR);
  }
}

void altrbcr_reset(qk_tap_dance_state_t *state, void *user_data)
{
  switch (td_state)
  {
  case SINGLE_TAP:
    unregister_code16(KC_RCBR);
    break;
  case SINGLE_HOLD:
    unregister_mods(MOD_BIT(KC_RALT)); // for a layer-tap key, use `layer_off(_MY_LAYER)` here
    break;
  case DOUBLE_SINGLE_TAP:
    unregister_code16(KC_RALT);
    unregister_code16(KC_RCBR);
  }
}

// define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in `finished` and `reset` functions
qk_tap_dance_action_t tap_dance_actions[] = {
    [ALT_LCBR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, altlbcr_finished, altlbcr_reset),
    [ALT_RCBR] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, altrbcr_finished, altrbcr_reset)};

bool process_record_user(uint16_t keycode, keyrecord_t *record)
{
  switch (keycode)
  {
  // Selects the DVORAK keyboard layout layer
  case DVORAK:
    if (record->event.pressed)
    {
      set_single_persistent_default_layer(_DVORAK);
    }
    return false;
    break;
  // Selects the QWERTY layer
  case QWERTY:
    if (record->event.pressed)
    {
      set_single_persistent_default_layer(_QWERTY);
    }
    return false;
    break;
  // Temporarily selects the SYM layer, containing symbols and numbers
  case SYM:
    if (record->event.pressed)
    {
      layer_on(_SYM);
    }
    else
    {
      layer_off(_SYM);
    }
    return false;
    break;
  // Temporarily selects the NAV layer, containing navigation keys and macros
  case NAV:
    if (record->event.pressed)
    {
      layer_on(_NAV);
    }
    else
    {
      layer_off(_NAV);
    }
    return false;
    break;
  // Temporarily selects the FUNC layer, containing function keys and macros
  case FUNC:
    if (record->event.pressed)
    {
      layer_on(_FUNC);
    }
    else
    {
      layer_off(_FUNC);
    }
    return false;
    break;
  // Writes an arrow
  case ARROW:
    if (record->event.pressed)
    {
      SEND_STRING("=>");
    }
    return false;
    break;
  // Writes a smiley
  case SMILEY:
    if (record->event.pressed)
    {
      SEND_STRING(":)");
    }
    return false;
    break;
  }
  return true;
};

bool muse_mode = false;

bool music_mask_user(uint16_t keycode)
{
  switch (keycode)
  {
  // Disables music mode for the FUNC key, so the user can deactive music mode
  case FUNC:
    return false;
  default:
    return true;
  }
}

enum combo_events {
  SCLNQ_CUT,
  SCLNJ_COPY,
  SCLNK_PASTE,
  LSLA_QWERTY,
  PDEL_DVORAK
};

const uint16_t PROGMEM cut_combo[] = {KC_SCLN, KC_Q, COMBO_END};
const uint16_t PROGMEM copy_combo[] = {KC_SCLN, KC_J, COMBO_END};
const uint16_t PROGMEM paste_combo[] = {KC_SCLN, KC_K, COMBO_END};
const uint16_t PROGMEM qwer_combo[] = {KC_L, KC_SLSH, COMBO_END};
const uint16_t PROGMEM dvor_combo[] = {KC_P, KC_DEL, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
  [SCLNQ_CUT] = COMBO_ACTION(cut_combo),
  [SCLNJ_COPY] = COMBO_ACTION(copy_combo),
  [SCLNK_PASTE] = COMBO_ACTION(paste_combo),
  [LSLA_QWERTY] = COMBO_ACTION(qwer_combo),
  [PDEL_DVORAK] = COMBO_ACTION(dvor_combo),
};

void process_combo_event(uint8_t combo_index, bool pressed) {
  switch(combo_index) {
    case SCLNQ_CUT:
      if (pressed) {
        tap_code16(LCTL(KC_X));
      }
      break;
    case SCLNJ_COPY:
      if (pressed) {
        tap_code16(LCTL(KC_C));
      }
      break;
    case SCLNK_PASTE:
      if (pressed) {
        tap_code16(LCTL(KC_V));
      }
      break;
    case LSLA_QWERTY:
      if (pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      break;
    case PDEL_DVORAK:
      if (pressed) {
        set_single_persistent_default_layer(_DVORAK);
      }
      break;
  }
}
