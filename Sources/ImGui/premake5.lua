project "ImGui"
    language "C++"
    cppdialect (CPPDIALECT)
    rtti (RTTI)
    targetdir (TARGET_DIR)
    objdir (OUTPUT_DIR .. "/%{prj.name}")
    kind "StaticLib"

    files { "**.h", "**.cpp" }

    includedirs
    {
        INCLUDE_PATHS.ROOT,
        INCLUDE_PATHS.SFML
    }

    defines { "SFML_STATIC", "IMGUI_DISABLE_OBSOLETE_FUNCTIONS" }

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

    filter "action:vs*"
        files {"**.natvis"}

    filter "system:windows"
        defines { "_CRT_SECURE_NO_WARNINGS" }