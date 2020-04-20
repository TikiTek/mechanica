$start_time = Get-Date
$asset_converter = Join-Path -Path $PSScriptRoot -ChildPath "library\tools\command_line_converter\build\x64\Release\command_line_converter.exe"

Echo "Building Assets..."
Invoke-Expression $asset_converter

$failed = $false
if($LASTEXITCODE -ne 0) {
	Echo "Failed to Assets"
	$failed = $true
}

$build_time = New-Timespan -Start $start_time -End (Get-Date)
$result = "Asset Build done after {0:c}." -f $build_time
echo $result

if ($failed) {
	exit 1
}
