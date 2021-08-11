add_library(DDiscord SHARED
   	# Headers
		module/include/ILuaModuleManager.h
		module/include/lauxlib.h
		module/include/lua.h
		module/include/luaconf.h
		module/include/lualib.h

		module/vendor/sol/config.hpp
		module/vendor/sol/forward.hpp
		module/vendor/sol/sol.hpp
	
		module/vendor/nlohmann/json.hpp

		module/Bot.h
		module/BotSession.h
	# Source
		module/Bot.cpp
		module/BotSession.cpp
		
		module/Main.cpp
)

if(NOT WIN32)
	target_sources(DDiscord PRIVATE
		# Headers
			module/luaimports/luaimports.linux.h
		# Source
			module/luaimports/luaimports.linux.cpp
	)
else()
	target_link_libraries(DDiscord ${PROJECT_SOURCE_DIR}/bot/module/lib/lua5.1_64.lib)
endif()

target_include_directories(DDiscord
	PRIVATE
		module
		module/include
		module/vendor
		module/luaimports
)

target_compile_features(DDiscord PUBLIC cxx_std_20)
target_link_libraries(DDiscord sleepy-discord)