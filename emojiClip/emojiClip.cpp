#include <iostream>
#include <windows.h>
#include "Keypress.h"

void toClipboard(std::string& s, HWND hwnd = GetDesktopWindow());

int main()
{
	const int pressed = -32767;
	std::string emoji = ":regional_indicator_";

	Keypress<int> backspace = Keypress<int>(VK_BACK, false);
	Keypress<int> ctrl = Keypress<int>(VK_CONTROL, false);
	Keypress<char> keyV = Keypress<char>('V', false);
	Keypress<char> space = Keypress<char>(' ', false);

	while(true)
	{
		for(int i = 65; i <= 90; ++i)
		{
			if(GetAsyncKeyState(i) == pressed) {
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

				std::cout << emoji << std::endl;
				Sleep(10);
			}
		}
	}
}

void toClipboard(std::string& s, HWND hwnd) 
{
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size() + 1);
	if(!hg) {
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size() + 1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}