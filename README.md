BZ2-LuaMission
==============

Lua Mission DLL for Battlezone II

To compile, you will need a custom luajit compillation name luajit51.dll/exp/dll.
You may need to create the folder "Redist" under "BuildMission" depending on your configuration.

To create the needed luajit51.dll and lib pull/clone this project:

git clone luajit-2.0.git">http://luajit.org/git/luajit-2.0.git

alter "src\msvcbuild.bat" to use:
@set LJDLLNAME=luajit51.dll
@set LJLIBNAME=luajit51.lib

Visual Studio 2008 Command Prompt
Navigate to the "src" folder and run "msvcbuild"

This will generate the luajit51.dll/exp/dll file to use in "BZ2 LUA Dll Source Code\Debug".

In the future, hopefully, luajit will be the lua51.dll included in BZ2's 1.3 patches rather than stock LUA5.1 .