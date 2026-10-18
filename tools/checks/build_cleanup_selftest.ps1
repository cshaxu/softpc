$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '../..'))
$tool = Join-Path $repo 'tools/Clean-Build.ps1'
$name = "cleanup-selftest-$PID"
$fixture = Join-Path $repo "build/$name"
function Invoke-Cleanup([string[]]$arguments, [bool]$success) {
    $ErrorActionPreference = 'Continue' # Expected child failures produce stderr.
    $output = & powershell -NoProfile -ExecutionPolicy Bypass -File $tool @arguments 2>&1
    if (($LASTEXITCODE -eq 0) -ne $success) { throw "Unexpected cleanup result: $output" }
}
try {
    New-Item -ItemType Directory -Path $fixture | Out-Null
    [IO.File]::WriteAllText((Join-Path $fixture 'sentinel'), 'keep until apply')
    foreach ($unsafe in @('.ninja', '.ccache', '../assets', '.', '...', 'stale.', '.ninja/child')) {
        Invoke-Cleanup @('-Entry', $unsafe, '-Apply', '-Quiescent') $false
    }
    Invoke-Cleanup @('-Entry', $name, '-Apply') $false
    Invoke-Cleanup @('-Check') $false
    Invoke-Cleanup @('-Entry', $name) $true
    if (!(Test-Path (Join-Path $fixture 'sentinel'))) { throw 'Dry run deleted content' }
    $git = Join-Path $fixture '.git'
    New-Item -ItemType Directory -Path $git | Out-Null
    Invoke-Cleanup @('-Entry', $name, '-Apply', '-Quiescent') $false
    [IO.Directory]::Delete($git)
    $link = Join-Path $fixture 'link'
    New-Item -ItemType Junction -Path $link -Target (Join-Path $repo 'assets') | Out-Null
    Invoke-Cleanup @('-Entry', $name, '-Apply', '-Quiescent') $false
    [IO.Directory]::Delete($link) # Delete the junction itself, never its target.
    Invoke-Cleanup @('-Entry', $name, '-Apply', '-Quiescent') $true
    if (Test-Path -LiteralPath $fixture) { throw 'Apply did not delete its exact target' }
    Write-Output 'Build cleanup safety: OK'
} finally {
    if (Test-Path -LiteralPath $fixture) {
        & $tool -Entry $name -Apply -Quiescent
    }
}
