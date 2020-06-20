local module = Module:new();

module:add_files( "*.lua" );

module:set_base_path( "third_party/imgui" );
module:add_files( "*.h" );
module:add_files( "imgui.cpp" );
module:add_files( "imgui_draw.cpp" );
module:add_files( "imgui_widgets.cpp" );
module:add_include_dir( "." );

module:add_files( "imgui_demo.cpp" );
