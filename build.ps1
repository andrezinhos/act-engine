Clear-Host
Write-Host "BUILDING..."

$source = Get-Location
$build = "build"

if (-Not (Test-Path -Path $build)){
	New-Item -Path $build -ItemType Directory | Out-Null
}

Set-Location $build
cmake ..
cmake --build .

if ($LASTEXITCODE -ne 0){
	Write-Host "BUILD ERROR"
	Set-Location $source
	exit 1
}

Write-Host "BUILD FINISHED"
Read-Host "PRESS ENTER TO EXIT"
Set-Location $source
Clear-Host
