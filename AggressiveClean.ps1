$ProjectDir = "C:\Users\Usuario\source\repos\centinelas_del_cosmosEE"
$EngineDir  = "C:\unreal engine\UE_4.27"
$LogFile    = Join-Path $ProjectDir "AggressiveClean.log"

function Log { param([string]$m) "$(Get-Date -Format 'HH:mm:ss') $m" | Out-File $LogFile -Append; Write-Host $m }

Log "=== Aggressive Cleanup ==="

# 1. Kill processes
Log "[1/4] Killing blocking processes..."
$targets = @("UE4Editor","LiveCodingConsole","ShaderCompileWorker","MSBuild")
foreach ($p in $targets) {
    Get-Process $p -ErrorAction SilentlyContinue | Stop-Process -Force -ErrorAction SilentlyContinue
    Log "  Terminated $p"
}
Start-Sleep -Seconds 3

# 2. Delete Binaries & Intermediate entirely
Log "[2/4] Deleting Binaries and Intermediate..."
$delPaths = @(
    (Join-Path $ProjectDir "Binaries"),
    (Join-Path $ProjectDir "Intermediate")
)
foreach ($p in $delPaths) {
    if (Test-Path $p) {
        Remove-Item -LiteralPath $p -Recurse -Force -ErrorAction Stop
        Log "  Deleted: $p"
    } else { Log "  Not found: $p" }
}

# 3. Fix BuildConfiguration.xml (regex removal)
Log "[3/4] Removing MemoryPerActionBytes from BuildConfiguration.xml..."
$cfgPath = "$env:APPDATA\Unreal Engine\UnrealBuildTool\BuildConfiguration.xml"
if (Test-Path $cfgPath) {
    $content = Get-Content $cfgPath -Raw
    $newContent = $content -replace '(?m)^\s*<MemoryPerActionBytes>.*?</MemoryPerActionBytes>\s*$', ''
    if ($content -ne $newContent) {
        Set-Content -Path $cfgPath -Value $newContent -Encoding UTF8
        Log "  Removed MemoryPerActionBytes entry."
    } else {
        Log "  No MemoryPerActionBytes found."
    }
} else { Log "  Config file not found." }

# 4. Regenerate project files
Log "[4/4] Regenerating VS project files..."
$ubt    = Join-Path $EngineDir "Engine\Binaries\DotNET\UnrealBuildTool.exe"
$uproj  = Join-Path $ProjectDir "CentCosmos.uproject"
if ((Test-Path $ubt) -and (Test-Path $uproj)) {
    & $ubt -projectfiles -project="$uproj" -game -engine
    if ($LASTEXITCODE -eq 0) { Log "  Project files regenerated successfully." }
    else                     { Log "  ERROR: UBT exited with code $LASTEXITCODE" }
} else {
    Log "  MISSING: UBT=$ubt or uproject=$uproj"
}

Log "=== Done ==="
Log "Log: $LogFile"
