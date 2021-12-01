#include <iostream>
#include <windows.h>
#include "Keypress.h"

inline void clearBackspace(int n)
{
	if(n != 1) 
		return;
	for (auto i = 0; i < 6; i++)
		std::cout << "\b \b";
}

void toClipboard(std::string& s, HWND hwnd = GetDesktopWindow())
{
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if (!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

int main()
{
	std::system("@title Emoji Typing for Discord by Factor & @mode 75, 25");

	constexpr int pressed = -32767;
	auto deletes = 0;
	std::string emoji = ":regional_indicator_";

	Keypress<int> backspace = Keypress<int>(VK_BACK, false);
	Keypress<int> ctrl = Keypress<int>(VK_CONTROL, false);
	Keypress<char> keyV = Keypress<char>('V', false);
	Keypress<char> space = Keypress<char>(' ', false);

	while(true)
	{
		if(GetAsyncKeyState(VK_BACK) == pressed) {
			// takes 2 backspace presses to delete the space between emojis and the space in discord
			if (++deletes == 2) {
				deletes = 0;
				clearBackspace(1);
				// delete last character
				std::cout << "\b \b";
			} else if(deletes == 1) {
				std::cout << " [\\b] ";
			}
			Sleep(100);
		} else if(GetAsyncKeyState(VK_SPACE) == pressed) {
			clearBackspace(deletes);
			deletes = 0;

			std::cout << " ";
			Sleep(100);
		} else if(GetAsyncKeyState(VK_RETURN) == pressed) {
			clearBackspace(deletes);
			deletes = 0;

			std::cout << "\n";
			Sleep(100);
		}

		for(auto i = 65; i <= 90; ++i)
		{
			if(GetAsyncKeyState(i) == pressed) {
				// delete the actual character when pressed
				backspace.keystroke();

				char curChar = i + 32;

				emoji = ":regional_indicator_";
				emoji.push_back(curChar);
				emoji.push_back(':');
				toClipboard(emoji);

				ctrl.press();
				keyV.keystroke();
				ctrl.release();
				space.keystroke();

				clearBackspace(deletes);
				deletes = 0;

				std::cout << curChar;
			}
		}
	}
}
