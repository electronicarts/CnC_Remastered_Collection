#pragma once

#include "FIXED.H"
#include "DEFINES.H"
#include "FACE.H"
#include "FUNCTION.H"
#include "TECHNO.H"

class Resurgence
{
public:
	static void on_kill_event(TechnoClass* victor, TechnoClass* victim);
	static unsigned int calculate_experience(TechnoClass* victor, TechnoClass* victim);
	static unsigned int calculate_level(TechnoClass *unit);
};