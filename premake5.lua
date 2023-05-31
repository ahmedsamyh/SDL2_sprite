workspace "SDL2_sprite"
    configurations {"Debug", "Release"}
    location "build"

project "SDL2_sprite"
    kind "ConsoleApp"
    language "C"
    architecture "x64"
    cdialect "C17"
    targetdir "bin/%{cfg.buildcfg}"

files {"src/main.c"}
includedirs {"include"}

filter "configurations:Debug"
    defines {"DEBUG"}
    symbols "On"
    links {"lib/Debug/SDL2d.lib"}
    links {"lib/Debug/SDL2maind.lib"}

filter "configurations:Release"
    defines {"NDEBUG"}
    optimize "On"
    links {"lib/Debug/SDL2.lib"}
    links {"lib/Debug/SDL2main.lib"}

filter {}
----------------------------------------------------
