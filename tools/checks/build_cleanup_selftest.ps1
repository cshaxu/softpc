$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$tool = Join-Path $repo 'tools/Clean-Build.ps1'
$name = "cleanup-selftest-$PID"
$fixture = Join-Path $repo "build/$name"
function Invoke-Cleanup([hashtable]$arguments, [bool]$success) {
    try { & $tool @arguments | Out-Null }
    catch { if ($success) { throw }; return }
    if (!$success) { throw "Unsafe cleanup unexpectedly succeeded: $($arguments.Entry)" }
}
try {
    New-Item -ItemType Directory -Path $fixture | Out-Null
    [IO.File]::WriteAllText((Join-Path $fixture 'sentinel'), 'keep until apply')
    foreach ($unsafe in @('.ninja', '.ccache', '../assets', '.', '...', 'stale.', '.ninja/child')) {
        Invoke-Cleanup @{Entry=$unsafe; Apply=$true; Quiescent=$true} $false
    }
    Invoke-Cleanup @{Entry=$name; Apply=$true} $false
    Invoke-Cleanup @{Check=$true} $false
    Invoke-Cleanup @{Entry=$name} $true
    if (!(Test-Path (Join-Path $fixture 'sentinel'))) { throw 'Dry run deleted content' }
    $git = Join-Path $fixture '.git'
    New-Item -ItemType Directory -Path $git | Out-Null
    Invoke-Cleanup @{Entry=$name; Apply=$true; Quiescent=$true} $false
    [IO.Directory]::Delete($git)
    $link = Join-Path $fixture 'link'
    New-Item -ItemType Junction -Path $link -Target (Join-Path $repo 'assets') | Out-Null
    Invoke-Cleanup @{Entry=$name; Apply=$true; Quiescent=$true} $false
    [IO.Directory]::Delete($link) # Delete the junction itself, never its target.
    Invoke-Cleanup @{Entry=$name; Apply=$true; Quiescent=$true} $true
    if (Test-Path -LiteralPath $fixture) { throw 'Apply did not delete its exact target' }
    Write-Output 'Build cleanup safety: OK'
} finally {
    if (Test-Path -LiteralPath $fixture) {
        & $tool -Entry $name -Apply -Quiescent
    }
}
