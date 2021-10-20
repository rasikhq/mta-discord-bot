#pragma once
#include "Bot.h"

class BotSession
{
public:
	static void Init(lua_State* vm);
	static void Pulse();

public:
	BotSession();
	~BotSession();

private: // Class
	void threadHandle();
	inline bool isReady() { return (m_Bot != nullptr); }
	inline bool isConnected() { return (m_Thread != nullptr); }

// Lua
public:
	bool operator==(const BotSession& other) { return this->m_Userdata == other.m_Userdata; }
private:
	void login(const std::string& token, sol::this_state s);
	void disconnect();

	void setPrefix(const std::string& newPrefix);
	void sendMessage(const std::string& channelID, const std::string& message);
	void sendEmbed(const std::string& channelID, const std::string& message);
	void editMessage(const std::string& channelID, const std::string& messageID, const std::string& newMessage);
	void setActivity(const std::string& activity);

private:
	std::string m_Prefix;

	lua_State* m_VM = nullptr;
	std::thread* m_Thread = nullptr;
	Bot* m_Bot = nullptr;
	void* m_Userdata = nullptr;
};