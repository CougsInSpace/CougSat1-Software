# Downloads the RTL-SDR latest software from http://osmocom.org/attachments/download/2242/RelWithDebInfo.zip
# Extracts "rtlsdr.lib", puts it in "lib/"
# Extracts "rtlsdr.dll" and "libusb-1.0.dll", puts them in "bin/"
# Downloads http://github.com/pbatard/libwdi/releases/download/b721/zadig-2.4.exe to "bin/"

$OutPath = './bin'
# ensure the output folder exists
$exists = Test-Path -Path $OutPath
if ($exists -eq $false)
{
  $null = New-Item -Path $OutPath -ItemType Directory -Force
}
[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12
Invoke-WebRequest -Uri http://github.com/pbatard/libwdi/releases/download/b721/zadig-2.4.exe -Outfile $OutPath/zadig.exe

$Path = "./rtlsdr.zip"
Invoke-WebRequest -Uri http://osmocom.org/attachments/download/2242/RelWithDebInfo.zip -Outfile $Path

# ensure the output folder exists
$exists = Test-Path -Path $OutPath
if ($exists -eq $false)
{
  $null = New-Item -Path $OutPath -ItemType Directory -Force
}

# load ZIP methods
Add-Type -AssemblyName System.IO.Compression.FileSystem

# open ZIP archive for reading
$zip = [System.IO.Compression.ZipFile]::OpenRead($Path)
$zip.Entries | where {$_.FullName -like 'rtl-sdr-release/x32/rtlsdr.lib'} | foreach {[System.IO.Compression.ZipFileExtensions]::ExtractToFile($_, "./lib/rtlsdr.lib", $true)}
$zip.Entries | where {$_.FullName -like 'rtl-sdr-release/x32/rtlsdr.dll'} | foreach {[System.IO.Compression.ZipFileExtensions]::ExtractToFile($_, "./bin/rtlsdr.dll", $true)}
$zip.Entries | where {$_.FullName -like 'rtl-sdr-release/x32/libusb-1.0.dll'} | foreach {[System.IO.Compression.ZipFileExtensions]::ExtractToFile($_, "./bin/libusb-1.0.dll", $true)}

$zip.Dispose()

Remove-Item $Path

./bin/zadig.exe