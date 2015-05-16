BZ2-LuaMission
==============

Lua Mission DLL for Battlezone II

To compile, you will need a custom luajit compillation named luajit51.dll/exp/dll.
You may need to create the folder "Redist" under "BuildMission" depending on your configuration.

luajit 2.0.4 has been included in this project.  It was compiled with the following modification:
"src\msvcbuild.bat" altered to use:
@set LJDLLNAME=luajit51.dll
@set LJLIBNAME=luajit51.lib
Compiled with 'Visual Studio 2008 Command Prompt'



Current Usage:
@root\luajit51.dll
@root\luajit51.exp
@root\lua\_api.lua
@root\lua\hook.lua
@root\*assetpath*\chilli.lua
@root\@assetpath*\missions\LUAMission.dll