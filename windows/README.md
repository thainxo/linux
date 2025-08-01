# Uninstall apps from powershell in Windows 10
Run "PowerShell" as Administrator
```bat
Get-AppxPackage *Teams* | Remove-AppxPackage 
```