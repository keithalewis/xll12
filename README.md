# xll12

## News Flash!

A new version of this library is available at [xlladdins/xll](https://github.com/xlladdins/xll).
See [xlladdins.com](https://xlladdins.com) for more informaton.

This library makes it easy to create Excel add-ins for
versions of Excel 12 or greater. See
http://keithalewis.github.io/xll12/ for documentation and
[xlltemplate](https://github.com/keithalewis/xlltemplate) to get started.

In order to hook up Excel [Help on this function]() to your Sandcastle Help File Builder documentation you must run `shfb.bat` in the `xll12` folder.

## Debugging

To debug an add-in you must tell Visual Studio the full path to the Excel executable and what add-in to load.  
In the project `Properties` in the `Debugging` tab the `Command` should be 

> `$(registry:HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\excel.exe)`

To have Excel open your add-in when debugging and specify the default directory set `Command Arguments` to be 

> `"$(TargetPath)" /p "$(ProjectDir)"`



