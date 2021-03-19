workspace "M42-Engine"
   configurations { "Debug", "Release", "Retail"}
   architecture "x64"
   startproject "Sandbox"
   platforms { "Win64" }

   flags { "MultiProcessorCompile" , "FatalWarnings", "NoPCH"}

   RTTI = "Off"

   TARGET_DIR = "%{wks.location}/../bin/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.architecture}"
   OUTPUT_DIR = "%{wks.location}/../bin/obj/%{cfg.buildcfg}-%{cfg.platform}-%{cfg.architecture}"

   INCLUDE_PATHS = 
   {
      ROOT = "%{wks.location}"
   }

   LIB_PATHS = 
   {

   }

   filter "action:vs*"
      buildoptions {"/Zc:__cplusplus", "/std:c++latest"}

   filter "system:windows"
      systemversion "latest"

   include "Sandbox"
