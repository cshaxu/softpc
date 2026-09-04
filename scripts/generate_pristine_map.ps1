[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidateNotNullOrEmpty()]
    [string]$OriginalRoot,

    [string]$RepositoryRoot = '.',

    [string]$OutputPath
)

$ErrorActionPreference = 'Stop'

$repository = (Resolve-Path -LiteralPath $RepositoryRoot).Path
$original = (Resolve-Path -LiteralPath $OriginalRoot).Path
$mvdm = (Resolve-Path -LiteralPath (Join-Path $original '..')).Path
if (-not $OutputPath) {
    $OutputPath = Join-Path $repository 'docs\etc\evidence\m8-t14-pristine-map.tsv'
}

$binaryExtensions = @('.obj', '.lib', '.exe', '.dll', '.pdb', '.res')
$sourceExtensions = @('.c', '.h', '.asm', '.inc', '.rc', '.def')
$sourceNames = @('makefile', 'sources', 'c-files')
$cmake = Get-Content -Raw -LiteralPath (Join-Path $repository 'CMakeLists.txt')
$ccpuManifest = Get-Content -LiteralPath (Join-Path $repository 'src\core\softpc\base\ccpu386\c-files') |
    Where-Object { $_ -and -not $_.StartsWith('p.') }

function Get-RelativePath([string]$Root, [string]$Path) {
    return $Path.Substring($Root.Length).TrimStart('\').Replace('\', '/')
}

function Get-Hash([string]$Path) {
    return (Get-FileHash -LiteralPath $Path -Algorithm SHA256).Hash.ToLowerInvariant()
}

function Get-CanonicalHash([string]$Path) {
    $latin1 = [System.Text.Encoding]::GetEncoding(28591)
    $text = $latin1.GetString([System.IO.File]::ReadAllBytes($Path))
    $normal = $text.Replace("`r", '').TrimEnd("`n")
    $bytes = $latin1.GetBytes($normal)
    $sha = [System.Security.Cryptography.SHA256]::Create()
    try {
        return ([System.BitConverter]::ToString($sha.ComputeHash($bytes))).Replace('-', '').ToLowerInvariant()
    }
    finally {
        $sha.Dispose()
    }
}

function Get-FileKind([string]$RelativePath) {
    $name = [System.IO.Path]::GetFileName($RelativePath).ToLowerInvariant()
    $extension = [System.IO.Path]::GetExtension($RelativePath).ToLowerInvariant()
    if ($binaryExtensions -contains $extension) { return 'binary-output' }
    if ($sourceExtensions -contains $extension -or $sourceNames -contains $name) { return 'source-or-header' }
    return 'resource-or-build-input'
}

function Get-BaselinePath([string]$CurrentRelative) {
    if ($CurrentRelative.StartsWith('src/core/softpc/')) {
        $relative = $CurrentRelative.Substring('src/core/softpc/'.Length)
        if ($relative.StartsWith('xms.486/')) {
            $name = $relative.Substring('xms.486/'.Length)
            if ($name -in @('memapi.h', 'xmssvc.h')) { return "../inc/$name" }
            return "../xms.486/$name"
        }
        if ($relative -eq 'suballoc/suballoc.h') { return '../inc/suballoc.h' }
        if ($relative.StartsWith('suballoc/')) { return "../$relative" }
        return $relative
    }
    if ($CurrentRelative.StartsWith('src/mvdm/softpc.new/roms/')) {
        return $CurrentRelative.Substring('src/mvdm/softpc.new/'.Length)
    }
    return $null
}

function Get-BaselineFile([string]$BaselineRelative) {
    if (-not $BaselineRelative) { return $null }
    return Join-Path $original $BaselineRelative.Replace('/', '\\')
}

function Get-CMakeSelection([string]$CurrentRelative) {
    if (-not $CurrentRelative.StartsWith('src/')) { return 'not-applicable' }
    $needle = $CurrentRelative.Replace('/', '\\')
    if ($cmake.Contains($CurrentRelative)) { return 'literal-reference' }
    if ($CurrentRelative.StartsWith('src/core/softpc/base/ccpu386/')) {
        $name = [System.IO.Path]::GetFileName($CurrentRelative)
        if ($ccpuManifest -contains $name) { return 'ccpu-manifest' }
    }
    if ($CurrentRelative -match '^src/core/softpc/base/cvidc/(sinit|sevid).*\.c$') {
        return 'cvid-glob'
    }
    return 'not-directly-selected'
}

function Get-Disposition([string]$CurrentRelative, [string]$BaselineState, [string]$Kind) {
    if ($Kind -eq 'binary-output') { return 'exclude-binary-output' }
    if ($BaselineState -in @('identical-original', 'normalized-identical-original')) {
        return 'retain-pristine'
    }
    if ($BaselineState -eq 'original-omitted') { return 'classify-before-T15' }
    if ($CurrentRelative.StartsWith('src/overlay/mvdm/softpc.new/')) { return 'overlay' }
    if ($CurrentRelative.StartsWith('src/app/')) { return 'app' }
    if ($CurrentRelative.StartsWith('src/host/')) { return 'host' }
    if ($CurrentRelative -match '^src/core/softpc_xms_host\.c$') { return 'remove-unselected' }
    if ($CurrentRelative.StartsWith('src/core/softpc/host/')) { return 'host' }
    if ($CurrentRelative -match '^src/core/softpc_(standalone|gfi_image|device_bop|machine)') { return 'host' }
    if ($CurrentRelative.StartsWith('src/core/softpc/base/')) { return 'overlay' }
    if ($CurrentRelative -match '^src/core/softpc/(xms\.486|suballoc)/') { return 'remove-unselected' }
    if ($BaselineState -eq 'local-standalone') { return 'host-or-app-review' }
    return 'overlay'
}

$currentFiles = @{}
Get-ChildItem -LiteralPath (Join-Path $repository 'src') -Recurse -File |
    ForEach-Object {
        $relative = Get-RelativePath (Join-Path $repository 'src') $_.FullName
        $currentFiles["src/$relative"] = $_.FullName
    }

$baselineFiles = @{}
Get-ChildItem -LiteralPath $original -Recurse -File |
    ForEach-Object { $baselineFiles[(Get-RelativePath $original $_.FullName)] = $_.FullName }
foreach ($extensionRoot in @('xms.486', 'suballoc')) {
    $path = Join-Path $mvdm $extensionRoot
    if (Test-Path -LiteralPath $path) {
        Get-ChildItem -LiteralPath $path -Recurse -File |
            ForEach-Object { $baselineFiles["../$extensionRoot/$(Get-RelativePath $path $_.FullName)"] = $_.FullName }
    }
}
foreach ($header in @('memapi.h', 'xmssvc.h', 'suballoc.h')) {
    $path = Join-Path $mvdm "inc\\$header"
    if (Test-Path -LiteralPath $path) { $baselineFiles["../inc/$header"] = $path }
}

$rows = [System.Collections.Generic.List[object]]::new()
$seenBaseline = [System.Collections.Generic.HashSet[string]]::new([System.StringComparer]::OrdinalIgnoreCase)
foreach ($currentRelative in ($currentFiles.Keys | Sort-Object)) {
    $currentPath = $currentFiles[$currentRelative]
    $baselineRelative = Get-BaselinePath $currentRelative
    $baselinePath = Get-BaselineFile $baselineRelative
    $kind = Get-FileKind $currentRelative
    $state = 'local-standalone'
    $currentHash = Get-Hash $currentPath
    $baselineHash = ''
    $canonicalHash = ''
    $canonicalBaselineHash = ''
    if ($baselinePath -and (Test-Path -LiteralPath $baselinePath)) {
        $null = $seenBaseline.Add($baselineRelative)
        $baselineHash = Get-Hash $baselinePath
        $canonicalHash = Get-CanonicalHash $currentPath
        $canonicalBaselineHash = Get-CanonicalHash $baselinePath
        if ($currentHash -eq $baselineHash) {
            $state = 'identical-original'
        }
        elseif ($canonicalHash -eq $canonicalBaselineHash) {
            $state = 'normalized-identical-original'
        }
        else {
            $state = 'direct-original-diff'
        }
    }
    $rows.Add([PSCustomObject]@{
        Path = $currentRelative
        BaselinePath = if ($baselineRelative) { $baselineRelative } else { '-' }
        Kind = $kind
        Classification = $state
        CMakeSelection = Get-CMakeSelection $currentRelative
        CurrentSha256 = $currentHash
        BaselineSha256 = $baselineHash
        CanonicalCurrentSha256 = $canonicalHash
        CanonicalBaselineSha256 = $canonicalBaselineHash
        Disposition = Get-Disposition $currentRelative $state $kind
    })
}
foreach ($baselineRelative in ($baselineFiles.Keys | Sort-Object)) {
    if ($seenBaseline.Contains($baselineRelative)) { continue }
    $baselinePath = $baselineFiles[$baselineRelative]
    $kind = Get-FileKind $baselineRelative
    if ($kind -eq 'binary-output') {
        $state = 'excluded-binary-output'
    }
    else {
        $state = 'original-omitted'
    }
    $rows.Add([PSCustomObject]@{
        Path = '-'
        BaselinePath = $baselineRelative
        Kind = $kind
        Classification = $state
        CMakeSelection = 'not-selected'
        CurrentSha256 = ''
        BaselineSha256 = Get-Hash $baselinePath
        CanonicalCurrentSha256 = ''
        CanonicalBaselineSha256 = ''
        Disposition = Get-Disposition '' $state $kind
    })
}

$outputDirectory = Split-Path -Parent $OutputPath
New-Item -ItemType Directory -Force -Path $outputDirectory | Out-Null
$header = @(
    'path', 'baseline_path', 'kind', 'classification', 'cmake_selection',
    'current_sha256', 'baseline_sha256', 'canonical_current_sha256',
    'canonical_baseline_sha256',
    'disposition'
) -join "`t"
$content = [System.Collections.Generic.List[string]]::new()
$content.Add($header)
foreach ($row in $rows) {
    $content.Add((@($row.Path, $row.BaselinePath, $row.Kind, $row.Classification,
        $row.CMakeSelection, $row.CurrentSha256, $row.BaselineSha256,
        $row.CanonicalCurrentSha256, $row.CanonicalBaselineSha256,
        $row.Disposition) -join "`t"))
}
[System.IO.File]::WriteAllLines($OutputPath, $content, [System.Text.UTF8Encoding]::new($false))
Write-Host "Wrote $($rows.Count) rows to $OutputPath"
