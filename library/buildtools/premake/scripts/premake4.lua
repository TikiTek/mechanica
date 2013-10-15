-- library/buildtools/premake/scripts

dofile( "global.lua" )
dofile( "configuration.lua" )
dofile( "platform.lua" )
dofile( "module.lua" )
dofile( "project.lua" )

include( path.getabsolute( path.getdirectory( _SCRIPT ).."/../../../../" ) );
