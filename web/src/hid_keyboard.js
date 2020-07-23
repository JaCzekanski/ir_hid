// From https://github.com/NicoHood/HID/blob/d4938ddcff7970bc1d32a040a08afeac4915e4a9/src/HID-APIs/ImprovedKeylayouts.h
const HidKeyboard = [
	{ name: "", id: 0 },
	{ name: "KEY_RESERVED", id: 0 },
	{ name: "KEY_ERROR_ROLLOVER", id: 1 },
	{ name: "KEY_POST_FAIL", id: 2 },
	{ name: "KEY_ERROR_UNDEFINED", id: 3 },
	{ name: "KEY_A", id: 4 },
	{ name: "KEY_B", id: 5 },
	{ name: "KEY_C", id: 6 },
	{ name: "KEY_D", id: 7 },
	{ name: "KEY_E", id: 8 },
	{ name: "KEY_F", id: 9 },
	{ name: "KEY_G", id: 10 },
	{ name: "KEY_H", id: 11 },
	{ name: "KEY_I", id: 12 },
	{ name: "KEY_J", id: 13 },
	{ name: "KEY_K", id: 14 },
	{ name: "KEY_L", id: 15 },
	{ name: "KEY_M", id: 16 },
	{ name: "KEY_N", id: 17 },
	{ name: "KEY_O", id: 18 },
	{ name: "KEY_P", id: 19 },
	{ name: "KEY_Q", id: 20 },
	{ name: "KEY_R", id: 21 },
	{ name: "KEY_S", id: 22 },
	{ name: "KEY_T", id: 23 },
	{ name: "KEY_U", id: 24 },
	{ name: "KEY_V", id: 25 },
	{ name: "KEY_W", id: 26 },
	{ name: "KEY_X", id: 27 },
	{ name: "KEY_Y", id: 28 },
	{ name: "KEY_Z", id: 29 },
	{ name: "KEY_1", id: 30 },
	{ name: "KEY_2", id: 31 },
	{ name: "KEY_3", id: 32 },
	{ name: "KEY_4", id: 33 },
	{ name: "KEY_5", id: 34 },
	{ name: "KEY_6", id: 35 },
	{ name: "KEY_7", id: 36 },
	{ name: "KEY_8", id: 37 },
	{ name: "KEY_9", id: 38 },
	{ name: "KEY_0", id: 39 },
	{ name: "KEY_ENTER", id: 40 },
	{ name: "KEY_ESC", id: 41 },
	{ name: "KEY_BACKSPACE", id: 42 },
	{ name: "KEY_TAB", id: 43 },
	{ name: "KEY_SPACE", id: 44 },
	{ name: "KEY_MINUS", id: 45 },
	{ name: "KEY_EQUAL", id: 46 },
	{ name: "KEY_LEFT_BRACE", id: 47 },
	{ name: "KEY_RIGHT_BRACE", id: 48 },
	{ name: "KEY_BACKSLASH", id: 49 },
	{ name: "KEY_NON_US_NUM", id: 50 },
	{ name: "KEY_SEMICOLON", id: 51 },
	{ name: "KEY_QUOTE", id: 52 },
	{ name: "KEY_TILDE", id: 53 },
	{ name: "KEY_COMMA", id: 54 },
	{ name: "KEY_PERIOD", id: 55 },
	{ name: "KEY_SLASH", id: 56 },
	{ name: "KEY_CAPS_LOCK", id: 0x39 },
	{ name: "KEY_F1", id: 0x3A },
	{ name: "KEY_F2", id: 0x3B },
	{ name: "KEY_F3", id: 0x3C },
	{ name: "KEY_F4", id: 0x3D },
	{ name: "KEY_F5", id: 0x3E },
	{ name: "KEY_F6", id: 0x3F },
	{ name: "KEY_F7", id: 0x40 },
	{ name: "KEY_F8", id: 0x41 },
	{ name: "KEY_F9", id: 0x42 },
	{ name: "KEY_F10", id: 0x43 },
	{ name: "KEY_F11", id: 0x44 },
	{ name: "KEY_F12", id: 0x45 },
	{ name: "KEY_PRINTSCREEN", id: 0x46 },
	{ name: "KEY_SCROLL_LOCK", id: 0x47 },
	{ name: "KEY_PAUSE", id: 0x48 },
	{ name: "KEY_INSERT", id: 0x49 },
	{ name: "KEY_HOME", id: 0x4A },
	{ name: "KEY_PAGE_UP", id: 0x4B },
	{ name: "KEY_DELETE", id: 0x4C },
	{ name: "KEY_END", id: 0x4D },
	{ name: "KEY_PAGE_DOWN", id: 0x4E },
	{ name: "KEY_RIGHT_ARROW", id: 0x4F },
	{ name: "KEY_LEFT_ARROW", id: 0x50 },
	{ name: "KEY_DOWN_ARROW", id: 0x51 },
	{ name: "KEY_UP_ARROW", id: 0x52 },
	{ name: "KEY_RIGHT", id: 0x4F },
	{ name: "KEY_LEFT", id: 0x50 },
	{ name: "KEY_DOWN", id: 0x51 },
	{ name: "KEY_UP", id: 0x52 },
	{ name: "KEY_NUM_LOCK", id: 0x53 },
	{ name: "KEYPAD_DIVIDE", id: 0x54 },
	{ name: "KEYPAD_MULTIPLY", id: 0x55 },
	{ name: "KEYPAD_SUBTRACT", id: 0x56 },
	{ name: "KEYPAD_ADD", id: 0x57 },
	{ name: "KEYPAD_ENTER", id: 0x58 },
	{ name: "KEYPAD_1", id: 0x59 },
	{ name: "KEYPAD_2", id: 0x5A },
	{ name: "KEYPAD_3", id: 0x5B },
	{ name: "KEYPAD_4", id: 0x5C },
	{ name: "KEYPAD_5", id: 0x5D },
	{ name: "KEYPAD_6", id: 0x5E },
	{ name: "KEYPAD_7", id: 0x5F },
	{ name: "KEYPAD_8", id: 0x60 },
	{ name: "KEYPAD_9", id: 0x61 },
	{ name: "KEYPAD_0", id: 0x62 },
	{ name: "KEYPAD_DOT", id: 0x63 },
	{ name: "KEY_NON_US", id: 0x64 },
	{ name: "KEY_MENU", id: 0x65 },

	// Most of the following keys will only work with Linux or not at all.
	// F13+ keys are mostly used for laptop functions like ECO key.
	{ name: "KEY_POWER", id: 0x66 }, // PowerOff (Ubuntu)
	{ name: "KEY_PAD_EQUALS", id: 0x67 }, // Dont confuse with KEYPAD_EQUAL_SIGN
	{ name: "KEY_F13", id: 0x68 }, // Tools (Ubunutu)
	{ name: "KEY_F14", id: 0x69 }, // Launch5 (Ubuntu)
	{ name: "KEY_F15", id: 0x6A }, // Launch6 (Ubuntu)
	{ name: "KEY_F16", id: 0x6B }, // Launch7 (Ubuntu)
	{ name: "KEY_F17", id: 0x6C }, // Launch8 (Ubuntu)
	{ name: "KEY_F18", id: 0x6D }, // Launch9 (Ubuntu)
	{ name: "KEY_F19", id: 0x6E }, // Disabled (Ubuntu)
	{ name: "KEY_F20", id: 0x6F }, // AudioMicMute (Ubuntu)
	{ name: "KEY_F21", id: 0x70 }, // Touchpad toggle (Ubuntu)
	{ name: "KEY_F22", id: 0x71 }, // TouchpadOn (Ubuntu)
	{ name: "KEY_F23", id: 0x72 }, // TouchpadOff Ubuntu)
	{ name: "KEY_F24", id: 0x73 }, // Disabled (Ubuntu)
	{ name: "KEY_EXECUTE", id: 0x74 }, // Open (Ubuntu)
	{ name: "KEY_HELP", id: 0x75 }, // Help (Ubuntu)
	{ name: "KEY_MENU2", id: 0x76 }, // Disabled (Ubuntu)
	{ name: "KEY_SELECT", id: 0x77 }, // Disabled (Ubuntu)
	{ name: "KEY_STOP", id: 0x78 }, // Cancel (Ubuntu)
	{ name: "KEY_AGAIN", id: 0x79 }, // Redo (Ubuntu)
	{ name: "KEY_UNDO", id: 0x7A }, // Undo (Ubuntu)
	{ name: "KEY_CUT", id: 0x7B }, // Cut (Ubuntu)
	{ name: "KEY_COPY", id: 0x7C }, // Copy (Ubuntu)
	{ name: "KEY_PASTE", id: 0x7D }, // Paste (Ubuntu)
	{ name: "KEY_FIND", id: 0x7E }, // Find (Ubuntu)
	{ name: "KEY_MUTE", id: 0x7F },
	{ name: "KEY_VOLUME_MUTE", id: 0x7F }, // Alias
	{ name: "KEY_VOLUME_UP", id: 0x80 },
	{ name: "KEY_VOLUME_DOWN", id: 0x81 },

	{ name: "KEY_LEFT_CTRL", id: 0xE0 },
	{ name: "KEY_LEFT_SHIFT", id: 0xE1 },
	{ name: "KEY_LEFT_ALT", id: 0xE2 },
	{ name: "KEY_LEFT_WINDOWS", id: 0xE3 },
	{ name: "KEY_RIGHT_CTRL", id: 0xE4 },
	{ name: "KEY_RIGHT_SHIFT", id: 0xE5 },
	{ name: "KEY_RIGHT_ALT", id: 0xE6 },
	{ name: "KEY_RIGHT_GUI", id: 0xE7 },
	{ name: "KEY_RIGHT_WINDOWS", id: 0xE7 },
];

export default HidKeyboard;