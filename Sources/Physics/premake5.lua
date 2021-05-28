project "Physics"
    kind "StaticLib"
    language "C++"
    cppdialect (CPPDIALECT)
    rtti (RTTI)
    targetdir (TARGET_DIR)
    objdir (OUTPUT_DIR .. "/%{prj.name}")

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
        
    filter "configurations:Retail"
        defines { "NDEBUG", "RETAIL"}
        optimize "On"
        symbols "Off"
        runtime "Release"