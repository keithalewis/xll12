rem install winhlp32.exe for reading .hlp files
rem https://stackoverflow.com/questions/43617504/how-to-convert-hlp-files-into-chm-files
md msu
expand Windows8.1-KB917607-x64.msu /f:* .\msu
cd msu
md cab
expand Windows8.1-KB917607-x64.cab /f:* .\cab
cd cab
cd amd64_microsoft-windows-winhstb.resources_31bf3856ad364e35_6.3.9600.20470_en-us_1ab8cd412c1028d0
takeown /f "%SystemRoot%\en-us\winhlp32.exe.mui"
icacls "%SystemRoot%\en-us\winhlp32.exe.mui" /grant "%UserName%":f
ren %SystemRoot%\en-us\winhlp32.exe.mui winhlp32.exe.mui.w10
copy winhlp32.exe.mui %SystemRoot%\en-us\winhlp32.exe.mui 
takeown /f "%SystemRoot%\winhlp32.exe"
icacls "%SystemRoot%\winhlp32.exe" /grant "%UserName%":f
ren %SystemRoot%\winhlp32.exe winhlp32.exe.w10
cd ..
cd "amd64_microsoft-windows-winhstb_31bf3856ad364e35_6.3.9600.20470_none_1a54d9f2f676f6c2"
copy winhlp32.exe %SystemRoot%\winhlp32.exe 