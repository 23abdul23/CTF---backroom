$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$server = Join-Path $root 'build\server_app.exe'
$client = Join-Path $root 'build\client_app.exe'

if (-not (Test-Path $server)) {
    $server = Join-Path $root 'build\server_app'
}
if (-not (Test-Path $client)) {
    $client = Join-Path $root 'build\client_app'
}

if (-not (Test-Path $server)) {
    throw "Server binary not found at $server"
}
if (-not (Test-Path $client)) {
    throw "Client binary not found at $client"
}

$serverJob = Start-Job -ScriptBlock {
    param($exe, $cwd)
    Set-Location $cwd
    & $exe
} -ArgumentList $server, $root

Start-Sleep -Milliseconds 1000

$clientJobs = @()
for ($i = 0; $i -lt 4; $i++) {
    $clientJobs += Start-Job -ScriptBlock {
        param($exe, $cwd)
        Set-Location $cwd
        & $exe 127.0.0.1
    } -ArgumentList $client, $root
    Start-Sleep -Milliseconds 400
}

Write-Host "Started server job $($serverJob.Id) and $($clientJobs.Count) client jobs on localhost."
Write-Host "Use Get-Job or Stop-Job to manage them."
