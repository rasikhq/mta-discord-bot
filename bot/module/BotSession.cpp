#include "BotSession.h"

#include "include/ILuaModuleManager.h"

extern ILuaModuleManager10* pModuleManager;
extern std::deque<BotPulse*> g_Pulses;

std::deque<BotPulse*> g_ClearPulses;
std::mutex g_PulseMutex;

/* static */ void BotSession::Init(lua_State* vm)
{
	sol::state_view view(vm);
	sol::usertype<BotSession> usertype = view.new_usertype<BotSession>("DiscordBot",
		sol::constructors<
			BotSession()
		>()
	);

	usertype["connect"] = &BotSession::login;
	usertype["disconnect"] = &BotSession::disconnect;

	usertype["setPrefix"] = &BotSession::setPrefix;
	usertype["sendMessage"] = &BotSession::sendMessage;
	usertype["sendEmbed"] = &BotSession::sendEmbed;
	usertype["editMessage"] = &BotSession::editMessage;
	usertype["setActivity"] = &BotSession::setActivity;
}

BotSession::BotSession()
{
}

BotSession::~BotSession()
{
	disconnect();
}

void BotSession::Pulse()
{
	std::lock_guard<std::mutex> guard(g_PulseMutex);

	while (!g_Pulses.empty()) {
		BotPulse* pulse = g_Pulses.back();
		g_Pulses.pop_back();

		TRIGGER_MTA_EVENT(pulse->m_State, pulse->m_Event, pulse->m_Data);

		g_ClearPulses.push_back(pulse);
	}

	while (!g_ClearPulses.empty()) {
		BotPulse* pulse = g_ClearPulses.front();
		g_ClearPulses.pop_front();

		delete pulse;
	}
}

void BotSession::threadHandle()
{
	m_Bot->run();
}

void BotSession::login(const std::string& token, sol::this_state s)
{
	if (isReady())
		return;

	m_VM = s.lua_state();
	m_Userdata = lua_newuserdata(m_VM, 128);
	try {
		m_Bot = new Bot(token, m_VM, m_Userdata);
		m_Thread = new std::thread(&BotSession::threadHandle, this);
	}
	catch (const std::bad_alloc& e) {
		std::cout << "Failed to allocate heap (" << e.what() << ")\n";
	}
}

void BotSession::disconnect()
{
	if (m_Bot != nullptr)
	{
		m_Bot->quit();
		if (isConnected() && m_Thread->joinable())
			m_Thread->join();

		delete m_Thread;
		delete m_Bot;
		m_Thread = nullptr;
		m_Bot = nullptr;

		m_VM = nullptr;
	}
}

void BotSession::setPrefix(const std::string& newPrefix)
{
	if (!isConnected())
		return;

	m_Prefix = newPrefix; // This is for the operator() overload
	m_Bot->setPrefix(newPrefix);
}

void BotSession::sendMessage(const std::string& channelID, const std::string& message)
{
	if (!isConnected())
		return;

	try
	{
		m_Bot->sendMessage(channelID, message, SleepyDiscord::RequestMode::Async);
	}
	catch (SleepyDiscord::ErrorCode e)
	{
		pModuleManager->ErrorPrintf("BotSession::sendMessage Failed! Error: %d", e);
	}
}

void BotSession::sendEmbed(const std::string& channelID, const std::string& message)
{
	if (!isConnected())
		return;

	try
	{
		//nlohmann::json embedMessage(message);
		SleepyDiscord::Embed embed(message);
		if (!embed.empty())
			m_Bot->sendMessage(channelID, "", embed);
	}
	catch (...) 
	{
		pModuleManager->ErrorPrintf("BotSession::sendMessage Failed!");
	}
}

void BotSession::editMessage(const std::string& channelID, const std::string& messageID, const std::string& newMessage)
{
	if (!isConnected())
		return;

	try
	{
		m_Bot->editMessage(channelID, messageID, newMessage);
	}
	catch (SleepyDiscord::ErrorCode e)
	{
		pModuleManager->ErrorPrintf("BotSession::editMessage Failed! Error: %d", e);
	}
}

void BotSession::setActivity(const std::string& activity)
{
	if (!isConnected())
		return;

	try
	{
		m_Bot->updateStatus(activity);
	}
	catch (SleepyDiscord::ErrorCode e)
	{
		pModuleManager->ErrorPrintf("BotSession::setActivity Failed! Error: %d", e);
	}
}
