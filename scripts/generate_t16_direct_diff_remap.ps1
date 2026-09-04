[CmdletBinding()]
param(
    [string]$RepositoryRoot = '.',
    [string]$InputPath,
    [string]$OutputPath
)

$ErrorActionPreference = 'Stop'
$repository = (Resolve-Path -LiteralPath $RepositoryRoot).Path
if (-not $InputPath) {
    $InputPath = Join-Path $repository 'docs\etc\evidence\m8-t14-pristine-map.tsv'
}
if (-not $OutputPath) {
    $OutputPath = Join-Path $repository 'docs\etc\evidence\m8-t16-direct-diff-remap.tsv'
}

function Get-CurrentPath([string]$OldPath) {
    if ($OldPath.StartsWith('src/core/softpc/base/')) {
        return 'src/mvdm/softpc.new/base/' + $OldPath.Substring('src/core/softpc/base/'.Length)
    }
    if ($OldPath.StartsWith('src/core/softpc/host/')) {
        return 'src/mvdm/softpc.new/host/' + $OldPath.Substring('src/core/softpc/host/'.Length)
    }
    return $OldPath
}

function Get-InitialOwner([string]$CurrentPath) {
    if ($CurrentPath.StartsWith('src/mvdm/softpc.new/')) { return 'recovered-machine-review' }
    if ($CurrentPath.StartsWith('src/core/softpc-port-abi/')) { return 'compat-review' }
    if ($CurrentPath.StartsWith('src/vm/')) { return 'app-pending-T18' }
    if ($CurrentPath.StartsWith('src/host/')) { return 'host-pending-T17' }
    if ($CurrentPath -match '^src/core/softpc_') { return 'host-pending-T17' }
    return 'unselected-or-review'
}

$rows = @(Import-Csv -LiteralPath $InputPath -Delimiter "`t" |
    Where-Object {
        $_.classification -in @('direct-original-diff', 'local-standalone')
    } |
    Sort-Object path)
if ($rows.Count -ne 98) {
    throw "Expected 98 T14 direct/local rows, found $($rows.Count)."
}

$content = [System.Collections.Generic.List[string]]::new()
$content.Add(('old_path', 'current_path', 'baseline_path', 'classification',
    't14_disposition', 'initial_t16_owner', 'cmake_selection', 'current_exists') -join "`t")
foreach ($row in $rows) {
    $current = Get-CurrentPath $row.path
    $exists = Test-Path -LiteralPath (Join-Path $repository $current) -PathType Leaf
    $content.Add((@(
        $row.path,
        $current,
        $row.baseline_path,
        $row.classification,
        $row.disposition,
        (Get-InitialOwner $current),
        $row.cmake_selection,
        $exists.ToString().ToLowerInvariant()
    ) -join "`t"))
}

if (@($content | Select-Object -Skip 1 | Where-Object { $_.EndsWith("`tfalse") }).Count -ne 0) {
    throw 'A T14 direct/local row has no current repository path.'
}

$outputDirectory = Split-Path -Parent $OutputPath
New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null
[System.IO.File]::WriteAllLines($OutputPath, $content, [System.Text.UTF8Encoding]::new($false))
Write-Host "Wrote $($rows.Count) rows to $OutputPath"
