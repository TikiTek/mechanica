-- local/mastering

local currentDir = os.getcwd();
local baseDir = "../../..";

local converterDir = path.getabsolute(path.join(baseDir, "library/tools/commandlineconverter"), currentDir);
local gameDir = path.getabsolute(path.join(baseDir, "local/game"), currentDir);

local configDir = path.getabsolute(path.join(baseDir, "local/mastering/configuration"), currentDir);
local gamebuildDir = path.getabsolute(path.join(baseDir, "gamebuild"), currentDir);
local contentDir = path.getabsolute(path.join(baseDir, "content"), currentDir);

local outputDir = path.getabsolute('../output', currentDir);
local logFilename = path.join(outputDir, 'master.log');

local msBuildExe = nil;
local nsisExe = path.getabsolute(path.join(baseDir, "library/buildtools/nsis/makensis.exe"), currentDir);
local mkisofsExe = path.getabsolute(path.join(baseDir, "library/buildtools/cdrtools/mkisofs.exe"), currentDir);

local logFile = nil;

function l(message, notPrint)
	logFile:write(message .. "\n");
	
	if not notPrint then
		print(message);
	end
end

function throw( text )
	l( "Exception: " .. text );
	l( debug.traceback() );
	error();
end

function iff( expr, when_true, when_false )
	if expr then
		return when_true
	else
		return when_false
	end
end

function writeAllText(fileName, content)
	local f = io.open(fileName, 'w+');
	if not f then
		throw('can\'t open ' .. fileName);
	end
	
	f:write(content);
	f:close();
end

function os.executew(commandLine, workingDir, errorMessage)
	if workingDir then
		os.chdir(workingDir);
	end
	
	local buildProcess = io.popen(commandLine);
	local buildProcessOutput = buildProcess:read('*a');
	local buildProcessResult = buildProcess:close();

	l(buildProcessOutput, true);
	
	if not buildProcessResult then
		throw(iff(errorMessage, errorMessage, 'Unknown build error!'));
	end
	
	return buildProcessResult, buildProcessOutput;
end

function cleanUp()
	l('Clean up ' .. outputDir .. '...');
	os.rmdir(outputDir);
	os.mkdir(outputDir);
end

function buildProject(projectPath, config)
	local projectName = path.getname(projectPath);
	
	l('Generate ' .. projectName .. '...');	
	os.executew('create_visualstudio.cmd', projectPath, 'Generation failed!');
	
	l('Building ' .. projectName .. '...');
	
	local projectFile = path.join(projectPath, 'build', projectName .. '.sln');
	local commandLine = string.format(
		[[""%s" /property:Configuration=%s /property:Platform=x64 /verbosity:m /nologo "%s""]],
		msBuildExe,
		config,
		string.gsub(projectFile, '/', "\\")
	);
	
	local buildDir = path.join(projectPath, 'build');
	local buildProcessResult, buildProcessOutput = os.executew(commandLine, buildDir, 'Build');

	return string.match(buildProcessOutput, '-> ([a-zA-Z]:[a-zA-Z0-9\\/.-_ \(\)]+.exe)');
end

function buildGame()
	buildProject(gameDir, 'Master');
end

function buildAssets()
	local converterExe = buildProject(converterDir, 'Release');

	l('Building assets...');
	local commandLine = string.format(
		[[""%s" --content-dir=%s --target-dir=%s"]],
		converterExe,
		contentDir,
		gamebuildDir
	);
	
	os.executew(commandLine, nil, 'Asset build failed!');
end

function buildSetup()
	l("Build Setup...");
	
	local scriptsDir = path.join(outputDir, "scripts")
	os.mkdir(scriptsDir);
	os.mkdir(path.join(outputDir, "setup"));
	
	local gamebuildPattern = path.join(gamebuildDir, '*');
	local gamebuildFiles = os.matchfiles(gamebuildPattern);
	
	local contentInstall = "";
	local contentUninstall = "";
	for i,fileName in pairs( gamebuildFiles ) do
		local fullPath = string.gsub(path.getabsolute(fileName), '/', "\\");
	
		contentInstall = contentInstall .. string.format("File \"%s\"\n", fullPath);
		contentUninstall = contentUninstall .. string.format("Delete \"%s\"\n", fullPath);
	end
	writeAllText(path.join(scriptsDir, 'install.nsh'), contentInstall);
	writeAllText(path.join(scriptsDir, 'uninstall.nsh'), contentUninstall);
	
	local commandLine = string.format(
		[[""%s" setup.nsi"]],
		nsisExe
	);
	os.executew(commandLine, currentDir, 'Setup build failed!');
end

function buildIso()
	--cdrtools\mkisofs.exe -o steam_os2.iso -iso-level 3 -r -R -l -L -J -allow-lowercase -allow-multidot data_folder
	--pause
	
	l("Build ISO-Image...");
	
	local commandLine = string.format(
		[[""%s" -o "%s" -l -hfs %s %s"]],
		mkisofsExe,
		path.join(outputDir, 'eu_image_win.iso'),
		path.join(configDir, 'cdcontent'),
		path.join(outputDir, 'setup')
	);
	os.executew(commandLine, nil, "ISO build failed!");
end

newaction {
    trigger = "master",
    description = "Generates an final Version",

    onStart = function()        
		logFile = io.open(logFilename, "w+");
		
		local msBuildPlatform = iif(os.is64bit(), '/reg:64', '/reg:32');
		local msBuildCommand = 'reg.exe query "HKLM\\SOFTWARE\\Microsoft\\MSBuild\\ToolsVersions\\14.0" /v MSBuildToolsPath ' .. msBuildPlatform
		local msBuildReg = io.popen(msBuildCommand);
		local msBuildResult = msBuildReg:read('*a');
		msBuildReg:close();
		
		msBuildExe = string.match(msBuildResult, "([a-zA-Z]:[a-zA-Z0-9\\/.-_ \(\)]+)");
		msBuildExe = string.gsub(msBuildExe, "\\", '/');
		msBuildExe = path.join(msBuildExe, 'MSBuild.exe');
		msBuildExe = string.gsub(msBuildExe, '/', "\\");
    end,

    onWorkspace = function(wks)
    end,

    onProject = function(prj)
    end,

    execute = function()
		cleanUp();
		buildGame();
		buildAssets();
		buildSetup();
		buildIso();
    end,

    onEnd = function()
		logFile:close();
    end
	
}
