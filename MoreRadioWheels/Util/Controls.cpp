#define NOMINMAX
#include "../../../ScriptHookV_SDK/inc/natives.h"
#include "Controls.hpp"
#include "Memory/NativeMemory.hpp"
#include <string>

bool IsControlPressed(int index, Control control)
{
	return PAD::IS_DISABLED_CONTROL_PRESSED(index, static_cast<int>(control));
}

bool IsControlJustPressed(int index, Control control)
{
	return PAD::IS_DISABLED_CONTROL_JUST_PRESSED(index, static_cast<int>(control));
}

bool IsControlJustReleased(int index, Control control)
{
	return PAD::IS_DISABLED_CONTROL_JUST_RELEASED(index, static_cast<int>(control));
}

bool IsEnabledControlPressed(int index, Control control)
{
	return PAD::IS_CONTROL_PRESSED(index, static_cast<int>(control));
}

bool IsEnabledControlJustPressed(int index, Control control)
{
	return PAD::IS_CONTROL_JUST_PRESSED(index, static_cast<int>(control));
}

bool IsEnabledControlJustReleased(int index, Control control)
{
	return PAD::IS_CONTROL_JUST_RELEASED(index, static_cast<int>(control));
}

bool IsDisabledControlPressed(int index, Control control)
{
	return IsControlPressed(index, control) && !IsControlEnabled(index, control);
}

bool IsDisabledControlJustPressed(int index, Control control)
{
	return IsControlJustPressed(index, control) && !IsControlEnabled(index, control);
}

bool IsDisabledControlJustReleased(int index, Control control)
{
	return IsControlJustReleased(index, control) && !IsControlEnabled(index, control);
}

bool IsControlEnabled(int index, Control control)
{
	return PAD::IS_CONTROL_ENABLED(index, static_cast<int>(control));
}

void EnableControlThisFrame(int index, Control control)
{
	PAD::ENABLE_CONTROL_ACTION(index, static_cast<int>(control), true);
}

void DisableControlThisFrame(int index, Control control)
{
	PAD::DISABLE_CONTROL_ACTION(index, static_cast<int>(control), true);
}

void DisableAllControlsThisFrame(int index)
{
	PAD::DISABLE_ALL_CONTROL_ACTIONS(index);
}

void EnableAllControlsThisFrame(int index)
{
	PAD::ENABLE_ALL_CONTROL_ACTIONS(index);
}

float GetControlNormal(int index, Control control)
{
	return PAD::GET_CONTROL_NORMAL(index, static_cast<int>(control));
}

float GetDisabledControlNormal(int index, Control control)
{
	return PAD::GET_DISABLED_CONTROL_NORMAL(index, static_cast<int>(control));
}

int GetControlValue(int index, Control control)
{
	return PAD::GET_CONTROL_VALUE(index, static_cast<int>(control));
}

std::string GetInstructionalButtonScaleform(int p0, Control control, bool p2) {
	std::string s(PAD::GET_CONTROL_INSTRUCTIONAL_BUTTON(p0, static_cast<int>(control), p2));
	return s;
}

CControlSys__GetActionName GetInstructionalButtonNameFunction() {
	const char*(*CControlSys__GetActionName)(uint32_t actionId);

	auto addr = Memory::FindPattern("\xE8\x00\x00\x00\x00\x44\x0F\xB7\x57\x00\x33\xC9",
		"x????xxxx?xx");

	CControlSys__GetActionName = (decltype(CControlSys__GetActionName))(addr + *(int*)(addr + 1) + 5);
	return CControlSys__GetActionName;
}

InputMode CurrentInputMode()
{
	return PAD::_IS_INPUT_DISABLED(2) ? InputMode::MouseAndKeyboard : InputMode::GamePad;
}