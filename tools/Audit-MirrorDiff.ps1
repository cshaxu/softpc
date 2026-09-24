param(
    [Parameter(Mandatory = $true)][string]$OriginalRoot,
    [string]$RepositoryRoot = (Split-Path $PSScriptRoot -Parent)
)

# Optional read-only research tool. Never a build/test dependency or source generator.
$ErrorActionPreference = 'Stop'
$RepositoryRoot = (Resolve-Path -LiteralPath $RepositoryRoot).Path
$OriginalRoot = (Resolve-Path -LiteralPath $OriginalRoot).Path
$prefix = 'src/app-softpc/softpc.new/'
# Research-only inverse comparison in memory: proves the enumerated rule edits,
# not behavioral equivalence. It never writes transformed source files.
function Test-RulePatterns([string]$original, [string]$current, [string]$path) {
    $a = (Get-Content -Raw -LiteralPath $original) -replace '\s',''
    $b = Get-Content -Raw -LiteralPath $current
    $b = $b -replace '(?m)^#include "gdp_rule_access.h"[^\r\n]*[\r\n]+',''
    $b = $b -replace '(?s)/\* DIVERGENCE\(MVDM-HOST-DIV-123\).*?\*/',''
    $b = $b -replace '\s',''
    $b = [regex]::Replace($b,
        '\((I[A-Z0-9]+)\*\)softpc_gdp_rule_slot\(\(void\*\)r1,(\d+)u,sizeof\(\1\)\)', {
        param($m) '('+$m.Groups[1].Value+'*)(r1+'+$m.Groups[2].Value+')'
    })
    $b = [regex]::Replace($b,
        '\((I[A-Z0-9]+)\*\)softpc_gdp_rule_slot\(\(void\*\)r1,\(unsignedint\)\(\*\(\(IHPE\*\)&\((r\d+)\)\)\),sizeof\(\1\)\)', {
        param($m) '('+$m.Groups[1].Value+'*)((*((IHPE*)&(r1)))+*((IHPE*)&('+$m.Groups[2].Value+')))'
    })
    $b = [regex]::Replace($b,
        '\((I[A-Z0-9]+)\*\)softpc_gdp_rule_address\(\(void\*\)r1,\(uintptr_t\)\(\*\(\(IHPE\*\)&\((r\d+)\)\)\),sizeof\(\1\)\)', {
        param($m) '('+$m.Groups[1].Value+'*)(*((IHPE*)&('+$m.Groups[2].Value+')))'
    })
    $b = [regex]::Replace($b, 'calloc\((\d+)u,sizeof\(IUH\)\)', {
        param($m) 'malloc('+([int]$m.Groups[1].Value*4)+')'
    })
    if ($path -match 'sevid0(19|20)\.c$') { $b = $b.Replace('((IUH)1<<','(1<<') }
    return $a -ceq $b
}
$paths = @(git -C $RepositoryRoot ls-files -- $prefix)
if ($LASTEXITCODE -ne 0) { throw 'Cannot enumerate tracked mirror' }
$rows = foreach ($path in $paths) {
    $relative = $path.Substring($prefix.Length)
    $current = Join-Path $RepositoryRoot $path
    $original = Join-Path $OriginalRoot $relative
    $currentHash = (Get-FileHash -LiteralPath $current -Algorithm SHA256).Hash
    $originalHash = if (Test-Path -LiteralPath $original) {
        (Get-FileHash -LiteralPath $original -Algorithm SHA256).Hash
    } else { $null }
    $counts = @{}
    foreach ($mode in @('raw', 'ignoreTrailingWhitespace')) {
        $added = 0; $deleted = 0; $hunks = @()
        if ($originalHash -and $currentHash -ne $originalHash) {
            $options = @('-c', 'core.autocrlf=false', '-c', 'core.safecrlf=false',
                'diff', '--no-index', '--no-ext-diff', '--no-textconv', '--unified=3')
            if ($mode -eq 'ignoreTrailingWhitespace') { $options += '--ignore-space-at-eol' }
            $lines = @(& git @options -- $original $current)
            if ($LASTEXITCODE -gt 1) { throw "Diff failed: $relative" }
            foreach ($line in $lines) {
                if ($line -match '^@@ ') { $hunks += $line }
                elseif ($line -match '^\+(?!\+\+)') { $added++ }
                elseif ($line -match '^-(?!--)') { $deleted++ }
                elseif ($line -match '^Binary files ') { throw "Binary difference: $relative" }
            }
        }
        $counts[$mode] = [ordered]@{added=$added; deleted=$deleted; hunks=$hunks}
    }
    [ordered]@{
        path=$relative; current_sha256=$currentHash; original_sha256=$originalHash
        same_bytes=($currentHash -eq $originalHash)
        rule_patterns_match=$(if ($relative -match '^base/cvidc/(sevid\d{3}|sinit\d{3})\.c$') {
            Test-RulePatterns $original $current $relative
        } else { $null })
        raw=$counts.raw; ignore_trailing_whitespace=$counts.ignoreTrailingWhitespace
    }
}
$retained = @{}
foreach ($row in $rows) { $retained[$row.path] = $true }
$notSelected = @(Get-ChildItem -LiteralPath $OriginalRoot -File -Recurse | ForEach-Object {
    $relative = $_.FullName.Substring($OriginalRoot.Length + 1).Replace('\','/')
    if (!$retained.ContainsKey($relative)) { $relative }
})
[ordered]@{
    git_version=(& git --version)
    repository_revision=(& git -C $RepositoryRoot rev-parse HEAD)
    original_revision=(& git -C $OriginalRoot rev-parse HEAD)
    retained_count=$rows.Count
    same_bytes_count=@($rows | Where-Object { $_.same_bytes }).Count
    no_peer_count=@($rows | Where-Object { !$_.original_sha256 }).Count
    not_selected=$notSelected
    files=$rows
} | ConvertTo-Json -Depth 6
