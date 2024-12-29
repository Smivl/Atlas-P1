#pragma once

#include <string>

struct Event
{
};

struct SystemInit : public Event
{
};

struct DamageEvent : public Event
{
	DamageEvent(const int& damage) : Damage(damage) {}
	const int Damage;
};

struct MuteEvent : public Event
{
	MuteEvent(const std::string& channel, const char mute = false) : Channel(channel), Mute(mute) 
	{
		if (channel == "All")
			All = true;
	}
	const std::string Channel{ "None" };
	const char Mute;
	char All{ false };
};

struct PauseEvent : public Event
{
	PauseEvent(const std::string& channel, const char pause = false) : Channel(channel), Pause(pause)
	{
		if (channel == "All")
			All = true;
	}
	const std::string Channel{ "None" };
	const char Pause;
	char All{ false };
};