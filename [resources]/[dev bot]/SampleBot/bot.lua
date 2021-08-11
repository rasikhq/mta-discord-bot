BOT_TOKEN = "YOUR_TOKEN"
BOT_PREFIX = "."
CHANNEL_ID = "CHANNELID"

addEventHandler("onResourceStart", resourceRoot, function()
    print("Discord module script started")
    g_Bot = DiscordBot:new()

    g_Bot:setPrefix(BOT_PREFIX)
    g_Bot:connect(BOT_TOKEN)
end)

addEventHandler("onResourceStop", resourceRoot, function()
    print("Discord module script stopping")
    if g_Bot then
        g_Bot:disconnect()
        g_Bot = nil
    end
end)

addEvent("onDiscordReady", true)
addEventHandler("onDiscordReady", resourceRoot, function(jdata)
    local data = fromJSON(jdata)
    iprint("onDiscordReady", data)

    g_Bot:setActivity("Some Activity") -- Sets the Playing [ACTIVITY] for the bot
end)

addEvent("onDiscordResume", true)
addEventHandler("onDiscordResume", resourceRoot, function(jdata)
    local data = fromJSON(jdata)
    iprint("onDiscordResume", data)
end)

addEvent("onDiscordServer", true)
addEventHandler("onDiscordServer", resourceRoot, function(jdata)
    local data = fromJSON(jdata)
    iprint("onDiscordServer", data)
end)

addEvent("onDiscordMessage", true)
addEventHandler("onDiscordMessage", resourceRoot, function(jdata)
    local data = fromJSON(jdata)
    iprint("onDiscordMessage", data)
end)

addEventHandler("onPlayerJoin", root, function()
    g_Bot:sendMessage(CHANNEL_ID, getPlayerName(source).." joined the server")
end)

addEventHandler("onPlayerQuit", root, function(quitType, reason)
    g_Bot:sendMessage(CHANNEL_ID, getPlayerName(source).." left the server ("..quitType..") "..(reason and "["..reason.."]" or ""))
end)

addEventHandler("onPlayerChat", root, function(msg, msgType)
    g_Bot:sendMessage(CHANNEL_ID, getPlayerName(source) .. ": " .. msg)
end)

_getPlayerName = getPlayerName
function getPlayerName(player)
    return _getPlayerName(player):gsub("#%x%x%x%x%x%x", ""):gsub("_|*`", "")
end