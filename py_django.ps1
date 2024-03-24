param ($projectName)
$path = $(Get-Location)  # get the path

if([string]::IsNullOrEmpty($projectName)){
    Write-Host "Use : py_djnago [ File Name ]"
}else{
    # install the virtualenv
    & pip install virtualenv

    & mkdir $projectName; cd $projectName

    # Create virtual environment
    & python -m venv env

    # Activate virtual environment
    & "$path\$projectName\env\Scripts\Activate.ps1"

	& python.exe -m pip install --upgrade pip
    # Install Django 
    & pip install django

    # Create Django project
    & django-admin startproject $projectName .

    # add requirements.txt
    & pip freeze > requirements.txt

    #& code .;cls ;

}