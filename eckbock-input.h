#pragma once

//Additions to xinput button bitflags
/*
#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000
*/
#define XINPUT_GAMEPAD_LT               0x0400
#define XINPUT_GAMEPAD_RT               0x0800
#define XINPUT_GAMEPAD_LEFTSTICK_UP     0x10000
#define XINPUT_GAMEPAD_LEFTSTICK_DOWN   0x20000
#define XINPUT_GAMEPAD_LEFTSTICK_LEFT   0x40000
#define XINPUT_GAMEPAD_LEFTSTICK_RIGHT  0x80000
#define XINPUT_GAMEPAD_RIGHTSTICK_UP    0x100000
#define XINPUT_GAMEPAD_RIGHTSTICK_DOWN  0x200000
#define XINPUT_GAMEPAD_RIGHTSTICK_LEFT  0x400000
#define XINPUT_GAMEPAD_RIGHTSTICK_RIGHT 0x800000

void Xinput_Update();
void Xinput_Init();
char Xinput_InputCheck(int button, int controllerIdx);
void Xinput_GetThumbPos(char rightStick, float *x, float *y, char applyDeadZone);
