#pragma once
#include "uno/Card.h"

void gameLoop();
void ClearScreen();
void PrintBorder();
int GetCharInput();
void IncrementIndex(bool bUp, size_t nSize, size_t& nCurrentIndex);
void FeatureTest();
void KeyInputTest();
void ColorString(bool bSelected, Card::Color eColor, std::string& Text);
void OpenGLTest();