/*
 * This file is an example implementation of the menu system.
 */
#include "script.h"
#include <NativeWheel.hpp>

// You can ignore these includes as they're only used to resolve
// paths, log things to a file or to provide nice wrappers for natives.
#include "Util/Paths.h"
#include "Util/Util.hpp"
#include "Util/Logger.hpp"
#include "Util/Controls.hpp"
#include "Memory/NativeMemory.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

//std::string settingsFile;
std::string wheelsFile;

Player player;
Ped playerPed;
bool gameVersionAllowed;

Control cNextWheel;
Control cPrevWheel;
bool event_JUST_OPENED_OnNextOpen = true;
CControlSys__GetActionName GetInputActionName;

std::vector<NativeWheel> wheelList;
int currentWheelIndex;
std::vector<string> validStationNames;

void unhide_all_stations() {
	if (!gameVersionAllowed) return;
	for (string s : validStationNames) {
		AUDIO::_LOCK_RADIO_STATION(s.c_str(), false);
	}
}

void init() {

	if ((int)getGameVersion() < 42) { // G_VER_1_0_1493_0_STEAM
		logger.Write(ERRORR, "Can't use the native radio wheel organizer on this game version. Please update to 1.0.1493.0 or higher.");
		gameVersionAllowed = false;
		return;
	}

	gameVersionAllowed = true;

	DWORD64 addr = Memory::FindPattern("\x3B\x0D\x00\x00\x00\x00\x73\x0E\x48\x8B\x05\x00\x00\x00\x00\x8B\xC9",
		"xx????xxxxx????xx");
	//DWORD64 *radioStationList = *(DWORD64 **)(addr + *(int *)(addr + 11) + 15);
	int count = *(int *)(addr + *(int *)(addr + 2) + 6);

	for (int i = 0; i < count; i++) {

		const char *name = AUDIO::GET_RADIO_STATION_NAME(i);
		const char *pname = UI::_GET_LABEL_TEXT(name);
		logger.Write(INFO, "%d || Name: %s || Proper Name: %s", i, name, pname);
		if (pname != NULL) {
			validStationNames.push_back(name);
		}
	}

	logger.Write(INFO, "Please use the 'Name' name for your wheel organization lists (wheels.cfg)! 'Proper name' is only for display purposes.");
	logger.Write(INFO, "Stations with a NULL proper name cannot be used in the radio wheel. Please don't add them to wheels.cfg");
	logger.Write(INFO, "You can edit and reload wheels.cfg in-game by pressing `~` and typing `radio_reload` (without quotes).");

	unhide_all_stations();
	/*int trackId = AUDIO::GET_AUDIBLE_MUSIC_TRACK_TEXT_ID();
	const char* songName = UI::_GET_LABEL_TEXT(fmt::sprintf("%dS", trackId).c_str());*/
}

bool wheel_list_is_populated() {
	return wheelList.size() > 0;
}

void read_cfg() {
	if (!gameVersionAllowed) return;

	wheelsFile = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\wheels.cfg";

	std::ifstream file(wheelsFile);

	if (!file) {
		logger.Write(ERRORR, "wheels.cfg not found!");
	}

	bool lastLineWasWheelName = false;

	std::string line;
	while (std::getline(file, line)) {
		string s = trim(line);

		if (s == "") {
			continue;
		}

		// Contains [ and ]
		if (s.find('[') != std::string::npos
			&& s.find(']') != std::string::npos) {
			if (lastLineWasWheelName) {
				wheelList.pop_back();	// Remove last element in list
				logger.Write(INFO, "No stations found in this wheel. Skipping...");
			}

			std::string name = s.substr(1, s.length() - 2);
			wheelList.push_back(NativeWheel(name));	// Add new wheel to list
			logger.Write(INFO, "Found wheel '" + name + "'");
			lastLineWasWheelName = true;
			continue;
		}

		if (wheel_list_is_populated()) {
			bool isValidName = (std::find(validStationNames.begin(), validStationNames.end(), s) != validStationNames.end());

			if (isValidName) {
				wheelList.back().stationList.push_back(s);
				lastLineWasWheelName = false;
				logger.Write(INFO, "Found station '" + s + "'");
			}
		}
	}

	if (wheel_list_is_populated()) {
		currentWheelIndex = 0;
		GetInputActionName = GetInstructionalButtonNameFunction();
	}
	else {
		gameVersionAllowed = false;
		logger.Write(ERRORR, "There are no wheels in wheels.cfg. Aborting script...");
	}
}

void show_help_texts() {
	cNextWheel = CurrentInputMode() == InputMode::GamePad ? Control::VehicleAccelerate : Control::WeaponWheelPrev;
	cPrevWheel = CurrentInputMode() == InputMode::GamePad ? Control::VehicleBrake : Control::WeaponWheelNext;

	std::string cNextText((*GetInputActionName)(static_cast<int>(cNextWheel)));
	std::string cPrevText((*GetInputActionName)(static_cast<int>(cPrevWheel)));
	std::string text = "~" + cNextText + "~ ~" + cPrevText + "~";
	text += " : Next / Prev Wheel\n";
	text += "Wheel: ";
	text += wheelList[currentWheelIndex].Name;

	showHelpText(text, false, false);
}

void disable_native_radio_scroll() {
	DisableControlThisFrame(2, Control::VehicleNextRadio);
	DisableControlThisFrame(2, Control::VehiclePrevRadio);
}

void update_wheel_this_frame() {
	// Unlock all stations in current wheel
	for (auto station : wheelList[currentWheelIndex].stationList) {
		AUDIO::_LOCK_RADIO_STATION(station.c_str(), false);
	}

	for (auto st : validStationNames) {
		// Lock/hide any stations not included in the current wheel.

		bool existsInCurrentWheel = (std::find(wheelList[currentWheelIndex].stationList.begin(),
			wheelList[currentWheelIndex].stationList.end(), st) != wheelList[currentWheelIndex].stationList.end());

		if (!existsInCurrentWheel) {
			AUDIO::_LOCK_RADIO_STATION(st.c_str(), true);
		}
	}
}

void control_wheel_change() {
	if (IsControlJustPressed(2, cNextWheel)) {
		DisableControlThisFrame(2, Control::VehicleRadioWheel);
		currentWheelIndex = currentWheelIndex == wheelList.size() - 1 ? 0 : currentWheelIndex + 1;
		update_wheel_this_frame();
	}
	else if (IsControlJustPressed(2, cPrevWheel)) {
		DisableControlThisFrame(2, Control::VehicleRadioWheel);
		currentWheelIndex = currentWheelIndex == 0 ? wheelList.size() - 1 : currentWheelIndex - 1;
		update_wheel_this_frame();
	}
}

void on_just_opened() {
	update_wheel_this_frame();
}

void update_game() {
	player = PLAYER::PLAYER_ID();
	playerPed = PLAYER::PLAYER_PED_ID();

	if (MISC::_HAS_CHEAT_STRING_JUST_BEEN_ENTERED(MISC::GET_HASH_KEY("radio_reload"))) {
		unhide_all_stations();
		wheelList.clear();
		currentWheelIndex = 0;
		read_cfg();
		showSubtitle("Reloaded wheels.cfg.");
		WAIT(150);
	}

	if (UI::IS_HUD_COMPONENT_ACTIVE(16)) {	// IsRadioHudComponentVisible
		show_help_texts();

		control_wheel_change();

		if (event_JUST_OPENED_OnNextOpen) {
			on_just_opened();
		}

		disable_native_radio_scroll();

		event_JUST_OPENED_OnNextOpen = false;
	}
	else {
		if (!event_JUST_OPENED_OnNextOpen) {
			event_JUST_OPENED_OnNextOpen = true;
		}
	}
}

void main() {
	logger.Write(INFO, "Script started");

	// Check the paths on runtime, though this could also be hardcoded with a relative path.
	//settingsFile = Paths::GetModuleFolder(Paths::GetOurModuleHandle()) + modDir + "\\settings_menu.ini";
	//logger.Write(INFO, "Loading " + settingsFile);

	init();
	read_cfg();

	while (true && gameVersionAllowed) {
		update_game();
		WAIT(0);
	}
}

void ScriptMain() {
	srand(GetTickCount());
	main();
}
