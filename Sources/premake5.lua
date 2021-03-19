workspace "M42-Engine"
    configurations { "Debug", "Release", "Retail"}
    architecture "x64"
    startproject "Sandbox"
    platforms { "Win64" }
 
    flags { "MultiProcessorCompile" , "FatalWarnings", "NoPCH"}
 
    CPPDIALECT = "C++latest"
    RTTI = "Off"
 
    TARGET_DIR = "%{wks.location}/../bin/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.architecture}"
    OUTPUT_DIR = "%{wks.location}/../bin/obj/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.architecture}"
 
    INCLUDE_PATHS = 
    {
        SFML = "%{wks.location}/SFML/Sources/include",
        ROOT = "%{wks.location}"
    }
 
    LIB_PATHS = 
    {
        SFML_DEBUG = "%{wks.location}/SFML/Build/lib/Debug",
        SFML_RELEASE = "%{wks.location}/SFML/Build/lib/RelWithDebInfo",
        SFML_RETAIL = "%{wks.location}/SFML/Build/lib/Release"
    }
    
    filter "action:vs*"
        LIB_PATHS.SFML_EXTLIB = "%{wks.location}/SFML/Sources/extlibs/libs-msvc-universal/x64"
        buildoptions {"/Zc:__cplusplus"}
 
    filter "system:windows"
        systemversion "latest"

    postbuildcommands {
        "{COPY} %{wks.location}/SFML/Sources/extlibs/bin/x64 " .. TARGET_DIR
    }
 
    include "Sandbox"
