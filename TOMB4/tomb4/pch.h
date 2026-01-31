// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#define _USE_MATH_DEFINES
#include <windows.h>
#include <d3d.h>
#include <d3dtypes.h>
#include <cmath>
#include <stdio.h>
#include <process.h>

#include "../../resource.h"

#define DIK_ESCAPE          VK_ESCAPE
#define DIK_1               '1'
#define DIK_2               '2'
#define DIK_3               '3'
#define DIK_4               '4'
#define DIK_5               '5'
#define DIK_6               '6'
#define DIK_7               '7'
#define DIK_8               '8'
#define DIK_9               '9'
#define DIK_0               '0'
#define DIK_MINUS           VK_OEM_MINUS    /* - on main keyboard */
#define DIK_EQUALS          VK_OEM_PLUS
#define DIK_BACK            VK_BACK    /* backspace */
#define DIK_TAB             VK_TAB
#define DIK_Q               'Q'
#define DIK_W               'W'
#define DIK_E               'E'
#define DIK_R               'R'
#define DIK_T               'T'
#define DIK_Y               'Y'
#define DIK_U               'U'
#define DIK_I               'I'
#define DIK_O               'O'
#define DIK_P               'P'
#define DIK_LBRACKET        VK_OEM_4
#define DIK_RBRACKET        VK_OEM_6
#define DIK_RETURN          VK_RETURN    /* Enter on main keyboard */
#define DIK_LCONTROL        VK_LCONTROL
#define DIK_A               'A'
#define DIK_S               'S'
#define DIK_D               'D'
#define DIK_F               'F'
#define DIK_G               'G'
#define DIK_H               'H'
#define DIK_J               'J'
#define DIK_K               'K'
#define DIK_L               'L'
#define DIK_SEMICOLON       VK_OEM_1
#define DIK_APOSTROPHE      VK_OEM_7
#define DIK_GRAVE           VK_OEM_3    /* accent grave */
#define DIK_LSHIFT          VK_LSHIFT
#define DIK_BACKSLASH       VK_OEM_5
#define DIK_Z               'Z'
#define DIK_X               'X'
#define DIK_C               'C'
#define DIK_V               'V'
#define DIK_B               'B'
#define DIK_N               'N'
#define DIK_M               'M'
#define DIK_COMMA           VK_OEM_COMMA
#define DIK_PERIOD          VK_OEM_PERIOD    /* . on main keyboard */
#define DIK_SLASH           VK_OEM_2    /* / on main keyboard */
#define DIK_RSHIFT          VK_RSHIFT
#define DIK_MULTIPLY        VK_MULTIPLY    /* * on numeric keypad */
#define DIK_LMENU           VK_LMENU    /* left Alt */
#define DIK_SPACE           VK_SPACE
#define DIK_CAPITAL         VK_CAPITAL
#define DIK_F1              VK_F1
#define DIK_F2              VK_F2
#define DIK_F3              VK_F3
#define DIK_F4              VK_F4
#define DIK_F5              VK_F5
#define DIK_F6              VK_F6
#define DIK_F7              VK_F7
#define DIK_F8              VK_F8
#define DIK_F9              VK_F9
#define DIK_F10             VK_F10
#define DIK_NUMLOCK         VK_NUMLOCK
#define DIK_SCROLL          VK_SCROLL    /* Scroll Lock */
#define DIK_NUMPAD7         VK_NUMPAD7
#define DIK_NUMPAD8         VK_NUMPAD8
#define DIK_NUMPAD9         VK_NUMPAD9
#define DIK_SUBTRACT        VK_SUBTRACT    /* - on numeric keypad */
#define DIK_NUMPAD4         VK_NUMPAD4
#define DIK_NUMPAD5         VK_NUMPAD5
#define DIK_NUMPAD6         VK_NUMPAD6
#define DIK_ADD             VK_ADD    /* + on numeric keypad */
#define DIK_NUMPAD1         VK_NUMPAD1
#define DIK_NUMPAD2         VK_NUMPAD2
#define DIK_NUMPAD3         VK_NUMPAD3
#define DIK_NUMPAD0         VK_NUMPAD0
#define DIK_DECIMAL         VK_DECIMAL    /* . on numeric keypad */
#define DIK_OEM_102         0    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define DIK_F11             VK_F11
#define DIK_F12             VK_F12
#define DIK_F13             VK_F13    /*                     (NEC PC98) */
#define DIK_F14             VK_F14    /*                     (NEC PC98) */
#define DIK_F15             VK_F15    /*                     (NEC PC98) */
#define DIK_KANA            0    /* (Japanese keyboard)            */
#define DIK_ABNT_C1         0    /* /? on Brazilian keyboard */
#define DIK_CONVERT         0    /* (Japanese keyboard)            */
#define DIK_NOCONVERT       0    /* (Japanese keyboard)            */
#define DIK_YEN             0    /* (Japanese keyboard)            */
#define DIK_ABNT_C2         0    /* Numpad . on Brazilian keyboard */
#define DIK_NUMPADEQUALS    0    /* = on numeric keypad (NEC PC98) */
#define DIK_PREVTRACK       0    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
#define DIK_AT              0    /*                     (NEC PC98) */
#define DIK_COLON           0    /*                     (NEC PC98) */
#define DIK_UNDERLINE       0    /*                     (NEC PC98) */
#define DIK_KANJI           0    /* (Japanese keyboard)            */
#define DIK_STOP            0    /*                     (NEC PC98) */
#define DIK_AX              0    /*                     (Japan AX) */
#define DIK_UNLABELED       0    /*                        (J3100) */
#define DIK_NEXTTRACK       0    /* Next Track */
#define DIK_NUMPADENTER     0    /* Enter on numeric keypad */
#define DIK_RCONTROL        VK_RCONTROL
#define DIK_MUTE            0    /* Mute */
#define DIK_CALCULATOR      0    /* Calculator */
#define DIK_PLAYPAUSE       0    /* Play / Pause */
#define DIK_MEDIASTOP       0    /* Media Stop */
#define DIK_VOLUMEDOWN      0    /* Volume - */
#define DIK_VOLUMEUP        0    /* Volume + */
#define DIK_WEBHOME         0    /* Web home */
#define DIK_NUMPADCOMMA     VK_DECIMAL    /* , on numeric keypad (NEC PC98) */
#define DIK_DIVIDE          VK_DIVIDE    /* / on numeric keypad */
#define DIK_SYSRQ           0
#define DIK_RMENU           VK_RMENU    /* right Alt */
#define DIK_PAUSE           VK_PAUSE    /* Pause */
#define DIK_HOME            VK_HOME    /* Home on arrow keypad */
#define DIK_UP              VK_UP    /* UpArrow on arrow keypad */
#define DIK_PRIOR           VK_PRIOR    /* PgUp on arrow keypad */
#define DIK_LEFT            VK_LEFT    /* LeftArrow on arrow keypad */
#define DIK_RIGHT           VK_RIGHT    /* RightArrow on arrow keypad */
#define DIK_END             VK_END    /* End on arrow keypad */
#define DIK_DOWN            VK_DOWN    /* DownArrow on arrow keypad */
#define DIK_NEXT            VK_NEXT    /* PgDn on arrow keypad */
#define DIK_INSERT          VK_INSERT    /* Insert on arrow keypad */
#define DIK_DELETE          VK_DELETE    /* Delete on arrow keypad */
#define DIK_LWIN            0    /* Left Windows key */
#define DIK_RWIN            0    /* Right Windows key */
#define DIK_APPS            0    /* AppMenu key */
#define DIK_POWER           0    /* System Power */
#define DIK_SLEEP           0    /* System Sleep */
#define DIK_WAKE            0    /* System Wake */
#define DIK_WEBSEARCH       0    /* Web Search */
#define DIK_WEBFAVORITES    0    /* Web Favorites */
#define DIK_WEBREFRESH      0    /* Web Refresh */
#define DIK_WEBSTOP         0    /* Web Stop */
#define DIK_WEBFORWARD      0    /* Web Forward */
#define DIK_WEBBACK         0    /* Web Back */
#define DIK_MYCOMPUTER      0    /* My Computer */
#define DIK_MAIL            0    /* Mail */
#define DIK_MEDIASELECT     0    /* Media Select */

/*
 *  Alternate names for keys, to facilitate transition from DOS.
 */
#define DIK_BACKSPACE       DIK_BACK            /* backspace */
#define DIK_NUMPADSTAR      DIK_MULTIPLY        /* * on numeric keypad */
#define DIK_LALT            DIK_LMENU           /* left Alt */
#define DIK_CAPSLOCK        DIK_CAPITAL         /* CapsLock */
#define DIK_NUMPADMINUS     DIK_SUBTRACT        /* - on numeric keypad */
#define DIK_NUMPADPLUS      DIK_ADD             /* + on numeric keypad */
#define DIK_NUMPADPERIOD    DIK_DECIMAL         /* . on numeric keypad */
#define DIK_NUMPADSLASH     DIK_DIVIDE          /* / on numeric keypad */
#define DIK_RALT            DIK_RMENU           /* right Alt */
#define DIK_UPARROW         DIK_UP              /* UpArrow on arrow keypad */
#define DIK_PGUP            DIK_PRIOR           /* PgUp on arrow keypad */
#define DIK_LEFTARROW       DIK_LEFT            /* LeftArrow on arrow keypad */
#define DIK_RIGHTARROW      DIK_RIGHT           /* RightArrow on arrow keypad */
#define DIK_DOWNARROW       DIK_DOWN            /* DownArrow on arrow keypad */
#define DIK_PGDN            DIK_NEXT            /* PgDn on arrow keypad */

#endif //PCH_H
