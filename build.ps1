Clear-Host
Write-Host "BUILDING..."

$source = Get-Location
$build = "build"

$build_mkr = "mkr/build"
$lib_mkr = "mkr/lib"

$build_pwra = "pwra/build"
$lib_pwra = "pwra/lib"

if (-Not (Test-Path -Path "bin")){
	New-Item -Path "bin" -ItemType Directory | Out-Null
}

if (-Not (Test-Path -Path $build)){
    New-Item -Path $build -ItemType Directory | Out-Null
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
cmake .. | Out-Null
cmake --build . -- -j2

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

if (-Not (Test-Path -Path $build_pwra)){
	New-Item -Path $build_pwra -ItemType Directory | Out-Null
}

if (-Not (Test-Path -Path $lib_pwra)){
	New-Item -Path $lib_pwra -ItemType Directory | Out-Null
}

Set-Location $build_pwra
Write-Host "POWER AUDIO BUILD"
cmake .. | Out-Null
cmake --build . -- -j2

if ($LASTEXITCODE -ne 0){
	Write-Host "POWER AUDIO BUILD ERROR"
	Set-Location $source
	exit 1
}

Clear-Host
Set-Location $source

# return to root
# ------------------------------------------

Set-Location $build
cmake ..
cmake --build . -- -j2

if ($LASTEXITCODE -ne 0){
	Write-Host "BUILD ERROR"
	Set-Location $source
	exit 1
}

Write-Host "BUILD FINISHED"
Read-Host "PRESS ENTER TO EXIT"
Set-Location $source
Clear-Host
