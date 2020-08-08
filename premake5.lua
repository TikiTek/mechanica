-- /

dofile( path.join( path.getdirectory( _SCRIPT ), "engine/buildtools/tiki_build/tiki_build.lua" ) );

tiki.project_path = path.getabsolute( path.getdirectory( _SCRIPT ) )

import( "engine" );
import( "local" );
