# xlltemplate

Template for Excel add-ins

Fork or clone the repository. Set the `Configuration` (Debug or Release) and `Platform` (x86 or x64) and build the `template` project.  The platform must be the same as Excel: x86 for 32-bit or x64 for 64-bit.

## Debugging

Set the configuration to `Debug|x86` if using 32-bit Excel or `Debug|x64` for 64-bit Excel.
Set breakpoints by clicking on the left boarder at the line you want to stop at, the hit `F5` to start debugging.

## Documentation

Instantiate a `AddIn xai_anyname(Documentation(LR"(Documentation goes here)"));` object to create a 
[Sandcastle Help File Builder](https://github.com/EWSoftware/SHFB) project file (`.shfbproj`) in the
Debug folder of your project. Put the `.chm` file it creates in the same location as the `.xll` add-in to
integrate with Excel's Insert Function [Help on this function](https://support.office.com/en-us/article/Insert-Function-74474114-7C7F-43F5-BEC3-096C56E2FB13).