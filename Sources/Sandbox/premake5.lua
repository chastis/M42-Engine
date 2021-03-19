project "Sandbox"
    language "C++"
    rtti (RTTI)
    targetdir (TARGET_DIR)
    objdir (OUTPUT_DIR .. "/%{prj.name}")

    files { "**.hpp", "**.cpp" }

    includedirs
    {
        INCLUDE_PATHS.ROOT
    }

    defines { "IMGUI_DISABLE_OBSOLETE_FUNCTIONS" }


    filter "configurations:Debug"
        kind "ConsoleApp"
        defines { "DEBUG" }
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        kind "ConsoleApp"
        defines { "NDEBUG", "RELEASE"}
        optimize "On"
        symbols "On"
        runtime "Release"

        
    filter "configurations:Retail"
        kind "WindowedApp"
        defines { "NDEBUG", "RETAIL"}
        optimize "On"
        symbols "On"
        runtime "Release"