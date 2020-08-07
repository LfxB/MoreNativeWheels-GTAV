#pragma once

#include <string>
#include <vector>

struct Color {
	int R;
	int G;
	int B;
	int A;
};

const Color solidWhite = {
	255, 255, 255, 255
};

// Natives called
void showText(float x, float y, float scale, const char* text, int font = 0, const Color &rgba = solidWhite, bool outline = false);
void showHelpText(std::string text, bool forever = false, bool beep = true, int shape = -1);
void showNotification(const char* message, int *prevNotification = nullptr);
void showSubtitle(std::string message, int duration = 2500);
void addLongString(std::string str);
std::string prettyNameFromHash(Hash hash);
std::string trim(const std::string& str);

//http://stackoverflow.com/questions/36789380/how-to-store-a-const-char-to-a-char
class CharAdapter
{
public:
	explicit CharAdapter(const char* s) : m_s(::_strdup(s)) { }
	CharAdapter(const CharAdapter& other) = delete; // non construction-copyable
	CharAdapter& operator=(const CharAdapter&) = delete; // non copyable
	
	~CharAdapter() /*free memory on destruction*/
	{
		::free(m_s); /*use free to release strdup memory*/
	}
	operator char*() /*implicit cast to char* */
	{
		return m_s;
	}

private:
	char* m_s;
};

bool GetIMGDimensions(std::string file, int *width, int *height);

bool GetPNGDimensions(std::string file, int *width, int *height);
bool GetJPGDimensions(std::string file, int *width, int *height);


inline bool exists(const std::string& name) {
	struct stat buffer;
	return stat(name.c_str(), &buffer) == 0;
}
