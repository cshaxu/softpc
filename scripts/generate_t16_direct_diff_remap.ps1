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
    'src/core/softpc/base/ccpu386/softpc_ccpu_facade.c' = 'src/host/compat/ccpu/facade.c'
    'src/core/softpc-port-abi/ccpu/softpc_ccpu_lifecycle.c' = 'src/host/compat/ccpu/lifecycle.c'
    'src/core/softpc-port-abi/ccpu/softpc_ccpu_lifecycle.h' = 'src/host/compat/ccpu/lifecycle.h'
    'src/core/softpc-port-abi/cmos/softpc_cmos_host.h' = 'src/host/compat/cmos/port.h'
    'src/core/softpc-port-abi/cvidc/softpc_gdp_rule_access.h' = 'src/host/compat/cvidc/gdp_rule_access.h'
    'src/core/softpc-port-abi/cvidc/softpc_gdp_slots.h' = 'src/host/compat/cvidc/gdp_slots.h'
    'src/core/softpc-port-abi/cvidc/softpc_gdp_state.c' = 'src/host/compat/cvidc/gdp_state.c'
    'src/core/softpc-port-abi/cvidc/softpc_gdp_state.h' = 'src/host/compat/cvidc/gdp_state.h'
    'src/core/softpc-port-abi/illegalp/error.h' = 'src/host/compat/system/error.h'
    'src/core/softpc-port-abi/keyba/cpu4.h' = 'src/host/compat/keymouse/cpu4.h'
    'src/core/softpc-port-abi/reset/host_def.h' = 'src/host/compat/bios/host_def.h'
    'src/core/softpc/host/inc/softpc_standalone_dib.h' = 'src/host/dib_surface.h'
    'src/core/softpc/host/inc/x86/prod/gdpvar.h' = 'src/host/compat/ccpu/legacy/gdpvar.h'
    'src/core/softpc/host/inc/x86/prod/PigReg_c.h' = 'src/host/compat/ccpu/legacy/PigReg_c.h'
    'src/core/softpc/host/inc/x86/prod/sas4gen.h' = 'src/host/compat/ccpu/legacy/sas4gen.h'
    'src/core/softpc/host/src/nt_com.c' = 'src/host/comms/serial.c'
    'src/core/softpc/host/src/nt_lpt.c' = 'src/host/comms/parallel.c'
    'src/core/softpc_device_bop.c' = 'src/host/device_bop.c'
    'src/core/softpc_gfi_image.c' = 'src/host/gfi_image.c'
    'src/core/softpc_machine.c' = 'src/host/machine.c'
    'src/core/softpc_machine.h' = 'src/host/machine.h'
    'src/core/softpc_standalone_dib.c' = 'src/host/dib_surface.c'
    'src/core/softpc_standalone_platform.c' = 'src/host/platform.c'
    'src/host/softpc_compat/conapi.h' = 'src/host/compat/conapi.h'
    'src/host/softpc_compat/edl_fast_bop.c' = 'src/host/compat/edl_fast_bop.c'
    'src/host/softpc_compat/graphics_console_compat.c' = 'src/host/compat/graphics_console_compat.c'
    'src/host/softpc_compat/softpc_standalone_dib.h' = 'src/host/dib_surface.h'
    'src/host/softpc_compat/softpc_host_input.h' = 'src/host/input.h'
    'src/vm/main.c' = 'src/app/main.c'
    'src/vm/softpc_firmware.rc' = 'src/app/firmware.rc'
    'src/vm/runtime.c' = 'src/app/runtime.c'
    'src/vm/runtime.h' = 'src/app/runtime.h'
    'src/vm/console.c' = 'src/app/console.c'
    'src/vm/console.h' = 'src/app/console.h'
    'src/vm/win32_keyboard.c' = 'src/app/keyboard.c'
    'src/vm/win32_keyboard.h' = 'src/app/keyboard.h'
    'src/vm/win32_window.c' = 'src/app/window.c'
    'src/vm/win32_window.h' = 'src/app/window.h'
}

# T19 removes these explicitly unselected NTVDM XMS/suballocation remnants.
# Keep their historical ledger rows, but make their deliberate absence visible.
$RemovedPaths = @{
    'src/core/softpc_xms_host.c' = $true
    'src/core/softpc/suballoc/suballcp.h' = $true
    'src/core/softpc/xms.486/xms.h' = $true
    'src/core/softpc/xms.486/xmsa20.c' = $true
    'src/core/softpc/xms.486/xmsblock.c' = $true
    'src/core/softpc-port-abi/xms/mvdm.h' = $true
    'src/core/softpc-port-abi/xms/softpc.h' = $true
}

function Get-CurrentPath([string]$OldPath) {
    if ($RemovedPaths.ContainsKey($OldPath)) { return '-' }
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
    if ($CurrentPath -eq '-') { return 'removed-unselected' }
    if ($CurrentPath.StartsWith('src/mvdm/softpc.new/')) { return 'recovered-machine-review' }
    if ($CurrentPath.StartsWith('src/overlay/mvdm/softpc.new/')) { return 'overlay' }
    if ($CurrentPath.StartsWith('src/app/')) { return 'app' }
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
    $exists = $current -ne '-' -and
        (Test-Path -LiteralPath (Join-Path $repository $current) -PathType Leaf)
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

$missingRows = @($content | Select-Object -Skip 1 | Where-Object {
    $_.EndsWith("`tfalse") -and $_ -notmatch "`tremoved-unselected`t"
})
if ($missingRows.Count -ne 0) {
    throw "A T14 direct/local row has no current repository path: $($missingRows -join '; ')"
}

$outputDirectory = Split-Path -Parent $OutputPath
New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null
[System.IO.File]::WriteAllLines($OutputPath, $content, [System.Text.UTF8Encoding]::new($false))
Write-Host "Wrote $($rows.Count) rows to $OutputPath"
