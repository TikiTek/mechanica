local module = Module:new( "qtassetconverter" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );
module:add_files( "*.lua" );
module:add_files( "source/*.*" );

module:add_dependency( "core/base" );
module:add_dependency( "thirdparty/qt" );

module.import_func = function(project)
	module:add_moc_files( "include/tiki/qtassetconverter/*widget.hpp" );
end