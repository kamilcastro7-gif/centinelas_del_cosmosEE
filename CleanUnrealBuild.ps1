param(
    [string]$ProjectDir = "C:\Users\Usuario\source\repos\centinelas_del_cosmosEE",
    [string]$EngineDir = "C:\unreal engine\UE_4.27"
)

$ErrorActionPreference = "Stop"
$LogFile = Join-Path $ProjectDir "CleanUnrealBuild.log"

function Write-Log { param([string]$Msg) $Msg | Out-File -FilePath $LogFile -Append; Write-Host $Msg }

Write-Log "=== Unreal Build Cleanup Script ==="

# 1. Kill blocking processes
Write-Log "[1/4] Terminating UnrealEditor.exe and ShaderCompileWorker.exe..."
Get-Process "UnrealEditor", "ShaderCompileWorker" -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
Start-Sleep -Seconds 2

# 2. Deep clean
Write-Log "[2/4] Cleaning Intermediate, DerivedDataCache, and Binaries Dlls..."
$pathsToDelete = @(
    (Join-Path $ProjectDir "Intermediate"),
    (Join-Path $ProjectDir "DerivedDataCache"),
    (Join-Path $ProjectDir "Saved")
)
foreach ($p in $pathsToDelete) {
    if (Test-Path $p) { Remove-Item -LiteralPath $p -Recurse -Force -ErrorAction SilentlyContinue; Write-Log "  Deleted: $p" }
}
$binDir = Join-Path $ProjectDir "Binaries\Win64"
if (Test-Path $binDir) {
    Get-ChildItem -LiteralPath $binDir -Filter "*.dll" | Remove-Item -Force -ErrorAction SilentlyContinue
    Get-ChildItem -LiteralPath $binDir -Filter "*.pdb" | Remove-Item -Force -ErrorAction SilentlyContinue
    Write-Log "  Cleaned: $binDir (*.dll, *.pdb)"
}

# 3. Fix BuildConfiguration.xml
Write-Log "[3/4] Fixing BuildConfiguration.xml..."
$bcfgPath = "$env:APPDATA\Unreal Engine\UnrealBuildTool\BuildConfiguration.xml"
if (Test-Path $bcfgPath) {
    [xml]$xml = Get-Content $bcfgPath
    $nodes = $xml.SelectNodes("//ParallelExecutor/MemoryPerActionBytes")
    $changed = $false
    foreach ($node in $nodes) {
        $parent = $node.ParentNode
        [void]$parent.RemoveChild($node)
        $changed = $true
    }
    if ($changed) {
        $xml.Save($bcfgPath)
        Write-Log "  Removed MemoryPerActionBytes from ParallelExecutor in $bcfgPath"
    } else {
        Write-Log "  No MemoryPerActionBytes entry found; nothing to remove."
    }
} else {
    Write-Log "  BuildConfiguration.xml not found at $bcfgPath"
}

# 4. Regenerate project files
Write-Log "[4/4] Regenerating Visual Studio project files..."
$ubt = Join-Path $EngineDir "Engine\Binaries\DotNET\UnrealBuildTool.exe"
$uproject = Join-Path $ProjectDir "centinelas_del_cosmosEE.uproject"
if ((Test-Path $ubt) -and (Test-Path $uproject)) {
    & $ubt -projectfiles -project="$uproject" -game -engine
    if ($LASTEXITCODE -eq 0) {
        Write-Log "  Project files regenerated successfully."
    } else {
        Write-Log "  ERROR: UBT exited with code $LASTEXITCODE"
    }
} else {
    Write-Log "  WARNING: UBT ($ubt) or uproject ($uproject) not found."
}

Write-Log "=== Cleanup complete ==="
Write-Log "Log written to: $LogFile"
