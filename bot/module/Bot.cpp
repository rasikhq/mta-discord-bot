#include "Bot.h"

std::deque<BotPulse*> g_Pulses;

Bot::Bot(const std::string& token, lua_State* vm, void* userdata)
	: SleepyDiscord::DiscordClient(token)
{
	m_VM = vm;
	m_Userdata = userdata;
	m_State = new sol::state_view(m_VM);
}

Bot::~Bot()
{
	delete m_State;
}

void Bot::onReady(SleepyDiscord::Ready readyResult)
{
	nlohmann::json responseObject;
	responseObject["id"] = this->getID().string();
	responseObject["user"] = 
	{
		{"id", readyResult.user.ID.string()},
		{"username", readyResult.user.username}
	};

	try {
		g_Pulses.push_back(new BotPulse(m_State, "onDiscordReady", responseObject.dump()));
	}
	catch (const std::bad_alloc& e) {
		std::cout << "Failed to allocate heap (" << e.what() << ")\n";
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
	nlohmann::json responseObject;
	std::stringstream ss;
	responseObject["server"] =
	{
		{"id", server.ID.string()},
		{"name", server.name}
	};

	responseObject["roles"] = {};

	auto& rolesList = server.roles;
	for (auto& role : rolesList)
	{
		ss.str(std::string());
		ss << "0x" << std::hex << std::setfill('0') << std::setw(2) << role.color;
		
		responseObject["roles"].push_back
		({
			{"id", role.ID.string()}, 
			{"name", role.name},
			{"color", ss.str()}
		});
	}

	try {
		g_Pulses.push_back(new BotPulse(m_State, "onDiscordServer", responseObject.dump()));
	}
	catch (const std::bad_alloc& e) {
		std::cout << "Failed to allocate heap (" << e.what() << ")\n";
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
		nlohmann::json responseObject;

		const SleepyDiscord::ServerMember serverMember = getMember(message.serverID, message.author.ID);
		const SleepyDiscord::Channel channel = getChannel(message.channelID);
		auto& roles = serverMember.roles;

		responseObject["user"] = {};
		nlohmann::json rolesObj = {};

		for (auto& role : roles)
			rolesObj[role.string()] = true;

		responseObject["message"] = 
		{
			{"prefix", m_Prefix},
			{"raw_content", message.content},
			{"content", message.content.substr(m_Prefix.length())}
		};
		
		responseObject["message"]["author"] =
		{
			{"id", message.author.ID.string()},
			{"username", message.author.username},
			{"nickname", serverMember.nick},
			{"roles", rolesObj}
		};

		responseObject["message"]["channel"] = 
		{
			{"id", channel.ID.string()},
			{"name", channel.name},
			{"type", channel.type},
			{"topic", channel.topic}
		};

		try {
			g_Pulses.push_back(new BotPulse(m_State, "onDiscordMessage", responseObject.dump()));
		}
		catch (const std::bad_alloc& e) {
			std::cout << "Failed to allocate heap (" << e.what() << ")\n";
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