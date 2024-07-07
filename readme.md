<p align="center">
  <img height="200" src="icon.png" alt="Drash logo">
  <h1 align="center">Basic MAK VR-Forces Backend Plugin</h1>
</p>
<p align="center">C++ project for a basic <a href="https://www.mak.com/products/simulate/vr-forces">MAK VR-Forces</a> backend plugin</p>
<p align="center">This serves as a good starting point for creating simple plugins
</p>

---

## Table of Contents
- [Quick Start](#quick-start)
- [Essential software](#essential-software)
- [Development environment](#development-environment)
- [Folder structure](#folder-structure)
- [Development](#development)
- [VS Build Configurations](#vs-build-configurations)
- [vcpkg packages](#vcpkg-packages)
- [Environment Variables](#environment-variables)
- [Configuration file](#configuration-file)
- [Logging](#logging)

## Quick start
1) git clone
2) Setup development environment with `vcpkg`
3) Set environment variables (refer to `Environment Variables`)
4) Open solution
5) Adjust PropertySheets as needed. Check [Development](#development)
6) Build > Batch Build. Enable all configurations. Click "Build"
7) Plugin is now inside VRF's plugin folder. Plugin's XML is in VRF's appData\plugins folder

## Essential software
- Visual Studio 2019
  - Workloads
    - Desktop development with C++
  - Individual components
    - MSVC V141 - VS 2017 C++ x64/x86 build tools (v14.16)
    - MSVC V142 - VS 2019 C++ x64/x86 build tools (Latest). This is used by vcpkg

Tested Configurations
| MAK VR-Forces | MAK VR-Link | MAK RTI | Qt |
|---|---|---|---|
| 4.7 | 5.4.1 | 4.5.1 | 5.10.1 |
| 4.10 | 5.7 | 4.6.1 | 5.10.1 |
| 5.0.1 | 5.8 | 4.6.1 | 5.15.2 |
| 5.1.0 | 5.9 | 4.6.1 | 5.15.2 |
  
## Development environment
- Visual Studio 2019
  - Configuration Properties (All Configurations)
    - Windows SDK Version: 10.0.18362.0
    - Platform Toolset: Visual Studio 2017 (v141)
- Install [vcpkg](https://github.com/microsoft/vcpkg)
```
> git clone https://github.com/microsoft/vcpkg
> .\vcpkg\bootstrap-vcpkg.bat
> .\vcpkg\vcpkg --feature-flags=versions install # Install versioning feature of vcpkg
> .\vcpkg\vcpkg integrate install # with elevated permissions
```
- Edit `vcpkg folder/triplets/x64-windows.cmake` to include 
  - `set(VCPKG_PLATFORM_TOOLSET "v141")`
  - `set(VCPKG_DEP_INFO_OVERRIDE_VARS "v141")`

- Add `your project folder\vcpkg_installed\x64-windows\bin` to your `PATH`

- Note: After a Visual Studio upgrade, you might need to run `vcpkg integrate install` again.

- Optional: Add `your project folder\vcpkg_installed` folder as an exception for your antivirus
  - Some packages, e.g., Boost `b2.exe` is treated as a virus (https://github.com/microsoft/vcpkg/issues/13353)
  - Start > Settings > Update & Security > Windows Security > Virus & threat protection. Under Virus & threat protection settings, select Manage settings, and then under Exclusions, select Add or remove exclusions. Add your vcpkg_installed folder path.

## Folder structure
```sh
├── [build] (intermediate and output directory)
├── [include]
├── [src]
├── .gitattributes
├── .gitignore
├── readme.md
├── solution.sln
├── plugin.xml
├── project.vcxproj
├── vcpkg.json
└── vcpkg_installed
```

## Development

### Property Sheets

Build configuration common to all projects should go into CommonProperties.props. Build configuration common to a Simulation Protocol (DIS, HLA13, HLA1516, HLA1516e), Debug or Release should go into Properties*.props. These files can be found in the root of the project. Some examples of configuration that should go into these files are Preprocessor Definitions, Include Directories, Link Dependencies, etc. These files can be edited in a text editor or by using Visual Studio > View > Other Windows > Property Manager.

For different versions of VR-Forces, the `PreprocessorDefinitions` in `CommonProperties.props` need to be modified. They are listed below. 

| MAK VR-Forces | PreprocessorDefinitions |
|---|---|
| 4.7 | `WIN32;_WINDOWS;NO_DFD_SUPPORT;NOMINMAX;_SCL_SECURE_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS;DT_VRF_DLL_BUILD;DT_VRF_PLUGIN_EXPORTS;BUILDING_PLUGIN;DT_DLL_BUILD;DT_USE_DLL;IS_64BIT;BOOST_NO_RVALUE_REFERENCES;BOOST_FILESYSTEM_VERSION=2` |
| 4.10 | `WIN32;_WINDOWS;NO_DFD_SUPPORT;NOMINMAX;_SCL_SECURE_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS;DT_VRF_DLL_BUILD;DT_VRF_PLUGIN_EXPORTS;BUILDING_PLUGIN;DT_DLL_BUILD;DT_USE_DLL;IS_64BIT;BOOST_SIGNALS_DYN_LINK=1;BOOST_FILESYSTEM_DYN_LINK=1;BOOST_FILESYSTEM_VERSION=3;BOOST_OPTIONAL_USE_OLD_DEFINITION_OF_NONE=1;BOOST_SIGNALS_NO_DEPRECATION_WARNING=1;NIGHTLYBUILD` |
| 5.0, 5.1 | `WIN32;_WINDOWS;NO_DFD_SUPPORT;NOMINMAX;_SCL_SECURE_NO_WARNINGS;_CRT_SECURE_NO_WARNINGS;DT_VRF_DLL_BUILD;DT_VRF_PLUGIN_EXPORTS;BUILDING_PLUGIN;DT_DLL_BUILD;DT_USE_DLL;IS_64BIT;BOOST_SIGNALS_DYN_LINK=1;BOOST_FILESYSTEM_DYN_LINK=1;BOOST_FILESYSTEM_VERSION=3;BOOST_OPTIONAL_USE_OLD_DEFINITION_OF_NONE=1;BOOST_SIGNALS_NO_DEPRECATION_WARNING=1;NIGHTLYBUILD;RELWITHDEBUG;MAK_RELWITHDEBUG` |

### Boost

You are supposed to use the version of boost that comes with VR Forces. You can determine the version by going to VR-Forces-Install-folder/lib64. For convenience, some of the versions have been listed below

| MAK VR-Forces | Boost |
|---|---|
| 4.7 | 1.46 |
| 4.10 - 5.1 | 1.63 |

## VS Build Configurations
- **ReleaseDIS, ReleaseHLA13, Release1516, Release1516e**
  - Equivalent to VRF's RelWithDebInfo
  - To build to VRF's Release's specifications:
    - `Configuration Properties > C/C++ > General > Debug Information Format: Program Database (/Zi)`
    - `Configuration Properties > C/C++ > General > Multi-processor Compilation: <blank>`    
    - `Configuration Properties > Linker > Debugging > Generate Debug Info: No`
  - To build to VRF's MinSizeRelease's specifications:
    - `Configuration Properties > C/C++ > General > Debug Information Format: <blank>`
    - `Configuration Properties > C/C++ > General > Multi-processor Compilation: <blank>`    
    - `Configuration Properties > C/C++ > Optimization > Optimization: Maximum Optimization (Favor Size) (/O1)`
    - `Configuration Properties > Linker > General > Enable Incremental Linking: No (/INCREMENTAL:NO)`
    - `Configuration Properties > Linker > Debugging > Generate Debug Info: No`
- **DebugDIS, DebugHLA13, DebugHLA1516, DebugHLA1516e**
  - Equivalent to VRF's Debug

## vcpkg packages
| Package  | Version  | Purpose  |
|---|---|---|
| spdlog | 1.13.0 | Logging |
| fmt | bundled with spdlog | String formatting |
| nlohmann-json | 3.11.3 | JSON SerDes |

## Environment Variables
Add all the below variables into system's environment variables (can also be added as user level environment variables)
- MAK_VRFDIR
  - VR-Forces install location (e.g C:/MAK/vrforces4.7)

- MAK_VRLDIR
  - VR-Link install location (e.g C:/MAK/vrlink5.4.1)

- MAK_RTIDIR
  - RTI install location (e.g C:/MAK/makRti4.5)

## Configuration file
Parameters for the application can be changed via an external configuration file located in the same folder as the DLL. Configuration file is in the form DLLNAME.json
| Field  | Data type  | Value(s)  | Usage  |
|---|---|---|---|
| IsEnable  | Boolean  | true or false  | Enable/disable plugin  |
| IsEnableDebugPrint  | Boolean  | true or false  | Enable/disable debug console print-out  |
| IsEnablePostTickLogic  | Boolean  | true or false  | Enable/disable code in PostTick  |

## Logging
This basic plugin includes `spdlog` and it's bundled `fmt`. Trace logs and above are written to `<VR Forces folder>\bin64\logs\mylog.txt` and warnings and above are written to console. Configure spdlog in `Config.cpp` 
