newoption({
	trigger = "ui-backend",
	value = "API",
	description = "Choose a backend for UI",
	allowed = {
		{"motif", "X11 Motif"},
		{"win32", "Win32 API"}
	},
	category = "UI",
	default = "motif"
})

function ui_flags()
	filter({
		"platforms:Native",
		"system:not windows"
	})
		includedirs({
			"/usr/local/include",
			"/usr/X11R6/include",
			"/usr/X11R7/include",
			"/usr/X11/include"
		})
		libdirs({
			"/usr/local/lib",
			"/usr/X11R6/lib",
			"/usr/X11R7/lib",
			"/usr/X11/lib"
		})
	filter({
		"platforms:Native",
		"system:not windows",
		"toolset:gcc or toolset:clang"
	})
		linkoptions({
			"-Wl,-R/usr/local/lib",
			"-Wl,-R/usr/X11R6/lib",
			"-Wl,-R/usr/X11R7/lib",
			"-Wl,-R/usr/X11/lib"
		})
	filter({
		"platforms:Native",
		"system:bsd"
	})
		includedirs({
			"/usr/pkg/include"
		})
		libdirs({
			"/usr/pkg/lib"
		})
	filter({
		"platforms:Native",
		"system:bsd",
		"toolset:gcc or toolset:clang"
	})
		linkoptions({
			"-Wl,-R/usr/pkg/lib"
		})
	filter({})
end

project("ui")
	kind("StaticLib")
	targetdir("lib/%{cfg.buildcfg}/%{cfg.platform}")
	includedirs("include")

	ui_flags()

	files("src/*.c")
	files("src/" .. _OPTIONS["ui-backend"] .. "/*.c")

local outfile = io.open("include/libui_config.h", "w")
outfile:write("#ifndef __LIBUI_CONFIG_H__\n")
outfile:write("#define __LIBUI_CONFIG_H__\n")
outfile:write("#define LIBUI_" .. string.upper(_OPTIONS["ui-backend"]) .. "\n")
outfile:write("#endif\n")
outfile:close()
