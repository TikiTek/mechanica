-- local/game/project

include "../../../library/buildtools/genie_scripts"

local projects =
{
	find_project( "game" ),
	find_project( "converterlibrary" )	
};

--if use_msvc then
--	projects[#projects+1] = find_project( "webinterfacelibrary" );
--end

finalize( "game", projects );
