Clear-Host
Write-Host "BUILDING..."

$source = Get-Location
$build = "build"
$build_mkr = "src/mkr/build"

if (-Not (Test-Path -Path "bin")){
	New-Item -Path "bin" -ItemType Directory | Out-Null
}

# ------------------------------------------
# enter the monkey render dir to build

if (-Not (Test-Path -Path $build_mkr)){
	New-Item -Path $build_mkr -ItemType Directory | Out-Null
}

Set-Location $build_mkr
Write-Host "MONKEY RENDER BUILD"
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build .

if ($LASTEXITCODE -ne 0){
	Write-Host "MONKEY RENDER BUILD ERROR"
	Set-Location $source
	exit 1
}

Write-Host "MONKEY RENDER BUILDED"
Set-Location $source

# return to root dir
# ------------------------------------------

Set-Location $build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
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
