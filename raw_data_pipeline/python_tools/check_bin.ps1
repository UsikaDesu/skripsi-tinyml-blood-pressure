$Shell = New-Object -ComObject Shell.Application
$RecycleBin = $Shell.Namespace(0xA) 
foreach ($item in $RecycleBin.Items()) { 
    if ($item.Name -match "\.csv$") { 
        Write-Host "Found: $($item.Name) from $($item.Path)"
    } 
}
