Clear-Host
Write-Host "BUILDING..."

$source = Get-Location
$build = "build"

$build_mkr = "src/mkr/build"
$lib_mkr = "src/mkr/lib"

$build_amk = "src/amk/build"
$lib_amk = "src/amk/lib"

if (-Not (Test-Path -Path "bin")){
	New-Item -Path "bin" -ItemType Directory | Out-Null
}

# ------------------------------------------
# enter the monkey render dir to build

if (-Not (Test-Path -Path $build_mkr)){
	New-Item -Path $build_mkr -ItemType Directory | Out-Null
}

if (-Not (Test-Path -Path $lib_mkr)){
	New-Item -Path $lib_mkr -ItemType Directory | Out-Null
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

Clear-Host
Set-Location $source

# return to root
# ------------------------------------------


# ------------------------------------------
# enter the audio make dir to build

if (-Not (Test-Path -Path $build_amk)){
	New-Item -Path $build_amk -ItemType Directory | Out-Null
}

if (-Not (Test-Path -Path $lib_amk)){
	New-Item -Path $lib_amk -ItemType Directory | Out-Null
}

Set-Location $build_amk
Write-Host "AUDIO MAKE BUILD"
cmake ..
cmake --build .

if ($LASTEXITCODE -ne 0){
	Write-Host "AUDIO MAKE BUILD ERROR"
	Set-Location $source
	exit 1
}

Clear-Host
Set-Location $source

# return to root
# ------------------------------------------

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
