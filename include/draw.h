#pragma once
#include <Windows.h>
#include <string>

void draw_axis(float length);
void draw_text(const HDC& hdc, const std::wstring& text);
