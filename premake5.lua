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

project("ui")
	kind("StaticLib")
	targetdir("lib/%{cfg.buildcfg}/%{cfg.platform}")
	includedirs("include")
	files("src/*.c")
	files("src/" .. _OPTIONS["ui-backend"] .. "/*.c")
