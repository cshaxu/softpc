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

$RelocatedPaths = @{
    'src/core/softpc/base/ccpu386/softpc_ccpu_facade.c' = 'src/core/softpc-port-abi/ccpu/softpc_ccpu_facade.c'
    'src/core/softpc/host/inc/softpc_standalone_dib.h' = 'src/host/compat/softpc_standalone_dib.h'
    'src/core/softpc/host/inc/x86/prod/gdpvar.h' = 'src/core/softpc-port-abi/ccpu/gdpvar.h'
    'src/core/softpc/host/inc/x86/prod/PigReg_c.h' = 'src/core/softpc-port-abi/ccpu/PigReg_c.h'
    'src/core/softpc/host/inc/x86/prod/sas4gen.h' = 'src/core/softpc-port-abi/ccpu/sas4gen.h'
    'src/core/softpc_device_bop.c' = 'src/host/machine/softpc_device_bop.c'
    'src/core/softpc_gfi_image.c' = 'src/host/media/softpc_gfi_image.c'
    'src/core/softpc_machine.c' = 'src/host/machine/softpc_machine.c'
    'src/core/softpc_machine.h' = 'src/host/machine/softpc_machine.h'
    'src/core/softpc_standalone_dib.c' = 'src/host/video/softpc_standalone_dib.c'
    'src/core/softpc_standalone_platform.c' = 'src/host/platform/softpc_standalone_platform.c'
    'src/core/softpc_xms_host.c' = 'src/host/compat/softpc_xms_host.c'
    'src/host/softpc_compat/conapi.h' = 'src/host/compat/conapi.h'
    'src/host/softpc_compat/edl_fast_bop.c' = 'src/host/compat/edl_fast_bop.c'
    'src/host/softpc_compat/graphics_console_compat.c' = 'src/host/compat/graphics_console_compat.c'
    'src/host/softpc_compat/softpc_standalone_dib.h' = 'src/host/compat/softpc_standalone_dib.h'
    'src/host/softpc_compat/softpc_host_input.h' = 'src/host/input/softpc_host_input.h'
}

function Get-CurrentPath([string]$OldPath) {
    if ($RelocatedPaths.ContainsKey($OldPath)) {
        return $RelocatedPaths[$OldPath]
    }
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

$missingRows = @($content | Select-Object -Skip 1 | Where-Object { $_.EndsWith("`tfalse") })
if ($missingRows.Count -ne 0) {
    throw "A T14 direct/local row has no current repository path: $($missingRows -join '; ')"
}

$outputDirectory = Split-Path -Parent $OutputPath
New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null
[System.IO.File]::WriteAllLines($OutputPath, $content, [System.Text.UTF8Encoding]::new($false))
Write-Host "Wrote $($rows.Count) rows to $OutputPath"
