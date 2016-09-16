#pragma once

#include "resource.h"
#include "targetver.h"
#define DONNYLIBUSEDLL
#include <Donny/DonnyLib.h>
#include <Donny/DonnyWindowsLib.h>
using namespace Donny;
using namespace DWindow;

#define GetStringAndSizeFromStr(Str) Str,wcslen(Str)

/* Horizontal */
const int nHButtonW = 100;
const int nHButtonH = 140;
const int nHBTextY = 100;
/* Vertical */
const int nVButtonW = 250;
const int nVButtonH = 50;
const int nVButtonPadding = 3;
const int nVBtnIconPadding = 5;
const int nVBtnIconSize = nVButtonH - nVBtnIconPadding * 2;
const int nVBTextPadding = 10;
const int nVBTextX = nVButtonH + nVBTextPadding;
enum VerticalStyle {
	VStyle1,
	VStyle2
};

class DWPCBK;