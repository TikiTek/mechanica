
ModuleExtensions:add_new_hook( function( module )
	module.resources = {}
end )

ModuleExtensions:add_pre_finalize_hook( function( solution, project, module )
	for _, pattern in ipairs( module.resources ) do
		local full_pattern = path.join( module.config.base_path, pattern )
		local exe_name = iff( tiki.platform == Platforms.Windows, ".exe", "" )
		
		local step_data = {
			tool_path = path.join( tiki.project_path, "engine/buildtools/resource_code_generator/resource_code_generator" .. exe_name ),
			input_pattern = full_pattern,
			output_name = module.name
		}
		
		module:add_pre_build_step( "resources", step_data )

		local basename = "res_" .. module.name
		local filename = path.join( project.generated_files_dir, basename )
		module:add_files( filename .. ".hpp", { optional = true } )
		module:add_files( filename .. ".cpp", { optional = true } )
	end
end )

function Module:add_resources( pattern )
	table.insert( self.resources, pattern )
end

function Project:add_resources( pattern )
	self.module:add_resources( pattern )
end
