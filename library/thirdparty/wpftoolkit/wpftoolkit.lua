-- library/thirdparty/wpftoolkit

local module = Module:new( "wpftoolkit" );

module:add_files( "wpftoolkit.lua" );
module:add_include_dir( "bin" );

module:add_library_file( path.getabsolute("bin/Xceed.Wpf.Toolkit") );
module:add_library_file( path.getabsolute("bin/Xceed.Wpf.DataGrid") );
module:add_library_file( path.getabsolute("bin/Xceed.Wpf.AvalonDock") );