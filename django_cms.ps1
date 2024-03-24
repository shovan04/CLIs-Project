param([string]$type)
$path = $(Get-Location)
#Check if type empty
if ($type -eq "") {
    #Activate virtual environment without providing path
    & "$path\env\Scripts\Activate.ps1"
}
#Check if type is equal to -S
elseif ($type -eq '-S') {
    #Run Django development server
    & "$path\env\Scripts\Activate.ps1"
    python "$path\manage.py" runserver
}
elseif ($type -eq '-M') {
    #Migrate Django project
    & "$path\env\Scripts\Activate.ps1"
    python "$path\manage.py" makemigrations
    python "$path\manage.py" migrate
}
elseif ($type -eq '-ST') {
    #Migrate Django project
    & deactivate
}
# elseif ($type -eq '-DOC-build') {
#     #Migrate Django project
#     &  docker build --tag python-django .
# }
else {
    #Show help message
    Write-Host "django_cms [ Starting Command ]`n'-S' [ For Start Server ] ,`n'-M' [ For makemigrations & Migrate ],`n'-ST' [ For Stop ], `n.h [ helps ]"
}
