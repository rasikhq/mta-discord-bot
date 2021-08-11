# MTA Discord Module
Discord bot module for MTA. Does not require NodeJS setup or sockets module. You just need to setup a bot on discord developer site and get the token.

**Note:**
- This module is still in development and may be incomplete, unstable, and/or broken. It can crash your MTA server, use at your own risk.
- Only x64 (64-bit) is supported. I'll add support for 32-bit if I get the time.

# Installation
- Simply grab the DLL or SO file from Release and follow installation steps at the bottom of this doc

# Build Guidelines
## Requirements
- Visual Studio 2019 (Windows) or Latest GCC/G++ compilers and dev-essentials (Linux)
- CMake (apt install cmake OR [Get it for windows here](https://cmake.org/download/))
- OpenSSL (apt install libssl-dev OR [Get it for windows here](https://slproweb.com/products/Win32OpenSSL.html))

## Building
- Clone the repository (git clone https://github.com/DizzasTeR/mta-discord-bot.git)
- Go into the cloned dir (cd mta-discord-bot)
- Create a build dir and enter it (mkdir build && cd build)
- Issue the CMake command to configure and generate project files (cmake ..)

### Building - Windows
- After the CMake command, you should have Visual Studio solution which you can open and build
- If build succeeded, you should have DDiscord.dll in build/bot/Release/DDiscord.dll

### Building - Linux
- Issue a make command (make)
- If build succeeded, you should have libDDiscord.so in build/bot/libDDiscord.so

# Installing the module
## 64-bit
- Copy the built module (DDiscord.dll or libDDiscord.so depending on your operating system)
- Paste it in *YOUR_MTA_SERVER_DIRECTORY/x64/modules/* (Create the folders if they don't exist)

## 32-bit
- Copy the built module (DDiscord32.dll or libDDiscord32.so depending on your operating system)
- Paste it in *YOUR_MTA_SERVER_DIRECTORY/mods/deathmatch/modules/* (Create the folders if they don't exist)

# Loading the module
- You have the following ways

## MTA server config
- Inside the server config, search for module
- You can see example code in the config to load modules on server start-up
- Example: 
```xml
<!-- 64-bit -->
<module src="DDiscord.dll"/>

<!-- 32-bit -->
<module src="DDiscord32.dll"/>
```
OR
```xml
<!-- 64-bit -->
<module src="libDDiscord.so"/>

<!-- 32-bit -->
<module src="libDDiscord32.so"/>
```

## Load from server console
- Run loadmodule DDiscord.dll OR loadmodule libDDiscord.so depending on your OS to load the module