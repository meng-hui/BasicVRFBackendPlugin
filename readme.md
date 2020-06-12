# Basic MAK VR-Forces Backend Plugin
This is the C++ project for a basic MAK VR-Forces Backend Plugin.
This is created because using MAK VR-Forces' example as a starting point is generally a bad idea. The example contains relative links that are broken when project is moved out of the folder. It becomes a bad habit for developers to have their entire project situated in the examples' folder. It also contains too much unnecessary stuff that developers have to remove. Most times, the unnecessary stuff are just left inside, becoming dead load, slowing down build and development time.

## Quick start
1) git clone
2) Open solution
3) Select `Release` as active configuration
4) Build
5) Plugin is now inside VRF's plugin folder

## Essential software
- Visual Studio 2019
  - Workloads
    - Desktop development with C++
  - Individual components
    - MSVC V141 - VS 2017 C++ x64/x86 build tools (v14.16)
- MAK VR-Forces 4.7
- MAK VR-Link 5.4.1
- MAK RTI 4.5
  
## Development environment
- Visual Studio 2019
  - Configuration Properties (RelWithDebInfo)
    - Windows SDK Version: 10.0.18362.0
    - Platform Toolset: Visual Studio 2017 (v141)

## Project folder structure
```sh
├── [build] (intermediate and output directory)
├── [include]
├── [src]
├── .gitattributes
├── .gitignore
├── readme.md
├── solution.sln
├── packages.config
├── plugin.xml
└── project.vcxproj
```

## VS Build Configurations
- **Release (default)**
  - Equivalent to VRF's RelWithDebInfo
  - To build to VRF's Release's specifications:
    - `Configuration Properties > C/C++ > General > Debug Information Format: Program Database (/Zi)`
    - `Configuration Properties > Linker > Debugging > Generate Debug Info: No`
  - To build to VRF's MinSizeRelease's specifications:
    - `Configuration Properties > C/C++ > General > Debug Information Format: <blank>`
    - `Configuration Properties > C/C++ > Optimization > Optimization: Maximum Optimization (Favor Size) (/O1)`
    - `Configuration Properties > Linker > General > Enable Incremental Linking: No (/INCREMENTAL:NO)`
    - `Configuration Properties > Linker > Debugging > Generate Debug Info: No`
- **Debug**
  - Equivalent to VRF's Debug

## Nuget packages
| Package  | Version  | Purpose  | Nuget Uri  | Comments  |
|---|---|---|---|---|
| fmt  | 6.2.1  | Formatting library  | https://www.nuget.org/packages/fmt/  | https://fmt.dev/  |

## Environment Variables
Add all the below variables into system's environment variables (can also be added as user level environment variables)
> MAK_VRFDIR
> - VR-Forces install location (e.g C:/MAK/vrforces4.7)

> MAK_VRLDIR
> - VR-Link install location (e.g C:/MAK/vrlink5.4.1)

> MAK_RTIDIR
> - RTI install location (e.g C:/MAK/makRti4.5)

## Settings
TODO: write down any settings that has to be applied manually