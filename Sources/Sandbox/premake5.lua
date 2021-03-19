project "Sandbox"
    language "C++"
    cppdialect (CPPDIALECT)
    rtti (RTTI)
    targetdir (TARGET_DIR)
    objdir (OUTPUT_DIR .. "/%{prj.name}")

    files { "**.hpp", "**.cpp" }

    includedirs
    {
        INCLUDE_PATHS.ROOT,
        INCLUDE_PATHS.SFML
    }

    libdirs
    {
        LIB_PATHS.SFML_EXTLIB
    }

    links
    {
        "opengl32",
        "openal32",
        "freetype",
        "winmm",
        "gdi32",
        "flac",
        "vorbisenc",
        "vorbisfile",
        "vorbis",
        "ogg",
        "ws2_32"
    }

    defines { "SFML_STATIC", "IMGUI_DISABLE_OBSOLETE_FUNCTIONS" }

    links { "ImGui" }

    filter "configurations:Debug"
        kind "ConsoleApp"
        defines { "DEBUG" }
        symbols "On"
        runtime "Debug"
        libdirs 
        {
            LIB_PATHS.SFML_DEBUG
        }
        links 
        {
            "sfml-graphics-s-d",
            "sfml-window-s-d",
            "sfml-system-s-d",
            "sfml-audio-s-d"
        }

    filter "configurations:Release"
        kind "ConsoleApp"
        defines { "NDEBUG", "RELEASE"}
        optimize "On"
        symbols "On"
        runtime "Release"
        libdirs 
        {
            LIB_PATHS.SFML_RELEASE
        }
        links
        {
            "sfml-graphics-s",
            "sfml-window-s",
            "sfml-system-s",
            "sfml-audio-s",
        }

        
    filter "configurations:Retail"
        kind "WindowedApp"
        defines { "NDEBUG", "RETAIL"}
        optimize "On"
        symbols "Off"
        runtime "Release"
        libdirs 
        {
            LIB_PATHS.SFML_RETAIL
        }
        links
        {
            "sfml-graphics-s",
            "sfml-window-s",
            "sfml-system-s",
            "sfml-audio-s",
            "sfml-main"
        }