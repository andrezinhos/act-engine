Clear-Host
Write-Host "CLEANING RESOURCES"

$build = "build"

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

