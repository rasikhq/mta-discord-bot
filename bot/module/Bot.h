#pragma once
#include <sol/sol.hpp>
#include <nlohmann/json.hpp>

#include "sleepy_discord/sleepy_discord.h"

#define TRIGGER_MTA_EVENT(STATE, EVENT, ...) (*STATE)["triggerEvent"](EVENT, (*STATE)["resourceRoot"], __VA_ARGS__) 

class Bot : public SleepyDiscord::DiscordClient
{
public:
	Bot(const std::string& token, lua_State* vm, void* userdata);
	~Bot();

	void onReady(SleepyDiscord::Ready readyResult) override;
	void onResumed() override;
	void onServer(SleepyDiscord::Server server) override;
	void onMessage(SleepyDiscord::Message message) override;

	void setPrefix(const std::string& newPrefix);

private:
	std::string m_Prefix = "!";

	lua_State* m_VM = nullptr;
	sol::state_view* m_State = nullptr;
	void* m_Userdata = nullptr;
};

struct BotPulse
{
	std::string m_Event, m_Data;
	sol::state_view* m_State;

	BotPulse(sol::state_view* _state, const std::string& _event, const std::string& _data)
		: m_State(_state), m_Event(_event), m_Data(_data) {}
};