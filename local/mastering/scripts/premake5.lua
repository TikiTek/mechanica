-- local/mastering/premake.lua

local currentDir = os.getcwd();
local baseDir = "../../..";
local converterDir = path.join(baseDir, "library/tools/commandlineconverter");
local gameDir = path.join(baseDir, "local/game");
local gamebuildDir = path.join(baseDir, "gamebuild");

function getOutputDir()
	return path.getabsolute('output', '.');
end
	
function cleanUp()
	os.rmdir(getOutputDir());
end

function buildProject(projectPath)
	print('Building ' .. path.getname(projectPath) .. '...');
	os.chdir(projectPath);
	os.execute('create_visualstudio.cmd');
	create_project = path.getabsolute(path.join(projectPath, 'create_visualstudio.cmd'), currentDir)
	print(create_project);
end

function buildGame()
	buildProject(gameDir);
end

function buildAssets()
	buildProject(converterDir);

	print('Building assets...');
	
end

function buildSetup()
	
end

function buildIso()
	
end

newaction {
    trigger = "master",
    description = "Generates an final Version",

    onStart = function()        
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