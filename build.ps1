Clear-Host
Write-Host "BUILDING..."

$source = Get-Location
$build = "build"

# enter the monkey render dir to build
$build_mkr = "src/mkr/build"
if (-Not (Test-Path -Path "bin")){
	New-Item -Path "bin" -ItemType Directory | Out-Null
}

if (-Not (Test-Path -Path $build)){
	New-Item -Path $build -ItemType Directory | Out-Null
}
Set-Location $build_mkr
Write-Host "MONKEY RENDER BUILD"
cmake ..
cmake --build .

if ($LASTEXITCODE -ne 0){
	Write-Host "MONKEY RENDER BUILD ERROR"
	Set-Location $source
	exit 1
}

Write-Host "MONKEY RENDER BUILDED"
Set-Location $source
# return to root dir

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
