#pragma once

#include "pch.h"
#include "graphics\font.h"
#include "graphics\renderer.h"

class Resources
{
public:
	static void Load()
	{
		Calibri = new Font("res/calibri.ttf", 64);
	}

	static void Unload()
	{
		if (Calibri) delete Calibri;
	}

	static Font* Calibri;
};