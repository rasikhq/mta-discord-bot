#include "Bot.h"

std::deque<BotPulse*> g_Pulses;

Bot::Bot(const std::string& token, lua_State* vm, void* userdata)
	: SleepyDiscord::DiscordClient(token)
{
	m_VM = vm;
	m_State = new sol::state_view(m_VM);
	m_Userdata = userdata;
}

Bot::~Bot()
{
	delete m_State;
}

void Bot::onReady(SleepyDiscord::Ready readyResult)
{
	nlohmann::json o;
	o["id"] = this->getID().string();
	o["user"] = 
	{
		{"id", readyResult.user.ID.string()},
		{"username", readyResult.user.username}
	};

	try {
		g_Pulses.push_back(new BotPulse(m_State, "onDiscordReady", o.dump()));
		//TRIGGER_MTA_EVENT(m_State, "onDiscordReady", o.dump());
	}
	catch (const nlohmann::json::exception& e) {
		std::cerr << e.what() << '\n';
	}
}

void Bot::onResumed()
{
	g_Pulses.push_back(new BotPulse(m_State, "onDiscordResume", "[]"));
}

void Bot::onServer(SleepyDiscord::Server server)
{
	nlohmann::json o;
	std::stringstream ss;
	o["server"] =
	{
		{"id", server.ID.string()},
		{"name", server.name}
	};

	o["roles"] = {};

	auto& rolesList = server.roles;
	for (auto& role : rolesList)
	{
		ss.str(std::string());
		ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << role.color;
		
		o["roles"].push_back
		({
			{"id", role.ID.string()}, 
			{"name", role.name},
			{"color", ss.str()}
		});
	}

	try {
		g_Pulses.push_back(new BotPulse(m_State, "onDiscordServer", o.dump()));
		//TRIGGER_MTA_EVENT(m_State, "onDiscordServer", o.dump());
	}
	catch (const nlohmann::json::exception& e) {
		std::cerr << e.what() << '\n';
	}
}

void Bot::onMessage(SleepyDiscord::Message message)
{
	if (message.author.bot) return;

	if (message.startsWith(m_Prefix))
	{
		nlohmann::json o;

		const SleepyDiscord::ServerMember serverMember = getMember(message.serverID, message.author.ID);
		const SleepyDiscord::Channel channel = getChannel(message.channelID);
		auto& roles = serverMember.roles;

		o["user"] = {};
		nlohmann::json rolesObj = {};

		for (auto& role : roles)
			rolesObj[role.string()] = true;

		o["message"] = 
		{
			{"prefix", m_Prefix},
			{"raw_content", message.content},
			{"content", message.content.substr(m_Prefix.length())}
		};
		
		o["message"]["author"] =
		{
			{"id", message.author.ID.string()},
			{"username", message.author.username},
			{"nickname", serverMember.nick},
			{"roles", rolesObj}
		};

		o["message"]["channel"] = 
		{
			{"id", channel.ID.string()},
			{"name", channel.name},
			{"type", channel.type},
			{"topic", channel.topic}
		};

		try {
			g_Pulses.push_back(new BotPulse(m_State, "onDiscordMessage", o.dump()));
			//TRIGGER_MTA_EVENT(m_State, "onDiscordMessage", o.dump());
		}
		catch (const nlohmann::json::exception& e) {
			std::cerr << e.what() << '\n';
		}
	}
}



void Bot::setPrefix(const std::string& newPrefix)
{
	m_Prefix = newPrefix;
}