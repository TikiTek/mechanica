-- library/tools/editor

local module = Module:new( "editor" );

module:add_files( "source/*.*" );

module:set_flag( "WPF" );

module:add_library_file( "System" );
module:add_library_file( "System.Core" );
module:add_library_file( "System.Drawing" );
module:add_library_file( "System.Windows.Forms" );
module:add_library_file( "System.Xaml" );
module:add_library_file( "WindowsBase" );
module:add_library_file( "PresentationCore" );
module:add_library_file( "PresentationFramework" );
module:add_library_file( "Microsoft.CSharp" );
module:add_library_file( "WindowsFormsIntegration" );

module:add_library_file( "editornative" );

local project = Project:new(
	"editor",
	{ "x32", "x64" },
	{ "Debug", "Release" },
	module,
	ProjectTypes.windowApplication
);

project.lang = ProjectLanguages.cs;
