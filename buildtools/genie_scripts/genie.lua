-- library/buildtools/genie_scripts

newoption { trigger = "outpath", description = "Location for generated project files" }

if not _OPTIONS["outpath"] then
	error("No outpath specified.")
end

global_configuration = {
	scripts_path = path.getdirectory( _SCRIPT ),
	root_path = path.getabsolute( path.getdirectory( _SCRIPT ) .. "/../../../" ),
	output_path = path.getrelative( _OPTIONS["outpath"], "." )
}

dofile( "thirdparty/toolchain.lua" )
dofile( "thirdparty/datadumper.lua" )

dofile( "global.lua" )
dofile( "configuration.lua" )
dofile( "module.lua" )
dofile( "project.lua" )
dofile( "solution.lua" )

add_module_include_path( global_configuration.root_path );
include( global_configuration.root_path );
