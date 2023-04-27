#pragma once

#include "mksv/common/types.hpp"

namespace mksv
{
enum class Key : u8 {
    None = 0x00,     // No key was pressed
    LButton = 0x01,  // Left mouse button
    RButton = 0x02,  // Right mouse button
    Cancel = 0x03,   // Cancel key
    MButton = 0x04,  // Middle mouse button
    XButton1 = 0x05, // X1 mouse button
    XButton2 = 0x06, // X2 mouse button
    // 0x07 is undefined
    Back = 0x08,
    Tab = 0x09,
    // 0x0A-0B are reserved
    Clear = 0x0C, // The CLEAR key
    Enter = 0x0D, // The Enter key
    // 0x0E-0F are undefined
    ShiftKey = 0x10,    // The Shift key
    ControlKey = 0x11,  // The Ctrl key
    AltKey = 0x12,      // The Alt key
    Pause = 0x13,       // The Pause key
    Capital = 0x14,     // The Caps Lock key
    CapsLock = 0x14,    // The Caps Lock key
    KanaMode = 0x15,    // IMI Kana mode
    HanguelMode = 0x15, // IMI Hanguel mode (Use HangulMode)
    HangulMode = 0x15,  // IMI Hangul mode
    // 0x16 is undefined
    JunjaMode = 0x17, // IMI Janja mode
    FinalMode = 0x18, // IMI Final mode
    HanjaMode = 0x19, // IMI Hanja mode
    KanjiMode = 0x19, // IMI Kanji mode
    // 0x1A is undefined
    Escape = 0x1B,        // The ESC key
    IMEConvert = 0x1C,    // IMI convert key
    IMINoConvert = 0x1D,  // IMI noconvert key
    IMEAccept = 0x1E,     // IMI accept key
    IMIModeChange = 0x1F, // IMI mode change key
    Space = 0x20,         // The Space key
    Prior = 0x21,         // The Page Up key
    PageUp = 0x21,        // The Page Up key
    Next = 0x22,          // The Page Down key
    PageDown = 0x22,      // The Page Down key
    End = 0x23,           // The End key
    Home = 0x24,          // The Home key
    Left = 0x25,          // The Left arrow key
    Up = 0x26,            // The Up arrow key
    Right = 0x27,         // The Right arrow key
    Down = 0x28,          // The Down arrow key
    Select = 0x29,        // The Select key
    Print = 0x2A,         // The Print key
    ExECute = 0x2B,       // The ExECute key
    PrintScreen = 0x2C,   // The Print Screen key
    Snapshot = 0x2C,      // The Print Screen key
    Insert = 0x2D,        // The Insert key
    Delete = 0x2E,        // The Delete key
    Help = 0x2F,          // The Help key
    D0 = 0x30,            // 0
    D1 = 0x31,            // 1
    D2 = 0x32,            // 2
    D3 = 0x33,            // 3
    D4 = 0x34,            // 4
    D5 = 0x35,            // 5
    D6 = 0x36,            // 6
    D7 = 0x37,            // 7
    D8 = 0x38,            // 8
    D9 = 0x39,            // 9
    // 0x3A - 40 are undefined
    A = 0x41,    // A
    B = 0x42,    // B
    C = 0x43,    // C
    D = 0x44,    // D
    E = 0x45,    // E
    F = 0x46,    // F
    G = 0x47,    // G
    H = 0x48,    // H
    I = 0x49,    // I
    J = 0x4A,    // J
    K = 0x4B,    // K
    L = 0x4C,    // L
    M = 0x4D,    // M
    N = 0x4E,    // N
    O = 0x4F,    // O
    P = 0x50,    // P
    Q = 0x51,    // Q
    R = 0x52,    // R
    S = 0x53,    // S
    T = 0x54,    // T
    U = 0x55,    // U
    V = 0x56,    // V
    W = 0x57,    // W
    X = 0x58,    // X
    Y = 0x59,    // Y
    Z = 0x5A,    // Z
    LWin = 0x5B, // Left Windows key
    RWin = 0x5C, // Right Windows key
    Apps = 0x5D, // Apps key
    // 0x5E is reserved
    Sleep = 0x5F,     // The Sleep key
    NumPad0 = 0x60,   // The Numeric keypad 0 key
    NumPad1 = 0x61,   // The Numeric keypad 1 key
    NumPad2 = 0x62,   // The Numeric keypad 2 key
    NumPad3 = 0x63,   // The Numeric keypad 3 key
    NumPad4 = 0x64,   // The Numeric keypad 4 key
    NumPad5 = 0x65,   // The Numeric keypad 5 key
    NumPad6 = 0x66,   // The Numeric keypad 6 key
    NumPad7 = 0x67,   // The Numeric keypad 7 key
    NumPad8 = 0x68,   // The Numeric keypad 8 key
    NumPad9 = 0x69,   // The Numeric keypad 9 key
    Multiply = 0x6A,  // The Multiply key
    Add = 0x6B,       // The Add key
    Separator = 0x6C, // The Separator key
    Subtract = 0x6D,  // The Subtract key
    Decimal = 0x6E,   // The Decimal key
    Divide = 0x6F,    // The Divide key
    F1 = 0x70,        // The F1 key
    F2 = 0x71,        // The F2 key
    F3 = 0x72,        // The F3 key
    F4 = 0x73,        // The F4 key
    F5 = 0x74,        // The F5 key
    F6 = 0x75,        // The F6 key
    F7 = 0x76,        // The F7 key
    F8 = 0x77,        // The F8 key
    F9 = 0x78,        // The F9 key
    F10 = 0x79,       // The F10 key
    F11 = 0x7A,       // The F11 key
    F12 = 0x7B,       // The F12 key
    F13 = 0x7C,       // The F13 key
    F14 = 0x7D,       // The F14 key
    F15 = 0x7E,       // The F15 key
    F16 = 0x7F,       // The F16 key
    F17 = 0x80,       // The F17 key
    F18 = 0x81,       // The F18 key
    F19 = 0x82,       // The F19 key
    F20 = 0x83,       // The F20 key
    F21 = 0x84,       // The F21 key
    F22 = 0x85,       // The F22 key
    F23 = 0x86,       // The F23 key
    F24 = 0x87,       // The F24 key
    // 0x88 - 0x8F are unassigned
    NumLock = 0x90, // The Num Lock key
    Scroll = 0x91,  // The Scroll Lock key
    // 0x92 - 96 are OEM specific
    // 0x97 - 9F are unassigned
    LShiftKey = 0xA0,          // The Left Shift key
    RShiftKey = 0xA1,          // The Right Shift key
    LControlKey = 0xA2,        // The Left Control key
    RControlKey = 0xA3,        // The Right Control key
    LAltKey = 0xA4,            // The Left Alt key
    RAltKey = 0xA5,            // The Right Alt key
    BrowserBack = 0xA6,        // The Browser Back key
    BrowserForward = 0xA7,     // The Browser Forward key
    BrowserRefresh = 0xA8,     // The Browser Refresh key
    BrowserStop = 0xA9,        // The Browser Stop key
    BrowserSearch = 0xAA,      // The Browser Search key
    BrowserFavorites = 0xAB,   // The Browser Favorites key
    BrowserHome = 0xAC,        // The Browser Home key
    VolumeMute = 0xAD,         // The Volume Mute key
    VolumeDown = 0xAE,         // The Volume Down key
    VolumeUp = 0xAF,           // The Volume Up key
    MediaNextTrack = 0xB0,     // The Next Track key
    MediaPreviousTrack = 0xB1, // The Previous Track key
    MediaStop = 0xB2,          // The Stop Media key
    MediaPlayPause = 0xB3,     // The Play/Pause Media key
    LaunchMail = 0xB4,         // The Start Mail key
    SelectMedia = 0xB5,        // The Select Media key
    LaunchApplication1 = 0xB6, // The Launch Application 1 key.
    LaunchApplication2 = 0xB7, // The Launch Application 2 key.
    // 0xB8 - B9 are reserved
    OemSemicolon = 0xBA, // It can vary by keyboard. For the US standard keyboard, the ';:' key
    Oem1 = 0xBA,         // It can vary by keyboard. For the US standard keyboard, the ';:' key
    OemPlus = 0xBB,      // For any country/region, the '+' key
    OemComma = 0xBC,     // For any country/region, the ',' key
    OemMinus = 0xBD,     // For any country/region, the '-' key
    OemPeriod = 0xBE,    // For any country/region, the '.' key
    OemQuestion = 0xBF,  // It can vary by keyboard. For the US standard keyboard, the '/?' key
    Oem2 = 0xBF,         // It can vary by keyboard. For the US standard keyboard, the '/?' key
    OemTilde = 0xC0,     // It can vary by keyboard. For the US standard keyboard, the '`~' key
    Oem3 = 0xC0,         // It can vary by keyboard. For the US standard keyboard, the '`~' key
    // 0xC1 - D7 are reserved
    // 0xD8 - DA are unassigned
    OemOpenBrackets = 0xDB,  // It can vary by keyboard. For the US standard keyboard, the '[{' key
    Oem4 = 0xDB,             // It can vary by keyboard. For the US standard keyboard, the '[{' key
    OemPipe = 0xDC,          // It can vary by keyboard. For the US standard keyboard, the '\|' key
    Oem5 = 0xDC,             // It can vary by keyboard. For the US standard keyboard, the '\|' key
    OemCloseBrackets = 0xDD, // It can vary by keyboard. For the US standard keyboard, the ']}' key
    Oem6 = 0xDD,             // It can vary by keyboard. For the US standard keyboard, the ']}' key
    OemQuotes = 0xDE,        // It can vary by keyboard. For the US standard keyboard, the '" key
    Oem7 = 0xDE,             // It can vary by keyboard. For the US standard keyboard, the '" key
    Oem8 = 0xDF,             // It can vary by keyboard.
    // 0xE0 is reserved
    // 0xE1 is OEM specific
    OemBackslash = 0xE2, // Either the angle bracket key or the backslash key on the RT 102-key keyboard
    Oem102 = 0xE2,       // Either the angle bracket key or the backslash key on the RT 102-key keyboard
    // 0xE3 - E4 OEM specific
    ProcessKey = 0xE5, // IME Process key
    // 0xE6 is OEM specific
    Packet = 0xE7, // Used to pass Unicode characters as if they were keystrokes. The Packet key value is the low word
                   // of a 32-bit virtual-key value used for non-keyboard input methods.
    // 0xE8 is unassigned
    // 0xE9 - F5 OEM specific
    Attn = 0xF6,     // The Attn key
    CrSel = 0xF7,    // The CrSel key
    ExSel = 0xF8,    // The ExSel key
    EraseEof = 0xF9, // The Erase EOF key
    Play = 0xFA,     // The Play key
    Zoom = 0xFB,     // The Zoom key
    NoName = 0xFC,   // Reserved
    Pa1 = 0xFD,      // The PA1 key
    OemClear = 0xFE, // The Clear key
};
} // namespace mksv
