#define NOMINMAX
#include "../../../ScriptHookV_SDK/inc/natives.h"
#include "Util.hpp"
#include <algorithm>
#include <fstream>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

void showText(float x, float y, float scale, const char* text, int font, const Color &rgba, bool outline) {
	UI::SET_TEXT_FONT(font);
	UI::SET_TEXT_SCALE(scale, scale);
	UI::SET_TEXT_COLOUR(rgba.R, rgba.G, rgba.B, rgba.A);
	UI::SET_TEXT_WRAP(0.0, 1.0);
	UI::SET_TEXT_CENTRE(0);
	if (outline) UI::SET_TEXT_OUTLINE();
	UI::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
	//UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(CharAdapter(text));
	addLongString(text);
	UI::END_TEXT_COMMAND_DISPLAY_TEXT(x, y, 0);
}

void showHelpText(std::string text, bool forever, bool beep, int shape) {
	UI::BEGIN_TEXT_COMMAND_DISPLAY_HELP("jamyfafi"); // CELL_EMAIL_BCON
	addLongString(text);
	UI::END_TEXT_COMMAND_DISPLAY_HELP(0, forever, beep, shape);
}

void showNotification(const char* message, int *prevNotification) {
	if (prevNotification != nullptr && *prevNotification != 0) {
		UI::_REMOVE_NOTIFICATION(*prevNotification);
	}
	UI::_SET_NOTIFICATION_TEXT_ENTRY("STRING");

	UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(CharAdapter(message));
	
	int id = UI::_DRAW_NOTIFICATION(false, false);
	if (prevNotification != nullptr) {
		*prevNotification = id;
	}
}

// borrowed from FiveM <3
void showSubtitle(std::string message, int duration) {
	UI::BEGIN_TEXT_COMMAND_PRINT("CELL_EMAIL_BCON");

	const int maxStringLength = 99;

	for (int i = 0; i < message.size(); i += maxStringLength) {
		int npos = std::min(maxStringLength, static_cast<int>(message.size()) - i);
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(CharAdapter(message.substr(i, npos).c_str()));
	}

	UI::END_TEXT_COMMAND_PRINT(duration, 1);
}

void addLongString(std::string str) {
	const int strLen = 99;
	for (int i = 0; i < str.length(); i += strLen)
	{
		const int nLen = str.length() - i;
		std::string substr = str.substr(i, std::min(strLen, nLen));
		UI::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(CharAdapter(substr.c_str()));
	}
}

std::string prettyNameFromHash(Hash hash) {
	const char *name = VEHICLE::GET_DISPLAY_NAME_FROM_VEHICLE_MODEL(hash);
	std::string displayName = UI::_GET_LABEL_TEXT(name);
	if (displayName == "NULL") {
		displayName = name;
	}
	return displayName;
}

std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (std::string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

// https://codereview.stackexchange.com/questions/149717/implementation-of-c-standard-library-function-ntohl
uint32_t my_ntohl(uint32_t const net) {
	uint8_t data[4] = {};
	memcpy(&data, &net, sizeof(data));

	return ((uint32_t)data[3] << 0)
		| ((uint32_t)data[2] << 8)
		| ((uint32_t)data[1] << 16)
		| ((uint32_t)data[0] << 24);
}

bool GetIMGDimensions(std::string file, int *width, int *height) {
	auto ext = fs::path(file).extension();
	if (ext == ".png" || ext == ".PNG")
		return GetPNGDimensions(file, width, height);
	if (ext == ".jpg" || ext == ".JPG" || ext == ".jpeg" || ext == ".JPEG")
		return GetJPGDimensions(file, width, height);
	return false;
}

bool GetPNGDimensions(std::string file, int *width, int *height) {
	std::ifstream in(file);
	int _width, _height;

	in.seekg(16);
	in.read((char *)&_width, 4);
	in.read((char *)&_height, 4);

	*width = my_ntohl(_width);
	*height = my_ntohl(_height);

	return true;
}

//https://stackoverflow.com/questions/17847171/c-library-for-getting-the-jpeg-image-size
bool GetJPGDimensions(std::string file, int *width, int *height) {
	FILE *image = nullptr;

	errno_t err = fopen_s(&image, file.c_str(), "rb");  // open JPEG image file
	if (!image || err) {
		return false;
	}
	fseek(image, 0, SEEK_END);
	int size = ftell(image);
	fseek(image, 0, SEEK_SET);
	unsigned char *data = (unsigned char *)malloc(size);
	fread(data, 1, size, image);
	/* verify valid JPEG header */
	int i = 0;
	if (data[i] == 0xFF && data[i + 1] == 0xD8 && data[i + 2] == 0xFF && data[i + 3] == 0xE0) {
		i += 4;
		/* Check for null terminated JFIF */
		if (data[i + 2] == 'J' && data[i + 3] == 'F' && data[i + 4] == 'I' && data[i + 5] == 'F' && data[i + 6] == 0x00) {
			while (i < size) {
				i++;
				if (data[i] == 0xFF) {
					if (data[i + 1] == 0xC0) {
						*height = data[i + 5] * 256 + data[i + 6];
						*width = data[i + 7] * 256 + data[i + 8];
						break;
					}
				}
			}
		}
		else {
			fclose(image);
			return false;
		}
	}
	else {
		fclose(image);
		return false;
	}
	fclose(image);
	return true;
}
