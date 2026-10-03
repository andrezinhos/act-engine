Clear-Host
Write-Host "BUILDING..."

$source = Get-Location
$build = "build"

$build_mkr = "mkr/build"
$lib_mkr = "mkr/lib"

$build_amk = "pwra/build"
$lib_amk = "pwra/lib"

if (-Not (Test-Path -Path "bin")){
	New-Item -Path "bin" -ItemType Directory | Out-Null
}

if (-Not (Test-Path -Path $build)){
	New-Item -Path $build -ItemType Directory | Out-Null
}

# ------------------------------------------
# enter the monkey render dir

if (-Not (Test-Path -Path $build_mkr)){
	New-Item -Path $build_mkr -ItemType Directory | Out-Null
}

if (-Not (Test-Path -Path $lib_mkr)){
	New-Item -Path $lib_mkr -ItemType Directory | Out-Null
}

Set-Location $build_mkr
Write-Host "MONKEY RENDER"
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if ($LASTEXITCODE -ne 0){
	Write-Host "MONKEY RENDER ERROR"
	Set-Location $source
	exit 1
}

Clear-Host
Set-Location $source

# return to root dir
# ------------------------------------------

# ------------------------------------------
# enter the power audio dir

if (-Not (Test-Path -Path $build_amk)){
	New-Item -Path $build_amk -ItemType Directory | Out-Null
}

if (-Not (Test-Path -Path $lib_amk)){
	New-Item -Path $lib_amk -ItemType Directory | Out-Null
}

Set-Location $build_amk
Write-Host "POWER AUDIO"
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if ($LASTEXITCODE -ne 0){
	Write-Host "POWER AUDIO ERROR"
	Set-Location $source
	exit 1
}

Clear-Host
Set-Location $source

# return to root
# ------------------------------------------

Set-Location $build
cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if ($LASTEXITCODE -ne 0){
	Write-Host "BUILD ERROR"
	Set-Location $source
	exit 1
}

Write-Host "BUILD FINISHED"
Read-Host "PRESS ENTER TO EXIT"
Set-Location $source
Clear-Host
