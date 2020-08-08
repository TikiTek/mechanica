local step_script = path.getabsolute( path.join( path.getdirectory( _SCRIPT ), "actions/resources.lua" ) );

function Module:add_resources( pattern )
	local full_pattern = path.join( self.config.base_path, pattern );
	local exe_name = iff( is_windows, "resource_code_generator.exe", "resource_code_generator" );
	
	local step_data = {
		tool_path = path.join( global_configuration.root_path, 'engine/buildtools/resource_code_generator/' .. exe_name ),
		input_pattern = full_pattern,
		output_name = self.name
	}
	
	self:add_pre_build_step( step_script, step_data );

	local basename = "res_" .. self.name;
	local filename = path.join( _OPTIONS[ "generated_files_dir" ], basename );
	self:add_files( filename .. '.hpp', { optional = true } );
	self:add_files( filename .. '.cpp', { optional = true } );
end
