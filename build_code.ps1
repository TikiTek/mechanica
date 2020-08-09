$start_time = Get-Date
$premake = Join-Path -Path $PSScriptRoot -ChildPath "engine\buildtools\tiki_build\premake5.exe"

$solutions = @(
	"engine/tools/command_line_converter",
	"engine/tools/editor",
	"engine/tools/generic_data_code_generator",
	"engine/tools/resource_code_generator",
	"local/game"
)

$build_count = 0
$build_success_count = 0
$fail_targets = @()
foreach ($solution in $solutions) {
	Echo "Solution: $($solution)"
	
	try {
		$targets_command_line = "$($premake) --quiet --file=$($solution)/premake5.lua --targets_action=vs2019 targets"
		$targets_json = Invoke-Expression $targets_command_line | Out-String
		$targets = $targets_json | ConvertFrom-Json

		$generate_command_line = "$($premake) --file=$($solution)/premake5.lua --to=build vs2019"
		Invoke-Expression $generate_command_line

		$sln_dir = Join-Path -Path $PSScriptRoot -ChildPath "$($solution)/build"
		$sln_file = "$(Split-Path $solution -Leaf).sln"
		$sln_path = Join-Path -Path $sln_dir -ChildPath $sln_file
	} catch {
		$fail_targets += "$solution configure"
		Echo "failed to configure '$($solution)'"
		Echo $targets_json
		Echo $_
		continue
	}
		
	foreach ($target in $targets) {
		Echo "Building $($sln_file) ($($target.config)|$($target.platform))..."
		$build_command_line = "msbuild $($sln_path) -p:Configuration=$($target.config) -p:Platform=$($target.platform)"
		Invoke-Expression $build_command_line
		
		if($LASTEXITCODE -ne 0) {
			$fail_targets += "$($sln_file) ($($target.config)|$($target.platform))"
			Echo "failed to build '$($solution)' in configuration '$($target.config)' and for platform '$($target.platform)'"
		}
		else
		{
			$build_success_count += 1
		}
		
		$build_count += 1
	}
}

if ($fail_targets.Count -gt 0) {
	Echo "Failed targets:"
	Echo $fail_targets
}

$build_time = New-Timespan -Start $start_time -End (Get-Date)
$result = "Code Build done after {0:c}. $build_success_count of $build_count builds succeded." -f $build_time
echo $result

if ($fail_targets.Count -gt 0) {
	exit 1
}