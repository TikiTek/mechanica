-- local/mastering/premake5.lua

local currentDir = os.getcwd();
local baseDir = "../../..";
local converterDir = path.getabsolute(path.join(baseDir, "library/tools/commandlineconverter"), currentDir);
local gameDir = path.getabsolute(path.join(baseDir, "local/game"), currentDir);
local gamebuildDir = path.getabsolute(path.join(baseDir, "gamebuild"), currentDir);
local contentDir = path.getabsolute(path.join(baseDir, "content"), currentDir);

local outputDir = path.getabsolute('../output', currentDir);
local logFilename = path.join(outputDir, 'master.log');

local msBuildPath = nil;

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

function cleanUp()
	l('Clean up ' .. outputDir .. '...');
	os.rmdir(outputDir);
	os.mkdir(outputDir);
end

function buildProject(projectPath, config)
	local projectName = path.getname(projectPath);
	l('Generate ' .. projectName .. '...');
	
	os.chdir(projectPath);
	local createProcess = io.popen('create_visualstudio.cmd');
	local createProcessOutput = createProcess:read('*a');
	local createProcessResult = createProcess:close();
	
	l(createProcessOutput, true);

	if not createProcessResult then
		throw('Generation failed!');
	end
	
	l('Building ' .. projectName .. '...');
	
	local projectFile = path.join(projectPath, 'build', projectName .. '.sln');
	local msBuildCommandLine = string.format(
		[[""%s" /property:Configuration=%s /property:Platform=x64 /verbosity:m /nologo "%s""]],
		msBuildPath,
		config,
		string.gsub(projectFile, '/', "\\")		
	);
	
	local buildProcess = io.popen(msBuildCommandLine);
	local buildProcessOutput = buildProcess:read('*a');
	local buildProcessResult = buildProcess:close();

	l(buildProcessOutput, true);
	
	if not buildProcessResult then
		throw('Build failed!');
	end
	
	return string.match(buildProcessOutput, '-> ([a-zA-Z]:[a-zA-Z0-9\\/.-_ \(\)]+.exe)');
end

function buildGame()
	buildProject(gameDir, 'Master');
end

function buildAssets()
	local converterExe = buildProject(converterDir, 'Release');

	l('Building assets...');
	local converterCommandLine = string.format(
		[[""%s" --content-dir=%s --target-dir=%s"]],
		converterExe,
		contentDir,
		gamebuildDir
	);
		
	local buildProcess = io.popen(converterCommandLine);
	local buildProcessOutput = buildProcess:read('*a');
	local buildProcessResult = buildProcess:close();

	l(buildProcessOutput, true);
	
	if not buildProcessResult then
		throw('Build failed!');
	end
end

function buildSetup()
	
end

function buildIso()
	
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
		
		msBuildPath = string.match(msBuildResult, "([a-zA-Z]:[a-zA-Z0-9\\/.-_ \(\)]+)");
		msBuildPath = string.gsub(msBuildPath, "\\", '/');
		msBuildPath = path.join(msBuildPath, 'MSBuild.exe');
		msBuildPath = string.gsub(msBuildPath, '/', "\\");
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


--[[
bool ask = false;
if (args.Length > 0)
{
	ask = (args[0] == "ask");
}

MasteringScript.CleanUp();
MasteringScript.BuildMaster(ask);
MasteringScript.BuildAssets(ask);
MasteringScript.BuildSetup(ask);
MasteringScript.BuildIso(ask);

//BeginScript
using System;
using System.IO;
using System.Diagnostics;

public static class MasteringScript
{

	private static string _cmakeExe = @"..\..\library\buildtools\cmake\bin\cmake.exe";

	private static bool _ask(bool ask, string text)
	{
		if (ask)
		{
			Console.Write(text + "(y/n)? ");
			string answer = "";
			while (answer != "y")
			{
				answer = Console.ReadLine();
				if (answer == "n") return false;
			}
		}
		
		return true;
	}

	private static void _start(string exeName, string[] arguments)
	{
		_start(exeName, arguments, ".");
	}

	private static void _start(string exeName, string[] arguments, string workingDir)
	{
		if (arguments == null)
		{
			arguments = new string[0];
		}
	
		ProcessStartInfo info = new ProcessStartInfo();
		info.FileName = exeName;
		info.Arguments = String.Join(" ", arguments);
		info.UseShellExecute = false;
		info.WorkingDirectory = Path.GetFullPath(workingDir);
		
		Process pro = Process.Start(info);
		pro.WaitForExit();
	}
	
	private static void _puts(string text)
	{
		Console.WriteLine(text);
	}
	
	private static void _mkdir(string path)
	{
		Directory.CreateDirectory(path);
	}
	
	public static void CleanUp()
	{
		Directory.Delete("output", true);
	}
	
	public static void BuildMaster(bool ask)
	{
		if (_ask(ask, "Build Master version"))
		{
			_puts("Configure...");
			_start(
				_cmakeExe,
				new string[] {
					"-DCMAKE_BUILD_TYPE=master",
					"."
				},
				_binaryDir
			);
			
			_puts("Build...");
			_start(
				_cmakeExe,
				new string[] {
					"--build",
					".",
					"--target game"
				},
				_binaryDir
			);			
		}
	}
	
	public static void BuildAssets(bool ask)
	{
		if (_ask(ask, "Build Assets"))
		{
			_puts("Build Assets...");
			_start(
				Path.GetFullPath(Path.Combine(_codeDir, "build_assets.cmd")),
				new string[] {
					"--master"
				},
				_codeDir
			);
		}	
	}
	
	public static void BuildSetup(bool ask)
	{
		if (_ask(ask, "Build Setup"))
		{
			_puts("Build Setup-Scripts...");
			
			_mkdir(@".\output\scripts");
			_mkdir(@".\output\setup");
			
			string contentInstall = "";
			string contentUninstall = "";
			foreach (string file in Directory.GetFiles(_gamebuildDir))
			{
				string fullPath = Path.GetFullPath(file);
			
				contentInstall += String.Format("File \"{0}\"{1}", fullPath, Environment.NewLine);
				contentUninstall += String.Format("Delete \"{0}\"{1}", fullPath, Environment.NewLine);
			}
			File.WriteAllText(@".\output\scripts\install.nsh", contentInstall);
			File.WriteAllText(@".\output\scripts\uninstall.nsh", contentUninstall);
			
			_puts("Build Setup-Executable...");
		
			_start(
				@"..\..\library\buildtools\nsis\makensis.exe",
				new string[] {
					"setup.nsi"
				},
				"scripts"
			);
		}	
	}
	
	--cdrtools\mkisofs.exe -o steam_os2.iso -iso-level 3 -r -R -l -L -J -allow-lowercase -allow-multidot data_folder
	--pause
	
	public static void BuildIso(bool ask)
	{
		if (_ask(ask, "Build ISO"))
		{
			_puts("Build ISO-Image...");
			
			_start(
				@"..\..\library\buildtools\cdrtools\mkisofs.exe",
				new string[] {
					"-o",
					Path.GetFullPath(@"output\eu_image_win.iso"),
					"-l",
					"-hfs",
					Path.GetFullPath(@"configuration\cdcontent"),
					Path.GetFullPath(@"output\setup")
				}
			);
		}	
	}
}
--]]