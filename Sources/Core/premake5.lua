project "Core"
    language "C++"
    cppdialect (CPPDIALECT)
    rtti (RTTI)
    targetdir (TARGET_DIR)
    objdir (OUTPUT_DIR .. "/%{prj.name}")
    kind "StaticLib"

    files { "**.hpp", "**.cpp" }

    includedirs
    {
        INCLUDE_PATHS.ROOT
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines { "NDEBUG", "RELEASE"}
        optimize "On"
        symbols "On"
        runtime "Release"

    filter "configurations:Dist"
        defines { "NDEBUG", "Dist"}
        optimize "On"
        runtime "Release"

    filter "action:vs*"
        files {"**.natvis"}