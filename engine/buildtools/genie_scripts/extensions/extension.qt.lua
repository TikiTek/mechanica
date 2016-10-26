-- library/buildtools/extensions/extension.qt

function Module:add_ui_file( file_name )
	self:add_pre_build_step( "ui", { filename = path.join( self.config.base_path, file_name ) } );
end

function Module:add_moc_file( file_name )
	self:add_pre_build_step( "ui", { filename = path.join( self.config.base_path, file_name ) } );
end
