# xll12

This library makes it easy to create Excel add-ins for
versions of Excel 12 or greater. See
[xlltemplate](https://github.com/keithalewis/xlltemplate) to get started.

## Debugging

To debug an add-in you must tell Visual Studio the full path to the Excel executable and what add-in to load.  
In the project `Properties` in the `Debugging` tab the `Command` should be `$(registry:HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\excel.exe)`.  
To have Excel open your add in when debugging specify `Command Arguments` to be `"$(TargetPath)"`.  
You may want to also specify `/p $(ProjectDir)` so Excel will default to opening files in your project directory.

