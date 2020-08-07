#pragma once

#include <string>

#include "..\..\ScriptHookV_SDK\inc\natives.h"
#include "..\..\ScriptHookV_SDK\inc\types.h"
#include "..\..\ScriptHookV_SDK\inc\enums.h"
#include "..\..\ScriptHookV_SDK\inc\main.h"

#define DISPLAY_VERSION "v1.0.0"

const std::string modDir  = "\\MoreRadioWheels";

void ScriptMain();

void unhide_all_stations();
