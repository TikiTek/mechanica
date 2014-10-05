newoption { trigger = "outpath", description = "Location for generated project files" }
newoption { trigger = "unity_dir", description = "" }

file_actions = {};
global_configuration = { enable_unity_builds = false };

if not _OPTIONS["outpath"] then
	error("No outpath specified.")
end

function throw( text )
	print( "Exception: " .. text );
	print( debug.traceback() );
	error();
end

function iff( expr, when_true, when_false )
	if expr then
		return when_true
	else
		return when_false
	end
end

function vardump(value, depth, key)
  local linePrefix = ""
  local spaces = ""

  if key == "__index" then
	return
  end
  
  if key ~= nil then
    linePrefix = "["..key.."] = "
  end
  
  if depth == nil then
    depth = 0
  else
    depth = depth + 1
    for i=1, depth do spaces = spaces .. "  " end
  end
  
  if type(value) == 'table' then
    mTable = getmetatable(value)
    if mTable == nil then
      print(spaces ..linePrefix.."(table) ")
    else
      print(spaces .."(metatable) ")
        value = mTable
    end		
    for tableKey, tableValue in pairs(value) do
      vardump(tableValue, depth, tableKey)
    end
  elseif type(value)	== 'function' or 
		 type(value)	== 'thread' or 
		 type(value)	== 'userdata' or
		value			== nil
  then
    print(spaces..tostring(value))
  else
    print(spaces..linePrefix.."("..type(value)..") "..tostring(value))
  end
end

function io.exists( file_name )
	local file_handle = io.open( file_name, "rb" );
	if file_handle then
		file_handle:close();
		return true;
	end
	return false;
end

function table.join( table, src1, src2, src3 )
	if ( table == nil or src1 == nil ) then
		throw( "table.join: null argument exception" );
	end

	for i,val in pairs( src1 ) do
		table[ i ] = val;
	end

	if src2 ~= nil then
		for i,val in pairs( src2 ) do
			table[ i ] = val;
		end
	end

	if src3 ~= nil then
		for i,val in pairs( src3 ) do
			table[ i ] = val;
		end
	end

	return table;
end

function table.join_array( table, array )
	if ( table == nil or array == nil ) then
		throw( "table.join_array: null argument exception" );
	end

	for i,arr in pairs( array ) do
		for j,val in pairs( arr ) do
			table[ j ] = val;
		end
	end

	return table;
end

function table.index_of( table, object )
	local result = -1;
 
	if type( table ) ~= "table" then
		throw( "not a table" );
	end
	
	local count = #table;
	for i = 0,count do
		if table[ i ] == object then
			result = i
			break
		end
	end
	
	return result;	
end

function copy_instance( target, source )
	for name,value in pairs( source ) do		
		if ( type( value ) == "table" and name ~= "__index" ) then
			target[ name ] = {};
			copy_instance( target[ name ], value );
		else
			target[ name ] = value;
		end
	end
end

function class( init )
	local cls = init;
	cls.__index = cls;

	return cls;
end

function class_instance( class )
	local new_instance = {};
	copy_instance( new_instance, class );
	setmetatable( new_instance, class );

	return new_instance;
end

function include_sub_directories()
	for i,dir in pairs( os.matchdirs("*") ) do
		include( dir );
	end
end

function get_config_dir( platform, configuration )
	if ( platform == nil or configuration == nil ) then
		throw( "get_config_dir: too few arguments." );
	end

	return _OPTIONS[ "outpath" ] .. "/" .. platform .. "/" .. configuration;
end

function file_action( file_name, action )
	file_actions[ file_name ] = action;
end

function finalize( output_name, projects )
	if type( output_name ) ~= "string" then
		throw "finalize: output_name is not a string.";
	end

	if type( projects ) ~= "table" then
		throw "finalize: projects argument are invalid.";
	end

	local var_platforms = {};
	local var_configurations = {};

	for i,project in pairs( projects ) do
		for i,platform in pairs( project.platforms ) do
			if not table.contains( var_platforms, platform ) then 
				table.insert( var_platforms, platform );
			end
		end
		for j,configuration in pairs( project.configurations ) do
			if not table.contains( var_configurations, configuration ) then 
				table.insert( var_configurations, configuration );
			end
		end		
	end
	
	solution( output_name );
	configurations( var_configurations );
	platforms( var_platforms );
	location( _OPTIONS[ "outpath" ] )

	for i,project in pairs( projects ) do
		print( "Project: " .. project.name );

		_OPTIONS[ "unity_dir" ] = path.join( _OPTIONS[ "outpath" ], "unity_files", project.name )
		if not os.isdir( _OPTIONS[ "unity_dir" ] ) then
			os.mkdir( _OPTIONS[ "unity_dir" ] )
		end

		project:finalize();
	end
end