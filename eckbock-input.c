//#include "..\main.h"
#include <windows.h>
#include <xinput.h>
#include "eckbock-input.h"
#pragma comment(lib, "XInput.lib")

#define MAXCONTROLLERS 4

static char xActive[MAXCONTROLLERS];
static XINPUT_STATE xStates[MAXCONTROLLERS];

void Xinput_Update()
{
	DWORD dwResult;    
	for(DWORD i = 0; i < MAXCONTROLLERS; i++)
	{
		ZeroMemory(&xStates[i], sizeof(XINPUT_STATE));

		//Simply get the state of the controller from XInput.
		dwResult = XInputGetState(i, &xStates[i]);

        if(xStates[i].Gamepad.wButtons & XINPUT_GAMEPAD_A)
            i = i;

		if(dwResult == ERROR_SUCCESS)
			xActive[i] = 1;
		else
			xActive[i] = 0;
	}
}

void Xinput_Init()
{
	memset(xActive, 0, sizeof(xActive));
}

static char IsAnalogButton(int button)
{
    if(button == XINPUT_GAMEPAD_LT || button == XINPUT_GAMEPAD_RT || button == XINPUT_GAMEPAD_LEFTSTICK_UP || button == XINPUT_GAMEPAD_LEFTSTICK_DOWN || button == XINPUT_GAMEPAD_LEFTSTICK_RIGHT || button == XINPUT_GAMEPAD_LEFTSTICK_LEFT
        || button == XINPUT_GAMEPAD_RIGHTSTICK_UP || button == XINPUT_GAMEPAD_RIGHTSTICK_DOWN || button == XINPUT_GAMEPAD_RIGHTSTICK_RIGHT || button == XINPUT_GAMEPAD_RIGHTSTICK_LEFT)
        return 1;
    return 0;
}

static char AnalogButtonCheck(int controllerIdx, int button)
{
    if(button == XINPUT_GAMEPAD_LT) return xStates[controllerIdx].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    else if(button == XINPUT_GAMEPAD_RT) return xStates[controllerIdx].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    else if(button == XINPUT_GAMEPAD_LEFTSTICK_UP) return xStates[controllerIdx].Gamepad.sThumbLY > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
    else if(button == XINPUT_GAMEPAD_LEFTSTICK_DOWN) return xStates[controllerIdx].Gamepad.sThumbLY < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
    else if(button == XINPUT_GAMEPAD_LEFTSTICK_RIGHT) return xStates[controllerIdx].Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
    else if(button == XINPUT_GAMEPAD_LEFTSTICK_LEFT) return xStates[controllerIdx].Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
    else if(button == XINPUT_GAMEPAD_RIGHTSTICK_UP) return xStates[controllerIdx].Gamepad.sThumbRY > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
    else if(button == XINPUT_GAMEPAD_RIGHTSTICK_DOWN) return xStates[controllerIdx].Gamepad.sThumbRY < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
    else if(button == XINPUT_GAMEPAD_RIGHTSTICK_RIGHT) return xStates[controllerIdx].Gamepad.sThumbRX > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
    else if(button == XINPUT_GAMEPAD_RIGHTSTICK_LEFT) return xStates[controllerIdx].Gamepad.sThumbRX < -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
    return 0;
}

char Xinput_InputCheck(int button, int controllerIdx)
{
	if(controllerIdx == -1) //Check any controller
	{
		char b = 0;
		for(int i = 0; i < MAXCONTROLLERS; i++)
		{
			if(!xActive[i])
				continue;

			if( (IsAnalogButton(button) && AnalogButtonCheck(i, button))
                || (!IsAnalogButton(button) && xStates[i].Gamepad.wButtons & button) )
			{
				b = 1;
				break;
			}
		}
		return b;
	}

	if(controllerIdx >= MAXCONTROLLERS || controllerIdx < -1 || !xActive[controllerIdx])
		return 0;
    if(IsAnalogButton(button))
        return AnalogButtonCheck(controllerIdx, button);
    else
	    return (xStates[controllerIdx].Gamepad.wButtons & button) != 0;
}

//Simple method for getting the thumbstick movement of any xinput controller. We should probably change how this works in the long run
//Right now we're doing the deadzone check in a bad way (we're checking within a square, but it should be within a circle). More info here: https://docs.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput
void Xinput_GetThumbPos(char rightStick, float *x, float *y, char applyDeadZone)
{
    /*
	for(int i = 0; i < MAXCONTROLLERS; i++)
	{
		if(!xActive[i])
			continue;
		short stickX, stickY;
		if(rightStick)
			stickX = xStates[i].Gamepad.sThumbRX, stickY = xStates[i].Gamepad.sThumbRY;
		else
			stickX = xStates[i].Gamepad.sThumbLX, stickY = xStates[i].Gamepad.sThumbLY;

		if(stickX == 0 && stickY == 0) //This stick has zero movement, so move onto another gamepad
			continue;

		if(applyDeadZone)
		{
			short minValue;
			if(rightStick)
				minValue = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
			else
				minValue = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

			if(abs(stickX) - minValue <= 0 && abs(stickY) - minValue <= 0)
				stickX = 0, stickY = 0;
		}

		*x = (float) stickX / 32768;
		*y = (float) stickY / 32768;
		break;
	}
    */
}
