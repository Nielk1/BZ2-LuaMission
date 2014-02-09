Notes on Battlezone 2 v1.3 Public Beta 6 MP DLL Source code.

   Disclaimer: this source code release is UNSUPPORTED by Activision,
Pandemic Studios, or anyone else. While it is a complete archive of an
functional multiplayer DLL source code and MS Visual Studio .NET 2008
project, it comes with no warranty, no official support, or anything
else. Use of other compilers may not be supported.

   Note: this source code is NOT required in any way to run BZ2 1.3pb4
or any other build.  It is only provided to give aspiring DLL authors
a template to work from. If you only want to write a singleplayer DLL,
the BZ2 DLL scriptor (a separate fan-made project) may well be easier
to work with. However, for multiplayer, C++ is a requirement.

   Included in this archive is a complete set of source code needed to
build Battlezone 2's Deathmatch, Instant Action, MPInstant, StratCTF,
and Strategy DLLs for version 1.3 public beta 4. Probably the easiest
way to build the DLLs is to open up the Missions/Missions.sln file
with VS.NET 2008, and hit build. If you want to access the individual
DLL projects directly, inside the Missions/Deathmatch01 folder, there
is a Deathmatch01.vcproj project file for MS Visual Studio .NET (aka
MSVC). Opening this project in that environment will let you build up
a DLL to the Debug/Missions folder off the root of this
archive. Similarly, the Missions/Instant, Missions/StratCTF,
Missions/Strategy02 and Missions/MPInstant folders contain source code
for the other DLLs.

   If you do not have VS.NET, there is a free download from
http://www.microsoft.com/Express/Download/ -- get either the
all-in-one iso download, or the C++ download. You should also get the
SP1 update as well. If you want to develop your own games, you may
want to also download the DirectX SDK from
http://msdn.microsoft.com/directx/sdk/ 

   Once you have everything unpacked and ready, and the Missions.sln
is opened, for each project (Deathmatch01, ...) right click on it in
the solution explorer, and select properties. Set the Configuration
Properties -> General -> Output Directory to the directory you've got
1.3pb6 or later unpacked to. I'd also recommend for each project,
doing a right click, and selecting rename, in order to ensure that any
DLLs you create do not have the same name as an existing official DLL;
different DLLs across various machines in a MP game will cause Bad
Assets.

   Comments on multiplayer programming: because BZ2 runs in a lockstep
fashion, every DLL running on all of the machines must behave
identically in terms of anything that affects gamestate. Thus, do not
do anything based on the local player, but if you're doing something,
loop over all players, e.g. a loop like

for(int CheckTeam=0;CheckTeam<MAX_TEAMS;CheckTeam++)
{
    Handle h=GetPlayerHandle(CheckTeam);
	if(h)
    { // Player Exists...
		// .. Do stuff here....
	}
}

   Only visual-only things like objectifying targets (see the
ExecuteRace() function) can be done not identically across platforms.

---

   Some useful tips and tricks:

1) The name of the .sln has no effect on the output file(s). But, if
you want to rename it, then do the following:

 - Close Visual Studio
 - In Windows Explorer (or your favorite file management app), copy & 
   rename the .sln on disk.
 - BEFORE you open the new .sln with Visual Studio, open the .sln file in
   notepad and rename it in there

--


2) How to debug a DLL you've built, under the Visual Studio debugger

 a) Build a 'debug' build. Probably need to ensure that the output
    .dll filename is the same as the release build's output .dll
    filename. With the latest 1.3 source code distributions, debug
    builds should be linked against bzone.exe; if it's linked against
    battlezone.exe, then change this.

 b) Put in breakpoints into your code. With default Visual C++
    keybindings, go to the line you want it to stop on, and hit F9 to
    drop a breakpoint. A red circle will appear left of the line

 c) Right click on the project you want to debug, ensure it's set as
    the startup project

 d) Right click on project you want to debug, select properties. Go to
    Config Properties -> Debugging. Set the command line to the full
    path to bzone.exe. (If you have BZ2 installed to multiple
    directories, then select the one that'll run the .dll you're
    building.) For the working directory entry, set it to the dir
    bzone.exe is in. As to the command arguments line
    (e.g. commandline params), for a MP dll, set the command line to
    "/win /host 0" (no quotes). If you're testing a SP/IA dll, you can
    use something like "/win mymap.bzn" . You should use /win unless
    you've got a multimonitor setup and put the debugger on the
    secondary monitor and let BZ2 take over the primary monitor. Leave
    the rest of the entries in this field at default.

 e) Hit F5 (assuming default VC++ keyboard shortcuts) to start
    debugging.  Your breakpoints will turn to thin rings, because your
    DLL isn't loaded.  But, when you load the mission, they should
    turn back to solid circles.  If they never turn solid, then the
    DLL you built isn't loaded. Double check output dir, build
    configuration, etc

 f) If they are solid, then when execution passes that line, it'll
    pause the game and drop you into in the debugger. You can examine
    variables in the locals/auto/watch window(s). Hit F10 to step one
    line, F11 to step into a function, F5 to resume full speed

   Note: when done, please be sure to switch back to the release build
type. A simple 'rebuild all' should help force that to happen.

---

3) More debugging tips -- how to set a hardware breakpoint when a
variable changes. This is taken from something I wrote previously:

http://groups.google.com/group/microsoft.public.vc.debugger/browse_thread/thread/ed6ff683e7f54b19/d1ee49e36fba4ccf?q=breakpoint+author:nathan%40visi.com#d1ee49e36fba4ccf

 You can set up to 4 hardware breakpoints that stop the program when
a particular memory address is written to. As such, they're not easy
to use if you're trying to breakpoint a variable on the stack, as the
stack's memory addresses are generally recycled from function to
function. To set a breakpoint, do the following:

a) Obtain the address you wish to breakpoint at. I generally do this
in DevStudio's watch window, adding an entry for "&g_SomeVariable".
You'll get an address like 0x12345678

b) In the breakpoints window (alt-F9), go to New -> New Data
Breakpoint.  If you don't see the entry for "New Data Breakpoint",
then go to the main DevStudio Tools -> Options... -> Debugging, and
check [x] Enable Address-level Debugging. Once address-level debugging
is enabled, then go back to the breakpoints window.

   Side note: with address-level debugging, DevStudio 2003/2005 has
the REALLY ANNOYING behavior for me of treating F10 (step over when
stopped) as a "go into the disassembly window and step one
instruction" instead. For me, it does that, even if I didn't have the
disassembly window open, and was staring at the source code.  I have
to go turn off address-level debugging, and then DevStudio will get a
clue and *stay* in the source window and step one C/C++ line at a time
when I hit F10. Very frickin' annoying. Fix it, MS!

c) Anyhow, once you you've selected New Data Breakpoint, in the window
that appears, enter the address (e.g. 0x12345678) where specified, and
hit ok.

   Side note - this can't be used to easily breakpoint memory that's
changed due to a file load or other times the OS writes to your
memory.  I seem to remember that the OS has a different view of your
app's memory, so the address entered won't trip the breakpoints. 