param ($projectName)
$path = $(Get-Location)  # get the path

if([string]::IsNullOrEmpty($projectName)){
    Write-Host "Use : py_env <Folder Name>"
}else{

    # install the virtualenv
    & pip install virtualenv

    & mkdir $projectName; cd $projectName; New-Item main.py

    # Create virtual environment
    & python -m venv env

    # Activate virtual environment
    & "$path\$projectName\env\Scripts\Activate.ps1"

    # open this project in the VS Code editor
    & code .
}
exit;