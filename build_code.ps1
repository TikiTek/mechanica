$start_time = Get-Date
$genie = Join-Path -Path $PSScriptRoot -ChildPath "library\buildtools\genie\genie.exe"

$solutions = @(
	"library/tools/command_line_converter",
	"library/tools/editor",
	"library/tools/generic_data_code_generator",
	"library/tools/resource_code_generator",
	"local/game"
)

$build_count = 0
$build_success_count = 0
$fail_targets = @()
foreach ($solution in $solutions) {
	Echo "Solution: $($solution)"
	
	try {
		$targets_command_line = "$($genie) --quiet --file=$($solution)/genie.lua --targets_action=vs2019 targets"
		$targets = Invoke-Expression $targets_command_line | Out-String | ConvertFrom-Json

		$generate_command_line = "$($genie) --file=$($solution)/genie.lua --to=build vs2019"
		Invoke-Expression $generate_command_line

		$sln_dir = Join-Path -Path $PSScriptRoot -ChildPath "$($solution)/build"
		$sln_file = "$(Split-Path $solution -Leaf).sln"
		$sln_path = Join-Path -Path $sln_dir -ChildPath $sln_file
	} catch {
		$fail_targets += "$solution configure"
		Echo "failed to configure '$($solution)'"
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