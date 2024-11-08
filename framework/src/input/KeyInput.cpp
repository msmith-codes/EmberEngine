#include <Ember/input/KeyInput.hpp>
#include <Ember/input/KeyListener.hpp>

// Note:
// Values are based on the GLFW key codes.
// https://www.glfw.org/docs/3.3/group__keys.html
namespace Ember
{
    // Key Input Values
    unsigned int KeyInput::KEY_SPACE = 32;
    unsigned int KeyInput::KEY_APOSTROPHE = 39;
    unsigned int KeyInput::KEY_COMMA = 44;
    unsigned int KeyInput::KEY_MINUS = 45;
    unsigned int KeyInput::KEY_PERIOD = 46;
    unsigned int KeyInput::KEY_SLASH = 47;
    unsigned int KeyInput::KEY_0 = 48;
    unsigned int KeyInput::KEY_1 = 49;
    unsigned int KeyInput::KEY_2 = 50;
    unsigned int KeyInput::KEY_3 = 51;
    unsigned int KeyInput::KEY_4 = 52;
    unsigned int KeyInput::KEY_5 = 53;
    unsigned int KeyInput::KEY_6 = 54;
    unsigned int KeyInput::KEY_7 = 55;
    unsigned int KeyInput::KEY_8 = 56;
    unsigned int KeyInput::KEY_9 = 57;
    unsigned int KeyInput::KEY_SEMICOLON = 59;
    unsigned int KeyInput::KEY_EQUAL = 61;
    unsigned int KeyInput::KEY_A = 65;
    unsigned int KeyInput::KEY_B = 66;
    unsigned int KeyInput::KEY_C = 67;
    unsigned int KeyInput::KEY_D = 68;
    unsigned int KeyInput::KEY_E = 69;
    unsigned int KeyInput::KEY_F = 70;
    unsigned int KeyInput::KEY_G = 71;
    unsigned int KeyInput::KEY_H = 72;
    unsigned int KeyInput::KEY_I = 73;
    unsigned int KeyInput::KEY_J = 74;
    unsigned int KeyInput::KEY_K = 75;
    unsigned int KeyInput::KEY_L = 76;
    unsigned int KeyInput::KEY_M = 77;
    unsigned int KeyInput::KEY_N = 78;
    unsigned int KeyInput::KEY_O = 79;
    unsigned int KeyInput::KEY_P = 80;
    unsigned int KeyInput::KEY_Q = 81;
    unsigned int KeyInput::KEY_R = 82;
    unsigned int KeyInput::KEY_S = 83;
    unsigned int KeyInput::KEY_T = 84;
    unsigned int KeyInput::KEY_U = 85;
    unsigned int KeyInput::KEY_V = 86;
    unsigned int KeyInput::KEY_W = 87;
    unsigned int KeyInput::KEY_X = 88;
    unsigned int KeyInput::KEY_Y = 89;
    unsigned int KeyInput::KEY_Z = 90;
    unsigned int KeyInput::KEY_LEFT_BRACKET = 91;
    unsigned int KeyInput::KEY_BACKSLASH = 92;
    unsigned int KeyInput::KEY_RIGHT_BRACKET = 93;
    unsigned int KeyInput::KEY_GRAVE_ACCENT = 96;
    unsigned int KeyInput::KEY_WORLD_1 = 161;
    unsigned int KeyInput::KEY_WORLD_2 = 162;
    unsigned int KeyInput::KEY_ESCAPE = 256;
    unsigned int KeyInput::KEY_ENTER = 257;
    unsigned int KeyInput::KEY_TAB = 258;
    unsigned int KeyInput::KEY_BACKSPACE = 259;
    unsigned int KeyInput::KEY_INSERT = 260;
    unsigned int KeyInput::KEY_DELETE = 261;
    unsigned int KeyInput::KEY_RIGHT = 262;
    unsigned int KeyInput::KEY_LEFT = 263;
    unsigned int KeyInput::KEY_DOWN = 264;
    unsigned int KeyInput::KEY_UP = 265;
    unsigned int KeyInput::KEY_PAGE_UP = 266;
    unsigned int KeyInput::KEY_PAGE_DOWN = 267;
    unsigned int KeyInput::KEY_HOME = 268;
    unsigned int KeyInput::KEY_END = 269;
    unsigned int KeyInput::KEY_CAPS_LOCK = 280;
    unsigned int KeyInput::KEY_SCROLL_LOCK = 281;
    unsigned int KeyInput::KEY_NUM_LOCK = 282;
    unsigned int KeyInput::KEY_PRINT_SCREEN = 283;
    unsigned int KeyInput::KEY_PAUSE = 284;
    unsigned int KeyInput::KEY_F1 = 290;
    unsigned int KeyInput::KEY_F2 = 291;
    unsigned int KeyInput::KEY_F3 = 292;
    unsigned int KeyInput::KEY_F4 = 293;
    unsigned int KeyInput::KEY_F5 = 294;
    unsigned int KeyInput::KEY_F6 = 295;
    unsigned int KeyInput::KEY_F7 = 296;
    unsigned int KeyInput::KEY_F8 = 297;
    unsigned int KeyInput::KEY_F9 = 298;
    unsigned int KeyInput::KEY_F10 = 299;
    unsigned int KeyInput::KEY_F11 = 300;
    unsigned int KeyInput::KEY_F12 = 301;
    unsigned int KeyInput::KEY_F13 = 302;
    unsigned int KeyInput::KEY_F14 = 303;
    unsigned int KeyInput::KEY_F15 = 304;
    unsigned int KeyInput::KEY_F16 = 305;
    unsigned int KeyInput::KEY_F17 = 306;
    unsigned int KeyInput::KEY_F18 = 307;
    unsigned int KeyInput::KEY_F19 = 308;
    unsigned int KeyInput::KEY_F20 = 309;
    unsigned int KeyInput::KEY_F21 = 310;
    unsigned int KeyInput::KEY_F22 = 311;
    unsigned int KeyInput::KEY_F23 = 312;
    unsigned int KeyInput::KEY_F24 = 313;
    unsigned int KeyInput::KEY_F25 = 314;
    unsigned int KeyInput::KEY_KP_0 = 320;
    unsigned int KeyInput::KEY_KP_1 = 321;
    unsigned int KeyInput::KEY_KP_2 = 322;
    unsigned int KeyInput::KEY_KP_3 = 323;
    unsigned int KeyInput::KEY_KP_4 = 324;
    unsigned int KeyInput::KEY_KP_5 = 325;
    unsigned int KeyInput::KEY_KP_6 = 326;
    unsigned int KeyInput::KEY_KP_7 = 327;
    unsigned int KeyInput::KEY_KP_8 = 328;
    unsigned int KeyInput::KEY_KP_9 = 329;
    unsigned int KeyInput::KEY_KP_DECIMAL = 330;
    unsigned int KeyInput::KEY_KP_DIVIDE = 331;
    unsigned int KeyInput::KEY_KP_MULTIPLY = 332;
    unsigned int KeyInput::KEY_KP_SUBTRACT = 333;
    unsigned int KeyInput::KEY_KP_ADD = 334;
    unsigned int KeyInput::KEY_KP_ENTER = 335;
    unsigned int KeyInput::KEY_KP_EQUAL = 336;
    unsigned int KeyInput::KEY_LEFT_SHIFT = 340;
    unsigned int KeyInput::KEY_LEFT_CONTROL = 341;
    unsigned int KeyInput::KEY_LEFT_ALT = 342;
    unsigned int KeyInput::KEY_LEFT_SUPER = 343;
    unsigned int KeyInput::KEY_RIGHT_SHIFT = 344;
    unsigned int KeyInput::KEY_RIGHT_CONTROL = 345;
    unsigned int KeyInput::KEY_RIGHT_ALT = 346;
    unsigned int KeyInput::KEY_RIGHT_SUPER = 347;
    unsigned int KeyInput::KEY_MENU = 348;

    // Methods
    //-------------------------------------------------------------------------
    bool KeyInput::isKeyPressed(int key)
    {
        return KeyListener::getInstance().getKeys()[key];
    }

    bool KeyInput::isKeyJustPressed(int key)
    {
        static bool lastState = false;
        bool currentState = KeyListener::getInstance().getKeys()[key];

        if(currentState && !lastState) {
            lastState = currentState;
            return true;
        } else {
            lastState = currentState;
            return false;
        }
    }
}