param([string[]]$Entry = @(), [switch]$Apply, [switch]$Quiescent, [switch]$Check)
$ErrorActionPreference = 'Stop'
$repo = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
$build = Join-Path $repo 'build'
$persistent = @('.ccache', '.ninja')
if (!(Test-Path -LiteralPath $build)) { return }
if ((Get-Item -LiteralPath $build -Force).Attributes -band [IO.FileAttributes]::ReparsePoint) {
    throw 'build itself must not be a reparse point'
}
$transient = @(Get-ChildItem -LiteralPath $build -Force | Where-Object { $_.Name -notin $persistent })
if ($Check) {
    if ($transient.Count) { throw "Disposable build entries remain: $($transient.Name -join ', ')" }
    foreach ($name in $persistent) {
        $path = Join-Path $build $name
        if ((Test-Path -LiteralPath $path) -and
            ((Get-Item -LiteralPath $path -Force).Attributes -band [IO.FileAttributes]::ReparsePoint)) {
            throw "Persistent cache must not redirect outside the repository: $name"
        }
    }
    Write-Output 'Build closure residue: OK'
    return
}
if (!$Entry.Count) { $transient | Select-Object Name,Attributes; return }
if ($Apply -and !$Quiescent) { throw 'Stop/join owned jobs before confirming -Quiescent' }
$targets = foreach ($name in $Entry) {
    if ($name -notmatch '^[a-zA-Z0-9_.-]+$' -or $name.EndsWith('.') -or $name -in $persistent) {
        throw "Only exact disposable build-child names are allowed: $name"
    }
    $path = [IO.Path]::GetFullPath((Join-Path $build $name))
    if ([IO.Path]::GetDirectoryName($path) -ne $build) { throw "Unsafe cleanup target: $path" }
    if ($name -eq 'output' -and (Test-Path -LiteralPath (Join-Path $path 'softpc.ini'))) {
        throw 'User configuration must be relocated by its owner, never deleted as cache'
    }
    if (Test-Path -LiteralPath $path) { $path }
}
# Preflight every target before deleting any. Do not traverse links/junctions.
foreach ($path in $targets) {
    $pending = [Collections.Generic.Stack[object]]::new()
    $pending.Push((Get-Item -LiteralPath $path -Force))
    while ($pending.Count) {
        $item = $pending.Pop()
        if ($item.Attributes -band [IO.FileAttributes]::ReparsePoint) { throw "Reparse point: $($item.FullName)" }
        if ($item.Name -eq '.git') { throw "Embedded Git data is not disposable cache: $($item.FullName)" }
        if ($item -is [IO.DirectoryInfo]) {
            foreach ($child in (Get-ChildItem -LiteralPath $item.FullName -Force)) { $pending.Push($child) }
        }
    }
}
foreach ($path in $targets) {
    if ($Apply) {
        Remove-Item -LiteralPath $path -Recurse -Force
        Write-Output "Removed disposable generated work: $path (not recoverable from Git)"
    } else { Write-Output "Would remove: $path" }
}
