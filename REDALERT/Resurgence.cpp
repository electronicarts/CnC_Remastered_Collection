#include "Resurgence.h"

void Resurgence::on_kill_event(TechnoClass* victor, TechnoClass* victim)
{
	victor->Experience += calculate_experience(victor, victim);
}

unsigned int Resurgence::calculate_experience(TechnoClass* victor, TechnoClass* victim)
{
	return 100;
}

unsigned int Resurgence::calculate_level(TechnoClass* unit)
{
	return (unit->Experience / 1000) % 5;
}
