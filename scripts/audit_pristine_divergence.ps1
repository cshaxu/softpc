[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateNotNullOrEmpty()]
    [string]$OriginalRoot,

    [string]$CurrentRoot = (Join-Path $PSScriptRoot '..\src\core\softpc')
)

$ErrorActionPreference = 'Stop'

$original = (Resolve-Path -LiteralPath $OriginalRoot).Path
$current = (Resolve-Path -LiteralPath $CurrentRoot).Path
$extensions = @('.c', '.h')
$records = [System.Collections.Generic.List[object]]::new()

# M5 owns these routes explicitly.  Keep the audit fail-closed: a newly
# divergent input must be given an owner rather than inheriting a broad family
# default that could hide a controller or product-policy branch.
$portAbiPaths = @(
    'base\ccpu386\evid_c.h', 'base\ccpu386\ntthread.c',
    'base\ccpu386\softpc_ccpu_facade.c',
    'base\cvidc\evidfunc.h', 'base\cvidc\evidgen.h',
    'base\cvidc\j_c_lang.c', 'base\cvidc\j_c_lang.h',
    'base\cvidc\sascdef.c', 'base\cvidc\sevid019.c',
    'base\cvidc\sevid020.c', 'base\cvidc\vglfunc.c',
    'base\inc\ckmalloc.h', 'base\inc\cpu_vid.h',
    'base\inc\egacpu.h', 'base\inc\emm.h', 'base\inc\gmi.h',
    'base\inc\host_com.h', 'base\inc\host.h', 'base\inc\ica.h',
    'base\inc\ios.h', 'base\inc\timeval.h', 'base\inc\video.h',
    'base\comms\com.c', 'base\keymouse\keybd_io.c',
    'base\keymouse\mouse_io.c', 'base\support\time_day.c',
    'base\system\cmosnt.c', 'base\system\idetect.c',
    'base\system\ica.c', 'base\system\rom.c', 'base\support\main.c',
    'base\video\gfx_updt.c', 'host\inc\x86\prod\gdpvar.h',
    'host\inc\x86\prod\PigReg_c.h', 'host\inc\x86\prod\sas4gen.h'
)
$compatHostPaths = @(
    'base\support\ios.c', 'host\inc\cfpu_def.h',
    'host\inc\host_cpu.h', 'host\inc\host_def.h',
    'host\inc\host_emm.h', 'host\inc\insignia.h',
    'host\inc\nt_event.h', 'host\inc\nt_graph.h',
    'host\inc\nt_inthk.h', 'host\inc\softpc_standalone_dib.h',
    'host\src\nt_cga.c', 'host\src\nt_com.c', 'host\src\nt_ega.c',
    'host\src\nt_graph.c', 'host\src\nt_keycd.c', 'host\src\nt_lpt.c',
    'host\src\nt_munge.c', 'host\src\nt_sound.c', 'host\src\nt_vga.c'
)
$restorePristinePaths = @(
    'base\bios\reset.c', 'base\keymouse\keyba.c',
    'base\keymouse\ppi.c', 'base\system\cmos.c',
    'base\system\timer.c', 'base\system\illegalp.c'
)

function Get-Family([string]$RelativePath) {
    if ($RelativePath.StartsWith('base\ccpu386\')) { return 'CCPU' }
    if ($RelativePath.StartsWith('base\cvidc\')) { return 'C-VID' }
    if ($RelativePath.StartsWith('host\')) { return 'original host contract' }
    if ($RelativePath.StartsWith('base\bios\')) { return 'BIOS / firmware' }
    if ($RelativePath.StartsWith('base\video\')) { return 'video controller' }
    if ($RelativePath.StartsWith('base\disks\')) { return 'disk controller' }
    if ($RelativePath.StartsWith('base\keymouse\')) { return 'input controller' }
    if ($RelativePath.StartsWith('base\comms\')) { return 'communications controller' }
    if ($RelativePath.StartsWith('base\system\')) { return 'machine system' }
    if ($RelativePath.StartsWith('base\support\')) { return 'machine support' }
    return 'machine declarations'
}

function Get-NextTask([string]$Family, [bool]$HasOriginalPeer) {
    if (-not $HasOriginalPeer) { return 'M1 T2 / M2 placement review' }
    switch ($Family) {
        'CCPU' { return 'M1 T1 S2, then M1 T2' }
        'C-VID' { return 'M1 T1 S2, then M1 T2' }
        'original host contract' { return 'M2 host compatibility' }
        default { return 'M1 T1 S2' }
    }
}

function Get-Disposition([string]$RelativePath, [bool]$HasOriginalPeer) {
    if ($portAbiPaths -contains $RelativePath) { return 'port-abi-overlay' }
    if ($compatHostPaths -contains $RelativePath) { return 'compat-host' }
    if ($restorePristinePaths -contains $RelativePath) {
        return 'restore-pristine'
    }
    throw "M5 ledger has no extraction route for $RelativePath"
}

function Get-CanonicalSource([string]$Path) {
    # The selected historical tree is CRLF while the working tree may be LF.
    # Source ownership is a text question, so do not report line-ending or
    # terminal-blank normalization as a machine divergence.
    $text = [System.Text.Encoding]::Latin1.GetString(
        [System.IO.File]::ReadAllBytes($Path)
    )
    return $text.Replace("`r", '').TrimEnd("`n")
}

Get-ChildItem -LiteralPath $current -Recurse -File |
    Where-Object { $_.Extension.ToLowerInvariant() -in $extensions } |
    Sort-Object FullName |
    ForEach-Object {
        $relative = $_.FullName.Substring($current.Length).TrimStart('\\')
        $originalPath = Join-Path $original $relative
        $hasPeer = Test-Path -LiteralPath $originalPath
        $state = 'no-original-peer'

        if ($hasPeer) {
            if ((Get-CanonicalSource $_.FullName) -ceq
                (Get-CanonicalSource $originalPath)) {
                return
            }
            $state = 'divergent-original-peer'
        }

        $markers = @(
            Select-String -LiteralPath $_.FullName `
                -Pattern 'SOFTPC_STANDALONE|softpc_standalone|runner_pacer|softpc_device_bop|softpc_gdp' `
                -AllMatches
        ).Count
        $family = Get-Family $relative
        $records.Add([PSCustomObject]@{
            Path = $relative.Replace('\\', '/')
            State = $state
            Family = $family
            Markers = $markers
            Disposition = Get-Disposition $relative $hasPeer
            NextTask = Get-NextTask $family $hasPeer
        })
    }

"# Pristine Divergence Manifest"
''
"Generated by `scripts/audit_pristine_divergence.ps1` from $(Split-Path -Leaf $original); it does not modify either tree."
''
'| Path | Baseline state | Family | Standalone markers | Disposition | Next task |'
'| --- | --- | --- | ---: | --- | --- |'
foreach ($record in $records) {
    "| ``$($record.Path)`` | $($record.State) | $($record.Family) | $($record.Markers) | $($record.Disposition) | $($record.NextTask) |"
}
''
"Total: $($records.Count)"
