-- locale/game/project

include "../../../library/buildtools/premake/scripts"

local tiki3project = find_project( "game" );
finalize( "tiki3game", { tiki3project } );

-- A solution contains projects, and defines the available configurations
--[[ solution "tiki3"
	configurations { "Debug", "Release", "Master" }
	platforms { "x32", "x64", "native" }
	location( _OPTIONS["outpath"] )

   -- A project defines one build target
   project "tiki3game"
      kind "WindowedApp"
      language "C++"
      files { "include/**/*.hpp",  }
 
      configuration "Debug"
         defines { "DEBUG", "TIKI_BUILD_DEBUG=TIKI_ON" }
         flags { "Symbols" }
 
      configuration "Release"
         defines { "NDEBUG", "TIKI_BUILD_RELEASE=TIKI_ON" }
         flags { "Optimize" }

       configuration "Master"
         defines { "NDEBUG", "TIKI_BUILD_MASTER=TIKI_ON" }
         flags { "Optimize" }
]]--

