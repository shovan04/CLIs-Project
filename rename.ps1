param (
    [string]$path,
    [string]$addition
)

Get-ChildItem $path | ForEach-Object {
    $newName = $_.BaseName + "-"+$addition+ "-"+ $_.Extension
    Rename-Item $_.FullName -NewName $newName
}
