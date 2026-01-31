#include "../tomb4/pch.h"
#include "input.h"
#include "dxshell.h"
#include "../game/sound.h"
#include "../game/newinv.h"
#include "../game/lara_states.h"
#include "../game/control.h"
#include "../game/camera.h"
#include "LoadSave.h"
#include "winmain.h"
#include "../game/lara.h"
#include "../game/savegame.h"
#include "../game/gameflow.h"
#include "../game/spotcam.h"
#include "../tomb4/tomb4.h"

const char* KeyboardButtons[272] =
//{
//	0,
//	"Esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "+", "Bksp",
//	"Tab", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "<", ">", "Return",
//	"Ctrl", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "`",
//	"Shift", "#", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/", "Shift",
//	"Padx", "Alt", "Space", "Caps", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	"Nmlk", 0,
//	"Pad7", "Pad8", "Pad9", "Pad-",
//	"Pad4", "Pad5", "Pad6", "Pad+",
//	"Pad1", "Pad2", "Pad3",
//	"Pad0", "Pad.", 0, 0, "\\", 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	"Enter", "Ctrl", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	"Shift", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	"Pad/", 0, 0, "Alt", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	"Home", "Up", "PgUp", 0, "Left", 0, "Right", 0, "End", "Down", "PgDn", "Ins", "Del",
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//	"Joy 1", "Joy 2", "Joy 3", "Joy 4", "Joy 5", "Joy 6", "Joy 7", "Joy 8",
//	"Joy 9", "Joy 10", "Joy 11", "Joy 12", "Joy 13", "Joy 14", "Joy 15", "Joy 16"
//};

{
	/* 0x00 */ 0,                          // Reserved
	/* 0x01 */ "Left mouse button",
	/* 0x02 */ "Right mouse button",
	/* 0x03 */ "Control-break",
	/* 0x04 */ "Middle mouse button",
	/* 0x05 */ "X1 mouse button",
	/* 0x06 */ "X2 mouse button",
	/* 0x07 */ 0,
	/* 0x08 */ "Backspace",
	/* 0x09 */ "Tab",
	/* 0x0A */ 0,
	/* 0x0B */ 0,
	/* 0x0C */ "Clear",
	/* 0x0D */ "Enter",
	/* 0x0E */ 0,
	/* 0x0F */ 0,

	/* 0x10 */ "Shift",
	/* 0x11 */ "Control",
	/* 0x12 */ "Alt",
	/* 0x13 */ "Pause",
	/* 0x14 */ "Caps Lock",
	/* 0x15 */ "IME Kana / Hangul",
	/* 0x16 */ 0,
	/* 0x17 */ "IME Junja",
	/* 0x18 */ "IME Final",
	/* 0x19 */ "IME Hanja / Kanji",
	/* 0x1A */ 0,
	/* 0x1B */ "Escape",
	/* 0x1C */ "IME Convert",
	/* 0x1D */ "IME Nonconvert",
	/* 0x1E */ "IME Accept",
	/* 0x1F */ "IME Mode change",

	/* 0x20 */ "Space",
	/* 0x21 */ "Page Up",
	/* 0x22 */ "Page Down",
	/* 0x23 */ "End",
	/* 0x24 */ "Home",
	/* 0x25 */ "Left Arrow",
	/* 0x26 */ "Up Arrow",
	/* 0x27 */ "Right Arrow",
	/* 0x28 */ "Down Arrow",
	/* 0x29 */ "Select",
	/* 0x2A */ "Print",
	/* 0x2B */ "Execute",
	/* 0x2C */ "Print Screen",
	/* 0x2D */ "Insert",
	/* 0x2E */ "Delete",
	/* 0x2F */ "Help",

	/* 0x30 */ "0",
	/* 0x31 */ "1",
	/* 0x32 */ "2",
	/* 0x33 */ "3",
	/* 0x34 */ "4",
	/* 0x35 */ "5",
	/* 0x36 */ "6",
	/* 0x37 */ "7",
	/* 0x38 */ "8",
	/* 0x39 */ "9",

	/* 0x3A */ 0,
	/* 0x3B */ 0,
	/* 0x3C */ 0,
	/* 0x3D */ 0,
	/* 0x3E */ 0,
	/* 0x3F */ 0,

	/* 0x40 */ 0,

	/* 0x41 */ "A",
	/* 0x42 */ "B",
	/* 0x43 */ "C",
	/* 0x44 */ "D",
	/* 0x45 */ "E",
	/* 0x46 */ "F",
	/* 0x47 */ "G",
	/* 0x48 */ "H",
	/* 0x49 */ "I",
	/* 0x4A */ "J",
	/* 0x4B */ "K",
	/* 0x4C */ "L",
	/* 0x4D */ "M",
	/* 0x4E */ "N",
	/* 0x4F */ "O",
	/* 0x50 */ "P",
	/* 0x51 */ "Q",
	/* 0x52 */ "R",
	/* 0x53 */ "S",
	/* 0x54 */ "T",
	/* 0x55 */ "U",
	/* 0x56 */ "V",
	/* 0x57 */ "W",
	/* 0x58 */ "X",
	/* 0x59 */ "Y",
	/* 0x5A */ "Z",

	/* 0x5B */ "Left Windows",
	/* 0x5C */ "Right Windows",
	/* 0x5D */ "Applications",

	/* 0x5E */ 0,

	/* 0x5F */ "Sleep",

	/* 0x60 */ "Numpad 0",
	/* 0x61 */ "Numpad 1",
	/* 0x62 */ "Numpad 2",
	/* 0x63 */ "Numpad 3",
	/* 0x64 */ "Numpad 4",
	/* 0x65 */ "Numpad 5",
	/* 0x66 */ "Numpad 6",
	/* 0x67 */ "Numpad 7",
	/* 0x68 */ "Numpad 8",
	/* 0x69 */ "Numpad 9",
	/* 0x6A */ "Numpad Multiply",
	/* 0x6B */ "Numpad Add",
	/* 0x6C */ "Numpad Separator",
	/* 0x6D */ "Numpad Subtract",
	/* 0x6E */ "Numpad Decimal",
	/* 0x6F */ "Numpad Divide",

	/* 0x70 */ "F1",
	/* 0x71 */ "F2",
	/* 0x72 */ "F3",
	/* 0x73 */ "F4",
	/* 0x74 */ "F5",
	/* 0x75 */ "F6",
	/* 0x76 */ "F7",
	/* 0x77 */ "F8",
	/* 0x78 */ "F9",
	/* 0x79 */ "F10",
	/* 0x7A */ "F11",
	/* 0x7B */ "F12",
	/* 0x7C */ "F13",
	/* 0x7D */ "F14",
	/* 0x7E */ "F15",
	/* 0x7F */ "F16",
	/* 0x80 */ "F17",
	/* 0x81 */ "F18",
	/* 0x82 */ "F19",
	/* 0x83 */ "F20",
	/* 0x84 */ "F21",
	/* 0x85 */ "F22",
	/* 0x86 */ "F23",
	/* 0x87 */ "F24",

	/* 0x88 */ 0, /* 0x89 */ 0, /* 0x8A */ 0, /* 0x8B */ 0,
	/* 0x8C */ 0, /* 0x8D */ 0, /* 0x8E */ 0, /* 0x8F */ 0,

	/* 0x90 */ "Num Lock",
	/* 0x91 */ "Scroll Lock",

	/* 0x92 */ 0, /* 0x93 */ 0, /* 0x94 */ 0, /* 0x95 */ 0,
	/* 0x96 */ 0, /* 0x97 */ 0,0,0,0,0,0,0,0,0,

	/* 0xA0 */ "Left Shift",
	/* 0xA1 */ "Right Shift",
	/* 0xA2 */ "Left Control",
	/* 0xA3 */ "Right Control",
	/* 0xA4 */ "Left Alt",
	/* 0xA5 */ "Right Alt",

	/* 0xA6 */ "Browser Back",
	/* 0xA7 */ "Browser Forward",
	/* 0xA8 */ "Browser Refresh",
	/* 0xA9 */ "Browser Stop",
	/* 0xAA */ "Browser Search",
	/* 0xAB */ "Browser Favorites",
	/* 0xAC */ "Browser Home",

	/* 0xAD */ "Volume Mute",
	/* 0xAE */ "Volume Down",
	/* 0xAF */ "Volume Up",
	/* 0xB0 */ "Next Track",
	/* 0xB1 */ "Previous Track",
	/* 0xB2 */ "Stop Media",
	/* 0xB3 */ "Play/Pause Media",
	/* 0xB4 */ "Mail",
	/* 0xB5 */ "Media Select",
	/* 0xB6 */ "Application 1",
	/* 0xB7 */ "Application 2",

	/* 0xB8 */ 0, /* 0xB9 */ 0,

	/* 0xBA */ "OEM ;",
	/* 0xBB */ "OEM +",
	/* 0xBC */ "OEM ,",
	/* 0xBD */ "OEM -",
	/* 0xBE */ "OEM .",
	/* 0xBF */ "OEM /",
	/* 0xC0 */ "OEM `",

	/* 0xC1 */ 0, /* 0xC2 */ 0, /* 0xC3 */ 0, /* 0xC4 */ 0,
	/* 0xC5 */ 0, /* 0xC6 */ 0, /* 0xC7 */ 0,

	/* 0xC8 */ 0, /* 0xC9 */ 0, /* 0xCA */ 0, /* 0xCB */ 0,
	/* 0xCC */ 0, /* 0xCD */ 0, /* 0xCE */ 0, /* 0xCF */ 0,

	/* 0xD0 */ 0, /* 0xD1 */ 0, /* 0xD2 */ 0, /* 0xD3 */ 0,
	/* 0xD4 */ 0, /* 0xD5 */ 0, /* 0xD6 */ 0, /* 0xD7 */ 0,
	/* 0xD8 */ 0, /* 0xD9 */ 0, /* 0xDA */ 0,

	/* 0xDB */ "OEM [",
	/* 0xDC */ "OEM \\",
	/* 0xDD */ "OEM ]",
	/* 0xDE */ "OEM '",
	/* 0xDF */ "OEM 8",

	/* 0xE0 */ 0,
	/* 0xE1 */ "OEM AX",
	/* 0xE2 */ "OEM 102",
	/* 0xE3 */ "ICO Help",
	/* 0xE4 */ "ICO 00",
	/* 0xE5 */ 0,
	/* 0xE6 */ "ICO Clear",
	/* 0xE7 */ 0,
	/* 0xE8 */ 0,

	/* 0xE9 */ "OEM Reset",
	/* 0xEA */ "OEM Jump",
	/* 0xEB */ "OEM PA1",
	/* 0xEC */ "OEM PA2",
	/* 0xED */ "OEM PA3",
	/* 0xEE */ "OEM WSCTRL",
	/* 0xEF */ "OEM CUSEL",
	/* 0xF0 */ "OEM ATTN",
	/* 0xF1 */ "OEM Finish",
	/* 0xF2 */ "OEM Copy",
	/* 0xF3 */ "OEM Auto",
	/* 0xF4 */ "OEM Enlw",
	/* 0xF5 */ "OEM BackTab",
};

const char* GermanKeyboard[272] =
{
	0,
	"Esc", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "=", "`", "Entf",
	"Tab", "Q", "W", "E", "R", "T", "Z", "U", "I", "O", "P", "~u", "x", "Enter",
	"Strg", "A", "S", "D", "F", "G", "H", "J", "K", "L", "~o", "~a", "( ",
	"Shift", "#", "Y", "X", "C", "V", "B", "N", "M", ",", ".", "-", "Shift",
	"Num x", "Alt", "Leert.", "Caps", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	"Nmlk", 0,
	"Num7", "Num8", "Num9", "Num-",
	"Num4", "Num5", "Num6", "Num+",
	"Num1", "Num2", "Num3",
	"Num0", "Num.", 0, 0, ">", 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	"Enter", "Strg", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	"Shift", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	"Num/", 0, 0, "Alt", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	"Pos1", "Hoch", "BdAuf", 0, "Links", 0, "Rechts", 0, "Ende", "Runter", "BdAb", "Einfg", "Entf",
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	"Joy 1", "Joy 2", "Joy 3", "Joy 4", "Joy 5", "Joy 6", "Joy 7", "Joy 8",
	"Joy 9", "Joy 10", "Joy 11", "Joy 12", "Joy 13", "Joy 14", "Joy 15", "Joy 16"
};

short layout[2][18] =
{
	{ DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT, DIK_PERIOD, DIK_SLASH, DIK_RSHIFT, DIK_RALT, DIK_RCONTROL,
	DIK_SPACE, DIK_COMMA, DIK_NUMPAD0, DIK_END, DIK_ESCAPE, DIK_DELETE, DIK_PGDN, DIK_P, DIK_RETURN },

	{ DIK_UP, DIK_DOWN, DIK_LEFT, DIK_RIGHT, DIK_PERIOD, DIK_SLASH, DIK_RSHIFT, DIK_RALT, DIK_RCONTROL,
	DIK_SPACE, DIK_COMMA, DIK_NUMPAD0, DIK_END, DIK_ESCAPE, DIK_DELETE, DIK_PGDN, DIK_P, DIK_RETURN }
};

long conflict[18];
long input;
long linput;
long dbinput;
long inputBusy;
short ammo_change_timer = 0;
char ammo_change_buf[12];

static long joy_x;
static long joy_y;
static long joy_fire;

static void DoWeaponHotkey()	//adds extra checks and does ammo type swaps..
{
	short state;
	bool goin;

	if (!lara_item)
		goin = 0;
	else
	{
		state = lara_item->current_anim_state;
		goin = !(gfLevelFlags & GF_YOUNGLARA) && (lara.water_status == LW_ABOVE_WATER || lara.water_status == LW_WADE) && !bDisableLaraControl &&
			(state != AS_ALL4S && state != AS_CRAWL && state != AS_ALL4TURNL && state != AS_ALL4TURNR && state != AS_CRAWLBACK &&
				state != AS_CRAWL2HANG && state != AS_DUCK && state != AS_DUCKROTL && state != AS_DUCKROTR);
	}

	if (!goin)
		return;

	if (keymap[DIK_1])
	{
		if (!(lara.pistols_type_carried & W_PRESENT))
			return;

		lara.request_gun_type = WEAPON_PISTOLS;

		if (lara.gun_status == LG_NO_ARMS && lara.gun_type == WEAPON_PISTOLS)
			lara.gun_status = LG_DRAW_GUNS;
	}
	else if (keymap[DIK_2])
	{
		if (!(lara.shotgun_type_carried & W_PRESENT))
			return;

		lara.request_gun_type = WEAPON_SHOTGUN;

		if (lara.gun_type == WEAPON_SHOTGUN)
		{
			if (lara.gun_status == LG_NO_ARMS)
				lara.gun_status = LG_DRAW_GUNS;
			else if (lara.gun_status == LG_READY && !ammo_change_timer)
			{
				if (!tomb4.ammotype_hotkeys)
					return;

				memset(ammo_change_buf, 0, sizeof(ammo_change_buf));

				if (lara.shotgun_type_carried & W_AMMO2)
				{
					lara.shotgun_type_carried &= ~W_AMMO2;
					lara.shotgun_type_carried |= W_AMMO1;
					ammo_change_timer = 30;
					sprintf(ammo_change_buf, "Normal");
				}
				else if (lara.shotgun_type_carried & W_AMMO1)
				{
					lara.shotgun_type_carried &= ~W_AMMO1;
					lara.shotgun_type_carried |= W_AMMO2;
					ammo_change_timer = 30;
					sprintf(ammo_change_buf, "Wideshot");
				}
			}
		}
	}
	else if (keymap[DIK_3])
	{
		if (!(lara.uzis_type_carried & W_PRESENT))
			return;

		lara.request_gun_type = WEAPON_UZI;

		if (lara.gun_status == LG_NO_ARMS && lara.gun_type == WEAPON_UZI)
			lara.gun_status = LG_DRAW_GUNS;
	}
	else if (keymap[DIK_4])
	{
		if (!(lara.sixshooter_type_carried & W_PRESENT))
			return;

		lara.request_gun_type = WEAPON_REVOLVER;

		if (lara.gun_status == LG_NO_ARMS && lara.gun_type == WEAPON_REVOLVER)
			lara.gun_status = LG_DRAW_GUNS;
	}
	else if (keymap[DIK_5])
	{
		if (!(lara.grenade_type_carried & W_PRESENT))
			return;

		lara.request_gun_type = WEAPON_GRENADE;

		if (lara.gun_type == WEAPON_GRENADE)
		{
			if (lara.gun_status == LG_NO_ARMS)
				lara.gun_status = LG_DRAW_GUNS;
			else if (lara.gun_status == LG_READY && !ammo_change_timer)
			{
				if (!tomb4.ammotype_hotkeys)
					return;

				memset(ammo_change_buf, 0, sizeof(ammo_change_buf));

				if (lara.grenade_type_carried & W_AMMO3)
				{
					lara.grenade_type_carried &= ~W_AMMO3;
					lara.grenade_type_carried |= W_AMMO2;
					ammo_change_timer = 30;
					sprintf(ammo_change_buf, "Super");
				}
				else if (lara.grenade_type_carried & W_AMMO2)
				{
					lara.grenade_type_carried &= ~W_AMMO2;
					lara.grenade_type_carried |= W_AMMO1;
					ammo_change_timer = 30;
					sprintf(ammo_change_buf, "Normal");
				}
				else if (lara.grenade_type_carried & W_AMMO1)
				{
					lara.grenade_type_carried &= ~W_AMMO1;
					lara.grenade_type_carried |= W_AMMO3;
					ammo_change_timer = 30;
					sprintf(ammo_change_buf, "Flash");
				}
			}
		}
	}
	else if (keymap[DIK_6])
	{
		if (!(lara.crossbow_type_carried & W_PRESENT))
			return;

		lara.request_gun_type = WEAPON_CROSSBOW;

		if (lara.gun_type == WEAPON_CROSSBOW)
		{
			if (lara.gun_status == LG_NO_ARMS)
				lara.gun_status = LG_DRAW_GUNS;
			else if (lara.gun_status == LG_READY && !ammo_change_timer)
			{
				if (!tomb4.ammotype_hotkeys)
					return;

				memset(ammo_change_buf, 0, sizeof(ammo_change_buf));

				if (lara.crossbow_type_carried & W_AMMO3)
				{
					lara.crossbow_type_carried &= ~W_AMMO3;
					lara.crossbow_type_carried |= W_AMMO2;
					ammo_change_timer = 30;
					sprintf(ammo_change_buf, "Poison");
				}
				else if (lara.crossbow_type_carried & W_AMMO2)
				{
					lara.crossbow_type_carried &= ~W_AMMO2;
					lara.crossbow_type_carried |= W_AMMO1;
					ammo_change_timer = 30;
					sprintf(ammo_change_buf, "Normal");
				}
				else if (lara.crossbow_type_carried & W_AMMO1)
				{
					lara.crossbow_type_carried &= ~W_AMMO1;
					lara.crossbow_type_carried |= W_AMMO3;
					ammo_change_timer = 30;
					sprintf(ammo_change_buf, "Explosive");
				}
			}
		}
	}
}

long Key(long number)
{
	short key;

	key = layout[1][number];

	if (key < 256)
	{
		if (keymap[key])
			return 1;

		switch (key)
		{
		case DIK_RCONTROL:
			return keymap[DIK_LCONTROL];

		case DIK_LCONTROL:
			return keymap[DIK_RCONTROL];

		case DIK_RSHIFT:
			return keymap[DIK_LSHIFT];

		case DIK_LSHIFT:
			return keymap[DIK_RSHIFT];

		case DIK_RMENU:
			return keymap[DIK_LMENU];

		case DIK_LMENU:
			return keymap[DIK_RMENU];
		}
	}
	else if (joy_fire & (1 << key))
		return 1;

	if (conflict[number])
		return 0;

	key = layout[0][number];

	if (keymap[key])
		return 1;

	switch (key)
	{
	case DIK_RCONTROL:
		return keymap[DIK_LCONTROL];

	case DIK_LCONTROL:
		return keymap[DIK_RCONTROL];

	case DIK_RSHIFT:
		return keymap[DIK_LSHIFT];

	case DIK_LSHIFT:
		return keymap[DIK_RSHIFT];

	case DIK_RMENU:
		return keymap[DIK_LMENU];

	case DIK_LMENU:
		return keymap[DIK_RMENU];
	}

	return 0;
}

long S_UpdateInput()
{
	static long LookCnt;
	static long med_hotkey_timer;
	short state;
	static bool flare_no_db = 0;
	bool debounce;

	debounce = SetDebounce;
	DXReadKeyboard(keymap);

	if (ControlMethod == 1)
		joy_fire = ReadJoystick(joy_x, joy_y);

	linput = 0;

	if (ControlMethod == 1)
	{
		if (joy_x < -8)
			linput = IN_LEFT;
		else if (joy_x > 8)
			linput = IN_RIGHT;

		if (joy_y > 8)
			linput |= IN_BACK;
		else if (joy_y < -8)
			linput |= IN_FORWARD;
	}

	if (Key(0))
		linput |= IN_FORWARD;

	if (Key(1))
		linput |= IN_BACK;

	if (Key(2))
		linput |= IN_LEFT;

	if (Key(3))
		linput |= IN_RIGHT;

	if (Key(4))
		linput |= IN_DUCK;

	if (Key(5))
		linput |= IN_SPRINT;

	if (Key(6))
		linput |= IN_WALK;

	if (Key(7))
		linput |= IN_JUMP;

	if (Key(8))
		linput |= IN_SELECT | IN_ACTION;

	if (Key(9))
		linput |= IN_DRAW;

	if (Key(10))
	{
		if (!flare_no_db)
		{
			state = lara_item->current_anim_state;

			if (state == AS_ALL4S || state == AS_CRAWL || state == AS_ALL4TURNL ||
				state == AS_ALL4TURNR || state == AS_CRAWLBACK || state == AS_CRAWL2HANG)
			{
				SoundEffect(SFX_LARA_NO, 0, SFX_ALWAYS);
				flare_no_db = 1;
			}
			else
			{
				flare_no_db = 0;
				linput |= IN_FLARE;
			}
		}
	}
	else
		flare_no_db = 0;

	if (Key(11))
		linput |= IN_LOOK;

	if (Key(12))
		linput |= IN_ROLL;

	if (Key(13))
		linput |= IN_OPTION;

	if (Key(14))
		linput |= IN_WALK | IN_LEFT;

	if (Key(15))
		linput |= IN_WALK | IN_RIGHT;

	if (Key(16))
		linput |= IN_PAUSE;

	if (Key(17))
		linput |= IN_SELECT;

	if (keymap[DIK_ESCAPE])
		linput |= IN_DESELECT | IN_OPTION;

	if (lara.gun_status == LG_READY)
	{
		savegame.AutoTarget = (uchar)App.AutoTarget;

		if (linput & IN_LOOK)
		{
			if (LookCnt >= 6)
				LookCnt = 100;
			else
			{
				linput &= ~IN_LOOK;
				LookCnt++;
			}
		}
		else
		{
			if (LookCnt && LookCnt != 100)
				linput |= IN_TARGET;

			LookCnt = 0;
		}
	}

	DoWeaponHotkey();

	if (keymap[DIK_0])
	{
		if (!med_hotkey_timer)
		{
			if (lara_item->hit_points > 0 && lara_item->hit_points < 1000 || lara.poisoned)
			{
				if (lara.num_small_medipack)
				{
					if (lara.num_small_medipack != -1)
						lara.num_small_medipack--;

					lara.dpoisoned = 0;
					lara_item->hit_points += 500;
					SoundEffect(SFX_MENU_MEDI, 0, SFX_ALWAYS);
					savegame.Game.HealthUsed++;

					if (lara_item->hit_points > 1000)
						lara_item->hit_points = 1000;

					if (InventoryActive && !lara.num_small_medipack)
					{
						construct_object_list();

						if (lara.num_large_medipack)
							setup_objectlist_startposition(INV_BIGMEDI_ITEM);
						else
							setup_objectlist_startposition(INV_MEMCARD_LOAD_ITEM);
					}

					med_hotkey_timer = 15;
				}
			}
		}
	}
	else if (keymap[DIK_9])
	{
		if (!med_hotkey_timer)
		{
			if (lara_item->hit_points > 0 && lara_item->hit_points < 1000 || lara.poisoned)
			{
				if (lara.num_large_medipack)
				{
					if (lara.num_large_medipack != -1)
						lara.num_large_medipack--;

					lara.dpoisoned = 0;
					lara_item->hit_points = 1000;
					SoundEffect(SFX_MENU_MEDI, 0, SFX_ALWAYS);
					savegame.Game.HealthUsed++;
					med_hotkey_timer = 15;

					if (InventoryActive && !lara.num_large_medipack)
					{
						construct_object_list();

						if (lara.num_small_medipack)
							setup_objectlist_startposition(INV_SMALLMEDI_ITEM);
						else
							setup_objectlist_startposition(INV_MEMCARD_LOAD_ITEM);
					}
				}
			}
		}
	}
	else if (med_hotkey_timer)
		med_hotkey_timer--;

	if (linput & IN_WALK && !(linput & (IN_FORWARD | IN_BACK)))
	{
		if (linput & IN_LEFT)
			linput = (linput & ~IN_LEFT) | IN_LSTEP;
		else if (linput & IN_RIGHT)
			linput = (linput & ~IN_RIGHT) | IN_RSTEP;
	}

	if (linput & IN_FORWARD && linput & IN_BACK)
		linput |= IN_ROLL;

	if (linput & IN_ROLL && BinocularRange)
		linput &= ~IN_ROLL;

	if ((linput & (IN_RIGHT | IN_LEFT)) == (IN_RIGHT | IN_LEFT))
		linput -= IN_RIGHT | IN_LEFT;

	if (debounce)
		dbinput = inputBusy;

	if (!gfGameMode && Gameflow->LoadSaveEnabled)
	{
		if (keymap[DIK_F5])
			linput |= IN_SAVE;

		if (keymap[DIK_F6])
			linput |= IN_LOAD;
	}

	if (keymap[DIK_APOSTROPHE])
		DXSaveScreen(App.dx.lpBackBuffer, "Tomb");

	inputBusy = linput;

	if (lara.Busy)
	{
		linput &= IN_PAUSE | IN_LOOK | IN_OPTION | IN_RIGHT | IN_LEFT | IN_BACK | IN_FORWARD;

		if (linput & IN_FORWARD && linput & IN_BACK)
			linput ^= IN_BACK;
	}

	// binoculars hotkey
	if (keymap[DIK_Q])
	{
		if ((lara_item->current_anim_state == AS_STOP && lara_item->anim_number == ANIM_BREATH || lara.IsDucked && !(input & IN_DUCK)))
		{
			//oldLaraBusy = 1;
			BinocularRange = 128;

			if (lara.gun_status != LG_NO_ARMS)
				lara.gun_status = LG_UNDRAW_GUNS;
		}
	}

	if (debounce)
		dbinput = inputBusy & (dbinput ^ inputBusy);

	input = linput;
	return 1;
}

long ReadJoystick(long& x, long& y)
{
	JOYINFOEX joystick;
	static JOYCAPS caps;
	static long unavailable = 1;

	joystick.dwSize = sizeof(JOYINFOEX);
	joystick.dwFlags = JOY_RETURNX | JOY_RETURNY | JOY_RETURNBUTTONS;

	if (joyGetPosEx(0, &joystick) != JOYERR_NOERROR)
	{
		unavailable = 1;
		x = 0;
		y = 0;
		return 0;
	}

	if (unavailable)
	{
		if (joyGetDevCaps(JOYSTICKID1, &caps, sizeof(caps)) != JOYERR_NOERROR)
		{
			x = 0;
			y = 0;
			return 0;
		}
		else
			unavailable = 0;
	}

	x = (joystick.dwXpos << 5) / (caps.wXmax - caps.wXmin) - 16;
	y = (joystick.dwYpos << 5) / (caps.wYmax - caps.wYmin) - 16;
	return joystick.dwButtons;
}
