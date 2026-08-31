Clear-Host
Write-Host "CLEANING RESOURCES"

$build_mkr = "src/mkr/build"
$build_amk = "src/amk/build"
$build = "build"

if (Test-Path -Path $build_mkr){
	Remove-Item -Path $build_mkr -Recurse -Force
	Write-Host "CLEANED MONKEY RENDER BUILD FOLDER"
} else {
	Write-Host "FOLDER ALREADY CLEAN"
}

if (Test-Path -Path $build_amk){
	Remove-Item -Path $build_amk -Recurse -Force
	Write-Host "CLEANED AUDIO MAKE BUILD FOLDER"
} else {
	Write-Host "FOLDER ALREADY CLEAN"
}

if (Test-Path -Path $build){
	Remove-Item -Path $build -Recurse -Force
	Write-Host "CLEANED BUILD FOLDER"
	Read-Host "PRESS ENTER TO EXIT"
	Clear-Host
} else {
	Write-Host "FOLDER ALREADY CLEAN"
	Read-Host "PRESS ENTER TO EXIT"
	Clear-Host
}
