-- library/buildtools/extensions/extension.qt

qt_dir = "C:/Qt/5.7/msvc2015_64" --TODO: find real path e.g. os.getenv("QTDIR")

local ui_script = path.getabsolute( path.join( path.getdirectory( _SCRIPT ), "actions/qt_ui.lua" ) );
local moc_script = path.getabsolute( path.join( path.getdirectory( _SCRIPT ), "actions/qt_moc.lua" ) );

function Module:add_ui_file( filename )
	local output_filename = "ui_" .. path.getbasename( filename ) .. ".h"

	local step_data = {
		source_filename = path.join( self.config.base_path, filename ),
		output_filename = output_filename,
		qt_dir = qt_dir
	}

	self:add_pre_build_step( ui_script, step_data );
	self:add_files( path.join( _OPTIONS[ "qt_dir" ], output_filename ), { optional = true} );
end

function Module:add_moc_file( filename )
	local output_filename = "moc_" .. path.getbasename( filename ) .. ".cpp";

	local step_data = {
		source_filename = path.join( self.config.base_path, filename ),
		output_filename = output_filename,
		qt_dir = qt_dir
	}
	
	self:add_pre_build_step( moc_script, step_data );
	self:add_files( path.join( _OPTIONS[ "qt_dir" ], output_filename ), { optional = true} );
end
