-- library/modules/runtime/qtassetconverter

local module = Module:new( "qtassetconverter" );

module:add_files( "include/**/*.*" );
module:add_include_dir( "include" );
module:add_files( "*.lua" );
module:add_files( "source/*.*" );

module:add_dependency( "core/base" );
module:add_dependency( "thirdparty/qt" );

module.import_func = function(project)
	module:add_moc_files( "include/tiki/qtassetconverter/qt_asset_converter_widget.hpp" );
	module:add_moc_files( "include/tiki/qtassetconverter/qt_convert_result_widget.hpp" );
end