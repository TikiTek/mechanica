-- engine/tests/nettest/project

include "../../../buildtools/genie_scripts"

dofile( "../nettest.lua" );

local project = Project:new(
	"nettest",
	"7199737d-373c-440d-a955-3f17889ec005",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	find_module( "nettest" ),
	ProjectTypes.consoleApplication
);

local solution = Solution:new( "nettest" );
solution:add_project( project );

solution:finalize();
