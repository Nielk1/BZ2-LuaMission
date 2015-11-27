--BZ2LuaMission Extended API

--==============================================================================================================================================================
-- Utility Functions
--==============================================================================================================================================================

--- Is this oject a function?
-- Checks that the object is a function
-- @param object Object in question
function isfunction(object)
    return (type(object) == "function");
end

local function istable(t) return type(t) == 'table' end


--[[
--- Message Buffer
-- Used to avoid extra allocation time when possible.
-- This API will avoid using this variable twice without first converting to lua strings.
local msgBuffer = ffi.new("char[1025]");

--- Convert input to CString char[]
-- This utility function is used in bindings to handle cstring reference passes.
-- This function will use the preallocated msgBuffer if possible
-- @param input The string for conversion.
-- @param doNotUseBuffer Do not use existing buffer, even if possible, defaults to false. (You may need this if an exported function has 2 cstring paramaters.)
function tocstring(input, doNotUseBuffer)
    if doNotUseBuffer or string.len(input) > 1024 then
        local longString = ffi.new("char[?]",string.len(input) + 1);
        ffi.copy(longString,input);
        return longString;
    end
    ffi.fill(msgBuffer,1025);
    ffi.copy(msgBuffer,input);
    return msgBuffer;
end

--- Clamps a number to within a certain range.
-- @param n Input number.
-- @param low Minimum of range.
-- @param high Maximum of range.
function math.clamp(n, low, high) return math.min(math.max(n, low), high) end
--]]
--- Is this oject an instance of GameObject?
-- Checks that the object is a GameObject or similar enough to one
-- @param object Object in question
function isgameobject(object)
    --return (type(object) == "table" and getmetatable(object) == GameObject);
    return (type(object) == "table" and object.__type == "GameObject");
end
--[[
function isobjectdefinition(object)
    return (type(object) == "table" 
      and object.Open ~= nil and type(object.Open) == "function"
      and object.Close ~= nil and type(object.Close) == "function"
      and object.GetHexInt ~= nil and type(object.GetHexInt) == "function"
      and object.GetInt ~= nil and type(object.GetInt) == "function"
      and object.GetLong ~= nil and type(object.GetLong) == "function"
      and object.GetFloat ~= nil and type(object.GetFloat) == "function"
      and object.GetDouble ~= nil and type(object.GetDouble) == "function"
      and object.GetChar ~= nil and type(object.GetChar) == "function"
      and object.GetBool ~= nil and type(object.GetBool) == "function"
      and object.GetString ~= nil and type(object.GetString) == "function"
      and object.GetColor ~= nil and type(object.GetColor) == "function"
      and object.GetVector ~= nil and type(object.GetVector) == "function");
end
--]]
--- Is this oject a string?
-- @param object Object in question
function isstring(object)
    return (type(object) == "string");
end

--- Is this oject a number?
-- @param object Object in question
function isnumber(object)
    return (type(object) == "number");
end

--- Is this oject a Vector?
-- @param object Object in question
function isvector(object)
    return (type(object) == "cdata" and ffi.istype("Vector", object));
end

--- Is this oject a VECTOR_2D?
-- @param object Object in question
function isvector_2d(object)
    return (type(object) == "cdata" and ffi.istype("VECTOR_2D", object));
end

--- Is this oject a Matrix?
-- @param object Object in question
function ismatrix(object)
    return (type(object) == "cdata" and ffi.istype("Matrix", object));
end

--- Is this object a Color?
-- @param object Object in question
function iscolor(object)
    return (type(object) == "table" and object.ToColorLong ~= nil and type(object.ToColorLong) == "function" and object.ToRBGA ~= nil and type(object.ToRBGA) == "function" and object.ToRBG ~= nil and type(object.ToRBG) == "function");
end

--- Is this object a TEAMCOLOR_TYPE?
-- @param object Object in question
function isteamcolortype(object)
    return (type(object) == "cdata" and ffi.istype("TEAMCOLOR_TYPE", object));
end

--- Is this object a ObjectInfoType?
-- @param object Object in question
function isobjectinfotype(object)
    return (type(object) == "cdata" and ffi.istype("ObjectInfoType", object));
end

--- Is this object a DLLAudioCategory?
-- @param object Object in question
function isaudiocategory(object)
    return (type(object) == "cdata" and ffi.istype("DLLAudioCategory", object));
end

--- Is this oject an instance of AudioHandle?
-- Checks that the object is a AudioHandle or similar enough to one
-- @param object Object in question
function isaudiohandle(object)
    return (type(object) == "table" and object.GetAudioHandle ~= nil and type(object.GetAudioHandle) == "function");
end

--- Is this oject an instance of RandomizeType?
-- @param object Object in question
function israndomizetype(object)
    return (type(object) == "cdata" and ffi.istype("RandomizeType", object));
end
--[[

--==============================================================================================================================================================
-- Common Enums, Structs, Types
--==============================================================================================================================================================

--local VECTOR_2D
VECTOR_2D = ffi.metatype("VECTOR_2D", {
  __add = function(a, b) return VECTOR_2D(a.x+b.x, a.z+b.z) end,
})

--local Vector
Vector = ffi.metatype("Vector", {
  __add = function(a, b) return Vector(a.x+b.x, a.y+b.y, a.z+b.z) end,
})

--local Matrix
Matrix = ffi.metatype("Matrix", {})

--local AnimationType = {};
AnimationType = {};
AnimationType.loop = 0;
AnimationType.twoway = 1;

--local AvoidType = {};
AvoidType = {};
AvoidType.AVD_NONE = 0; -- don't avoid collisions
AvoidType.AVD_FORCE = 1; -- use force avoidance
AvoidType.AVD_PLAN = 2; -- plan collision avoidance

--local Command = {};
Command = {};
Command.CMD_NONE = 0;
Command.CMD_SELECT = 1;
Command.CMD_STOP = 2;
Command.CMD_GO = 3;
Command.CMD_ATTACK = 4;
Command.CMD_FOLLOW = 5;
Command.CMD_FORMATION = 6; -- not used anywhere in code.
Command.CMD_PICKUP = 7;
Command.CMD_DROPOFF = 8;
Command.CMD_UNDEPLOY = 9;
Command.CMD_DEPLOY = 10;
Command.CMD_NO_DEPLOY = 11; -- Used by crigs, deploybuildings to indicate they can't do that there
Command.CMD_GET_REPAIR = 12;
Command.CMD_GET_RELOAD = 13;
Command.CMD_GET_WEAPON = 14;
Command.CMD_GET_CAMERA = 15; -- Human players only.
Command.CMD_GET_BOMB = 16;
Command.CMD_DEFEND = 17;
Command.CMD_RESCUE = 18;
Command.CMD_RECYCLE = 19;
Command.CMD_SCAVENGE = 20;
Command.CMD_HUNT = 21;
Command.CMD_BUILD = 22;
Command.CMD_PATROL = 23;
Command.CMD_STAGE = 24;
Command.CMD_SEND = 25;
Command.CMD_GET_IN = 26;
Command.CMD_LAY_MINES = 27;
Command.CMD_LOOK_AT = 28;
Command.CMD_SERVICE = 29;
Command.CMD_UPGRADE = 30;
Command.CMD_DEMOLISH = 31;
Command.CMD_POWER = 32;
Command.CMD_BACK = 33;
Command.CMD_DONE = 34;
Command.CMD_CANCEL = 35;
Command.CMD_SET_GROUP = 36;
Command.CMD_SET_TEAM = 37;
Command.CMD_SEND_GROUP = 38;
Command.CMD_TARGET = 39;
Command.CMD_INSPECT = 40;
Command.CMD_SWITCHTEAM = 41;
Command.CMD_INTERFACE = 42;
Command.CMD_LOGOFF = 43;
Command.CMD_AUTOPILOT = 44;
Command.CMD_MESSAGE = 45;
Command.CMD_CLOSE = 46;
Command.CMD_MORPH_SETDEPLOYED = 47; -- For morphtanks
Command.CMD_MORPH_SETUNDEPLOYED = 48; -- For morphtanks
Command.CMD_MORPH_UNLOCK = 49; -- For morphtanks
Command.CMD_BAILOUT = 50;
Command.CMD_BUILD_ROTATE = 51; -- Update building rotations by 90 degrees.
Command.CMD_CMDPANEL_SELECT = 52;
Command.CMD_CMDPANEL_DESELECT = 53;

--local Slots = {};
Slots = {};
Slots.DLL_TEAM_SLOT_RECYCLER = 1;
Slots.DLL_TEAM_SLOT_FACTORY = 2;
Slots.DLL_TEAM_SLOT_ARMORY = 3;
Slots.DLL_TEAM_SLOT_TRAINING = 4;
Slots.DLL_TEAM_SLOT_BOMBERBAY = 5;
Slots.DLL_TEAM_SLOT_SERVICE = 6;
Slots.DLL_TEAM_SLOT_TECHCENTER = 7;
Slots.DLL_TEAM_SLOT_COMMTOWER = 8;
Slots.DLL_TEAM_SLOT_BASE4 = 9; -- Whatever's in base slot #4 (only w/ mods)
Slots.DLL_TEAM_SLOT_BASE5 = 10; -- see above
Slots.DLL_TEAM_SLOT_BASE6 = 11; -- see above
Slots.DLL_TEAM_SLOT_BASE7 = 12; -- see above
Slots.DLL_TEAM_SLOT_BASE8 = 13; -- see above
Slots.DLL_TEAM_SLOT_BASE9 = 14; -- see above


--==============================================================================================================================================================
-- Color
--==============================================================================================================================================================

--local Color = {};
Color = {};
Color.__index = Color;
Color.__add = function(a,b)
    return Color.new((a.r + b.r) / 2, (a.g + b.g) / 2, (a.b + b.b) / 2, (a.a + b.a) / 2);
end;
Color.__newindex = function(table, key, value)
    if key == "a" or key == "r"  or key == "g"  or key == "b" then
        rawset(table, key, value);
    else
        error("Attempt to modify read-only table")
    end
end


--- Create new Color Intance
-- @param r Red or long describing color.
-- @param g Green or nil if r is long.
-- @param b Blue or nil if r is long.
-- @param a Alpha, defaults to 255, nil if r is long.
function Color.new(r,g,b,a)
  local self = setmetatable({}, Color)
  if r ~= nil and g == nil and b == nil and a == nil then
    self.a = bit.rshift(bit.band(0xFF000000,r),24);
    self.r = bit.rshift(bit.band(0x00FF0000,r),16);
    self.g = bit.rshift(bit.band(0x0000FF00,r),8);
    self.b = bit.band(0x000000FF,r);
  else
    self.r = math.clamp(r,0,255);
    self.g = math.clamp(g,0,255);
    self.b = math.clamp(b,0,255);
    if a == nill then
      self.a = 255;
    else
      self.a = math.clamp(a,0,255);
    end
  end
  return self;
end

--- Return long form color for most BZ2 functions
-- @param self Color instance
-- @return long form color
function Color.ToColorLong(self)
  return bit.bor(bit.lshift(self.a, 24), bit.lshift(self.r, 16), bit.lshift(self.g, 8), self.b);
end

--- Return RGB form color for some BZ2 functions for normal use
-- @param self Color instance
-- @return Red
-- @return Green
-- @return Blue
function Color.ToRGB(self)
    return self.r,self.g,self.b;
end

--- Return RGBA form color for normal use
-- @param self Color instance
-- @return Red
-- @return Green
-- @return Blue
-- @return Alpha
function Color.ToRGBA(self)
    return self.r,self.g,self.b,self.a;
end

--- Color long for White
Color.WHITE     = 0xFFFFFFFF;
--- Color long for Red
Color.RED       = 0xFFFF0000;
--- Color long for Green
Color.GREEN     = 0xFF00FF00;
--- Color long for Blue
Color.BLUE      = 0xFF0000FF;
--- Color long for Turquoise
Color.TURQUOISE = 0xFF00FFFF;
--- Color long for Violet
Color.VIOLET    = 0xFFFF00FF;
--- Color long for Yellow
Color.YELLOW    = 0xFFFFFF00;

--==============================================================================================================================================================
-- AiPathObject
--==============================================================================================================================================================

--FindAiPath
--FreeAiPath
--GetAiPaths
--SetPathType
--[[
__declspec( dllexport ) class AiPath * __cdecl FindAiPath(struct Vector const &,struct Vector const &) asm("?FindAiPath@@YAPAVAiPath@@ABUVector@@0@Z");
__declspec( dllexport ) void __cdecl FreeAiPath(class AiPath *) asm("?FreeAiPath@@YAXPAVAiPath@@@Z");
__declspec( dllexport ) void __cdecl GetAiPaths(int &,char * * &) asm("?GetAiPaths@@YAXAAHAAPAPAD@Z"); ??
__declspec( dllexport ) void __cdecl SetPathType(char *,enum PathType) asm("?SetPathType@@YAXPADW4PathType@@@Z"); ??

--- AiPathObject
-- An object containing all functions and data related to an AiPath
local AiPathObject = {}; -- the table representing the class, which will double as the metatable for the instances
AiPathObject.__index = AiPathObject; -- failed table lookups on the instances should fallback to the class table, to get methods
AiPathObject.__gc = function(self) ffi.C.FreeAiPath(self.GetAiPath()); end;

--- Create new GameObject Intance
-- @param id Handle from BZ2.
function AiPathObject.new(object)
  local self = setmetatable({}, AiPathObject);
  self.object = object;
  return self;
end

--- Get AiPath used by BZ2.
-- @param self AiPathObject instance.
function AiPathObject.GetAiPath(self)
  return self.object;
end
]]

--]]
--[[
--==============================================================================================================================================================
-- ObjectDefinition
--==============================================================================================================================================================

local ObjectDefinition_ = {};
ObjectDefinition_.__index = ObjectDefinition_;
ObjectDefinition_.__newindex = function(table, key, value)
    if key == "open" or key == "name" then
        rawset(table, key, value);
    else
        error("Attempt to modify read-only table")
    end
end

function ObjectDefinition(name)
    if not isstring(name) then error("Paramater name must be string."); end
    local self = setmetatable({}, ObjectDefinition_)
    self.name = name;
    return self;
end

function ObjectDefinition_.Save(self)
    WriteMarker("ObjectDefinition");
    local length = string.len(self.name);
    ffi.C.WriteInt(ffi.new("int[1]", length));
    ffi.C.WriteBytes(tocstring(self.name), length);
end

function ObjectDefinition_.Open(self)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not self.open then
       ffi.C.OpenODF(tocstring(self.name)); 
       self.open = true;
    end
end

function ObjectDefinition_.Close(self)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if self.open then
       ffi.C.CloseODF(tocstring(self.name)); 
       self.open = false;
    end
end

function ObjectDefinition_.GetHexInt(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = 0; end
    local value = ffi.new("int[1]");
    ffi.C.GetODFHexInt(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tonumber(value);
end

function ObjectDefinition_.GetInt(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = 0; end
    local value = ffi.new("int[1]");
    ffi.C.GetODFInt(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tonumber(value);
end

function ObjectDefinition_.GetLong(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = 0; end
    local value = ffi.new("long[1]");
    ffi.C.GetODFLong(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tonumber(value);
end

function ObjectDefinition_.GetFloat(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = 0.0; end
    local value = ffi.new("float[1]");
    ffi.C.GetODFFloat(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tonumber(value);
end

function ObjectDefinition_.GetDouble(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = 0.0; end
    local value = ffi.new("double[1]");
    ffi.C.GetODFDouble(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tonumber(value);
end

function ObjectDefinition_.GetChar(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = '\0'; end
    local value = ffi.new("char[1]");
    ffi.C.GetODFChar(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tostring(value);
end

function ObjectDefinition_.GetBool(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = false; end
    local value = ffi.new("bool[1]");
    ffi.C.GetODFBool(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return tobool(value);
end

function ObjectDefinition_.GetString(self, block, name, size, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = nil; end
    local passIn;
    if size == nil then size = 1024; end
    if size < 1025 then
        passIn = msgBuffer;
    else
        passIn = ffi.new("char[?]",size + 1);
    end
    ffi.fill(passIn,size + 1);
    if default ~= nil then
        ffi.C.GetODFString(tocstring(self.name), tocstring(block), tocstring(name), size, passIn, tocstring(default));
    else
        ffi.C.GetODFString(tocstring(self.name), tocstring(block), tocstring(name), size, passIn, nil);
    end
    return ffi.string(passIn);
end

function ObjectDefinition_.GetColor(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then
        default = 0;
    elseif iscolor(default) then
        default = default:ToColorLong();
    end
    local value = ffi.new("unsigned long[1]");
    ffi.C.GetODFColor(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return Color.new(value);
end

function ObjectDefinition_.GetVector(self, block, name, default)
    if not isobjectdefinition(self) then error("Paramater self must be ObjectDefinition instance."); end
    if not isstring(block) then error("Paramater block must be string."); end
    if not isstring(name) then error("Paramater name must be string."); end
    if default == nil then default = Vector(); end
    local value = ffi.new("Vector[1]");
    ffi.C.GetODFVector(tocstring(self.name), tocstring(block), tocstring(name), value, default);
    return value;
end
--]]

--[[
--==============================================================================================================================================================
-- AudioHandle
--==============================================================================================================================================================

local AudioHandle = {};
AudioHandle.__index = AudioHandle;
AudioHandle.__newindex = function(table, key, value)
    error("Attempt to modify read-only table")
end

function AudioHandle.new(id)
    local self = setmetatable({}, AudioHandle)
    self.handle = id;
    return self;
end

function AudioHandle.GetAudioHandle(self)
    return self.handle;
end

function AudioHandle.Save(self)
    WriteMarker("AudioHandle");
    ffi.C.WriteInt(ffi.new("int[1]", self.handle), 1);
end

--- Start audio in 3D on object
-- @param name Soundfile name
-- @param gameobject GameObject to play sound around
-- @param category DLLAudioCategory
-- @param loop Loop audio, defaults to false
-- @param stopLast Stop a prior sound on this object with the same name
-- @return AudioHandle object
function StartAudio3D(name, gameobject, category, loop, stopLast)
    if not isstring(block) then error("Paramater block must be string."); end
    if not isgameobject(gameobject) then error("Paramater gameobject must be GameObject instance."); end
    if category == nil then category = DLLAudioCategory.AUDIO_CAT_UNKNOWN; end
    if not isaudiocategory(category) then error("Paramater block must be string."); end
    --if loop == nil then loop = false; end
    --if stopLast == nil then stopLast = false; end
    return AudioHandle.new(ffi.C.StartAudio3D(tocstring(name), gameobject:GetHandle(), category, loop, stopLast));
end

--- Start audio in 3D at a location
-- @param name Soundfile name
-- @param pos Vector location for sound
-- @param category DLLAudioCategory
-- @param loop Loop audio, defaults to false
-- @return AudioHandle object
function StartAudio3DV(name, pos, category, loop)
    if not isstring(block) then error("Paramater block must be string."); end
    if not isvector(pos) then error("Paramater pos must be a Vector."); end
    if category == nil then category = DLLAudioCategory.AUDIO_CAT_UNKNOWN; end
    if not isaudiocategory(category) then error("Paramater block must be string."); end
    --if loop == nil then loop = false; end
    return ffi.C.StartAudio3DV(tocstring(name), pos.x, post.y, pos.z, category, loop);
end

--- Start audio in 2D
-- @param name Soundfile name
-- @param volume Volume, -1 uses ShellSFX volume
-- @param pan Pan
-- @param rate Rate
-- @param loop Loop audio, defaults to false, default false
-- @param isMusic Is this muic? Defaults false
-- @return AudioHandle object
function StartAudio2D(name, volume, pan, rate, loop, isMusic)
    if not isstring(block) then error("Paramater block must be string."); end
    if not isnumber(volume) then error("Paramater volume must be number."); end
    if not isnumber(pan) then error("Paramater pan must be number."); end
    if not isnumber(rate) then error("Paramater rate must be number."); end
    --if loop == nil then loop = false; end
    --if isMusic == nil then isMusic = false; end
    return AudioHandle.new(ffi.C.StartAudio2D(tocstring(name), volume, pan, rate, loop, isMusic));
end

--- Is audio playing?
-- Looped audio will be true unless a reload has occured
-- @param self AudioHandle instance.
-- @return Boolean
function AudioHandle.IsAudioPlaying(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    return ffi.C.IsAudioPlaying(self:GetAudioHandle());
end

--- Stop
-- @param self AudioHandle instance.
-- @return Boolean
function AudioHandle.StopAudio(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    ffi.C.StopAudio(self:GetAudioHandle());
end

--- Pause
-- @param self AudioHandle instance.
function AudioHandle.PauseAudio(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    ffi.C.PauseAudio(self:GetAudioHandle());
end

--- Resume
-- @param self AudioHandle instance.
function AudioHandle.ResumeAudio(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    ffi.C.ResumeAudio(self:GetAudioHandle());
end

--- Set Volume
-- @param self AudioHandle instance.
-- @param vol Volume ratio
-- @param ignoreUserVolume Do not use the user's preference as 100%
function SetVolume(self, vol, ignoreUserVolume)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    if not isnumber(vol) then error("Paramater vol must be number."); end
    ffi.C.SetVolume(self:GetAudioHandle(), vol, not ignoreUserVolume);
end

--- Set Pan
-- @param self AudioHandle instance.
-- @param pan Pan Only valid for 2D sounds, -1.0 to +1.0
function SetPan(self, pan)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    if not isnumber(vol) then error("Paramater vol must be number."); end
    ffi.C.SetPan(self:GetAudioHandle(), pan);
end

--- Set the rate
-- @param self AudioHandle instance.
-- @param rate Valid values are 0.0f .. 44100.0f. 
function SetRate(self, rate)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    if not isnumber(vol) then error("Paramater vol must be number."); end
    ffi.C.SetRate(self:GetAudioHandle(), rate);
end

--- Get duration
-- @param self AudioHandle instance.
-- @return Length in seconds
function GetAudioFileDuration(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    return ffi.C.GetAudioFileDuration(self:GetAudioHandle());
end

--- Is the playing looped
-- @param self AudioHandle instance.
-- @return Boolean
function IsPlayingLooped(self)
    if not isaudiohandle(self) then error("Paramater self must be an instance of AudioHandle."); end
    return ffi.C.IsPlayingLooped(self:GetAudioHandle());
end
--]]

--==============================================================================================================================================================
-- GameObject
--==============================================================================================================================================================

local GameObjectMetatable = {};
GameObjectMetatable.__mode = "k";
local GameObjectWeakList = setmetatable({}, GameObjectMetatable);
local GameObjectAltered = {};

--- GameObject
-- An object containing all functions and data related to a game object.
local GameObject = {}; -- the table representing the class, which will double as the metatable for the instances
--GameObject.__index = GameObject; -- failed table lookups on the instances should fallback to the class table, to get methods
GameObject.__index = function(table, key)
    local retVal = rawget(table, key);
    if retVal ~= nil then return retVal; end
    if rawget(table, "addonData") ~= nil and rawget(rawget(table, "addonData"), key) ~= nil then return rawget(rawget(table, "addonData"), key); end
    return rawget(GameObject, key); -- if you fail to get it from the subdata, move on to base (looking for functions)
end
GameObject.__newindex = function(table, key, value)
    if key ~= "id" and key ~= "addonData" then
        local addonData = rawget(table, "addonData");
        if addonData == nil then
            rawset(table, "addonData", {});
            addonData = rawget(table, "addonData");
        end
        rawset(addonData, key, value);
        GameObjectAltered[table:GetHandle()] = table;
    else
        rawset(table, key, value);
    end
end
GameObject.__type = "GameObject";

--- Create new GameObject Intance
-- @param id Handle from BZ2.
function GameObject.new(id)
    local stringId = tostring(id);
    if GameObjectWeakList[stringId] ~= nil then return GameObjectWeakList[stringId]; end
    local self = setmetatable({}, GameObject);
    self.id = id;
    GameObjectWeakList[stringId] = self;
    return self;
end

--- Get Handle used by BZ2.
-- @param self GameObject instance.
function GameObject.GetHandle(self)
    return self.id;
end

-- Save data
-- If this function is present in a table, it will save the return data instead of table elements
--function GameObject.Save(self)
--    return self.id;
--end

function GameObject.PostLoad(self)

end

function GameObject.Save(self)
    return self.id;
end

function GameObject.Load(id)
    return GameObject.new(id);
end

function GameObject.BulkSave()
    local returnData = {};
    
    for k,v in pairs(GameObjectAltered) do
      returnData[v.id] = v.addonData;
    end
    
    return returnData;
end

function GameObject.BulkLoad(data)
    for k,v in pairs(data) do
      local newGameObject = GameObject.new(k);
      newGameObject.addonData = v;
    end
end

--- Remove GameObject from world
-- @param self GameObject instance.
function GameObject.RemoveObject(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    --ffi.C.RemoveObject(self:GetHandle());
    RemoveObject(self:GetHandle());
end

--- Set group of GameObject in interface
-- @param self GameObject instance.
-- @param group Group number.
function GameObject.SetGroup(self, group)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    --ffi.C.SetGroup(self:GetHandle(), group);
    SetGroup(self:GetHandle(), group);
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Orders
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Order GameObject to Attack target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject.
-- @param priority Order priority, >0 removes user control.
function GameObject.Attack(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    --if priority == nil then priority = 1; end
    --ffi.C.Attack(self:GetHandle(), target:GetHandle(), priority);
    Service(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Service target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject.
-- @param priority Order priority, >0 removes user control.
function GameObject.Service(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    --if priority == nil then priority = 1; end
    --ffi.C.Service(self:GetHandle(), target:GetHandle(), priority);
    Service(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Goto target GameObject / Path
-- @param self GameObject instance.
-- @param target Target GameObject or Path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Goto(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    --if priority == nil then priority = 1; end
    if isgameobject(target) then
        ffi.C.GotoH(self:GetHandle(), target:GetHandle(), priority);
        --Goto(self:GetHandle(), target:GetHandle(), priority);
    --elseif isstring(target) then
    --    ffi.C.GotoP(self:GetHandle(), tocstring(target), priority)
    else
        --error("Paramater self must be GameObject instance or string");
        Goto(self:GetHandle(), target, priority);
    end
end

--- Order GameObject to Mine target Path
-- @param self GameObject instance.
-- @param target Target Path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Mine(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(target) then error("Paramater target must be string."); end
    --if priority == nil then priority = 1; end
    --ffi.C.Mine(self:GetHandle(), tocstring(target), priority);
    Mine(self:GetHandle(), target, priority);
end

--- Order GameObject to Follow target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject instance.
-- @param priority Order priority, >0 removes user control.
function GameObject.Follow(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    --if priority == nil then priority = 1; end
    --ffi.C.Follow(self:GetHandle(), target:GetHandle(), priority);
    Follow(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Defend area
-- @param self GameObject instance.
-- @param priority Order priority, >0 removes user control.
function GameObject.Defend(self, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    --if priority == nil then priority = 1; end
    --ffi.C.Defend(self:GetHandle(), priority);
    Defend(self:GetHandle(), priority);
end

--- Order GameObject to Defend2 target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject instance.
-- @param priority Order priority, >0 removes user control.
function GameObject.Defend2(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    --if priority == nil then priority = 1; end
    --ffi.C.Defend2(self:GetHandle(), target:GetHandle(), priority);
    Defend2(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Stop
-- @param self GameObject instance.
-- @param priority Order priority, >0 removes user control.
function GameObject.Stop(self, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    --if priority == nil then priority = 1; end
    --ffi.C.Stop(self:GetHandle(), priority);
    Stop(self:GetHandle(), priority);
end

--- Order GameObject to Patrol target path
-- @param self GameObject instance.
-- @param target Target Path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Patrol(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(target) then error("Paramater target must be string."); end
    --if priority == nil then priority = 1; end
    --ffi.C.Patrol(self:GetHandle(), tocstring(target), priority);
    Patrol(self:GetHandle(), target, priority);
end

--- Order GameObject to Retreat
-- @param self GameObject instance.
-- @param target Target GameObject or Path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Retreat(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if priority == nil then priority = 1; end
    if isgameobject(target) then
        --ffi.C.RetreatH(self:GetHandle(), target:GetHandle(), priority);
        Retreat(self:GetHandle(), target:GetHandle(), priority);
    --elseif isstring(target) then
    --    ffi.C.RetreatP(self:GetHandle(), tocstring(target), priority)
    else
        --error("Paramater self must be GameObject instance or string");
        Retreat(self:GetHandle(), target, priority)
    end
end

--- Order GameObject to GetIn target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject.
-- @param priority Order priority, >0 removes user control.
function GameObject.GetIn(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.GetIn(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Pickup target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject.
-- @param priority Order priority, >0 removes user control.
function GameObject.Pickup(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.Pickup(self:GetHandle(), target:GetHandle(), priority);
end

--- Order GameObject to Pickup target path name
-- @param self GameObject instance.
-- @param target Target path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Dropoff(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(target) then error("Paramater target must be string."); end
    if priority == nil then priority = 1; end
    ffi.C.Dropoff(self:GetHandle(), tocstring(target), priority)
end

--- Order GameObject to Build target config
-- Oddly this function does not include a location for the action, might want to use the far more powerful orders system.
-- @param self GameObject instance.
-- @param target Target path name.
-- @param priority Order priority, >0 removes user control.
function GameObject.Build(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(target) then error("Paramater target must be string."); end
    if priority == nil then priority = 1; end
    ffi.C.Build(self:GetHandle(), tocstring(target), priority);
end

--- Order GameObject to LookAt target GameObject
-- @param self GameObject instance.
-- @param target Target GameObject instance.
-- @param priority Order priority, >0 removes user control.
function GameObject.LookAt(self, target, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    if priority == nil then priority = 1; end
    ffi.C.LookAt(self:GetHandle(), target:GetHandle(), priority);
end

--- Order entire team to look at GameObject
-- @param self GameObject instance.
-- @param target Target team number.
-- @param priority Order priority, >0 removes user control.
function GameObject.AllLookAtMe(self, team, priority)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(team) then error("Paramater target must be number."); end
    if priority == nil then priority = 1; end
    ffi.C.AllLookAt(team, self:GetHandle(), priority);
end

--- Order GameObject to FireAt target GameObject
-- @param self GameObject instance.
-- @param target Gameobject instance.
-- @param doAim Aimfirst, defaults to false.
function GameObject.FireAt(self, target, doAim)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    ffi.C.FireAt(self:GetHandle(), target:GetHandle(), doAim);
end

--- Set command of GameObject directly
-- @param self GameObject instance.
-- @param cmd Command, see Command enum.
-- @param priority Command priority, default is 0 but only if loc is Vector
-- @param who GameObject target, defaults to 0/invalid/none but only if loc is Vector
-- @param pos Vector position or path name string
-- @param param Optional paramater interger
function GameObject.SetCommand(self, cmd, priority, who, loc, param)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(cmd) then error("Paramater target must be number."); end
    if param == nil then param = 0; end
    if isvector(pos) then
        ffi.C.SetCommandV(self:GetHandle(), cmd, priority, who.GetHandle(), where, param);
    else
        if priority == nil then priority = 0; end
        if who == nil then who = 0; end
        if isgameobject(who) then who = who.GetHandle(); end
        if path ~= nil then path = tocstring(path); end
        ffi.C.SetCommandP(self:GetHandle(), cmd, priority, who, path, param);
    end
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Position and Velocity
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get the position vector of a GameObject
-- @param self GameObject instance.
-- @return Position Vector
function GameObject.GetPositionV(self)
    --if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    --local retVal = Vector();
    --ffi.C.GetPositionV(self:GetHandle(), retVal);
    --return retVal;
    --for k,v in pairs(getmetatable(self:GetHandle())) do
    --    PrintConsoleMessage(k .. ' ' .. v;
    --end
    return GetPosition(self:GetHandle());
end

--- Get the position vector of a GameObject using deeper search
-- Slower search that covers recently killed or removed objects.
-- @param self GameObject instance.
-- @return Position Vector
function GameObject.GetPosition2(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = Vector();
    ffi.C.GetPosition2(self:GetHandle(), retVal);
    return retVal;
end

--- Get the forward facing vector of the GameObject
-- @param self GameObject instance.
-- @return Facing Vector
function GameObject.GetFront(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = Vector();
    ffi.C.GetFront(self:GetHandle(), retVal);
    return retVal;
end

--- Get the position matrix of the GameObject
-- @param self GameObject instance.
-- @return Position Matrix
function GameObject.GetPositionM(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = Matrix();
    ffi.C.GetPositionM(self:GetHandle(), retVal);
    return retVal;
end

--- Set the position of the GameObject
-- @param self GameObject instance.
-- @param loc Vector or Matrix position, or path point string
function GameObject.SetPosition(self, pos)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if ismatrix(loc) then
        ffi.C.GetPositionM(self:GetHandle(), pos);
    elseif isvector(loc) then
        ffi.C.SetVectorPosition(self:GetHandle(), pos);
    elseif isstring(loc) then
        ffi.C.SetPositionP(self:GetHandle(), tocstring(pos));
    else
        error("Paramater loc must be Vector, Matrix, or path point name (string).");
    end
end

--- Get the velocity vector of the GameObject
-- @param self GameObject instance.
-- @return Velocity vector.
function GameObject.GetVelocity(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetVelocity(self:GetHandle());
end

--- Set the velocity vector of the GameObject
-- @param self GameObject instance.
-- @param vel Velocity vector.
function GameObject.SetVelocity(self, vel)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isvector(vel) then error("Paramater vel must be Vector instance."); end
    ffi.C.SetVelocity(self:GetHandle(), vel);
end

--- Set a random heading angle for the GameObject
-- @param self GameObject instance.
function GameObject.SetRandomHeadingAngle(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.SetRandomHeadingAngle(self:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Health / Damage
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Applys damage to the GameObject crediting last damager
-- @param self GameObject instance.
-- @param amt Ammount of Damage, support whole numbers.
function GameObject.Damage(self, amt)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(amt) then error("Paramater amt must be a number."); end
    ffi.C.Damage(self:GetHandle(), amt);
end

--- Applys damage to the GameObject crediting last damager
-- @param self GameObject instance.
-- @param amt Ammount of Damage, supports decimal numbers.
function GameObject.DamageF(self, amt)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(amt) then error("Paramater amt must be a number."); end
    ffi.C.DamageF(self:GetHandle(), amt);
end

--- Applys damage to the GameObject with the doer as self
-- @param self GameObject instance.
-- @param amt Ammount of Damage, supports decimal numbers.
function GameObject.SelfDamage(self, amt)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(amt) then error("Paramater amt must be a number."); end
    ffi.C.SelfDamage(self:GetHandle(), amt);
end

--- Get the health of the GameObject
-- @param self GameObject instance.
-- @return Health
function GameObject.GetHealth(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(amt) then error("Paramater amt must be a number."); end
    return ffi.C.GetHealth(self:GetHandle());
end

--- Get the current health of the GameObject
-- @param self GameObject instance.
-- @return Health
function GameObject.GetCurHealth(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetCurHealth(self:GetHandle());
end

--- Get the max health of the GameObject
-- @param self GameObject instance.
-- @return Health
function GameObject.GetMaxHealth(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetMaxHealth(self:GetHandle());
end

--- Sets the current health of the GameObject
-- @param self GameObject instance.
-- @param NewHealth New health value.
function GameObject.SetCurHealth(self, NewHealth)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(NewHealth) then error("Paramater NewHealth must be a number."); end
    ffi.C.SetCurHealth(self:GetHandle(), NewHealth);
end

--- Sets the max health of the GameObject
-- @param self GameObject instance.
-- @param NewHealth New health value.
function GameObject.SetMaxHealth(self, NewHealth)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(NewHealth) then error("Paramater NewHealth must be a number."); end
    ffi.C.SetMaxHealth(self:GetHandle(), NewHealth);
end

--- Adds health to the GameObject
-- @param self GameObject instance.
-- @param health New health value.
function GameObject.AddHealth(self, health)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(health) then error("Paramater health must be a number."); end
    ffi.C.AddHealth(self:GetHandle(), health)
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Ammo / Local Ammo
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get the ammo of the GameObject
-- @param self GameObject instance.
-- @return Ammo
function GameObject.GetAmmo(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetAmmo(self:GetHandle());
end

--- Adds ammo to the GameObject
-- @param self GameObject instance.
-- @param ammo Ammo to add.
function GameObject.AddAmmo(self, ammo)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammo) then error("Paramater health must be a number."); end
    ffi.C.AddAmmo(self:GetHandle(), ammo);
end

--- Get the current ammo of the GameObject
-- @param self GameObject instance.
-- @return Current Ammo.
function GameObject.GetCurAmmo(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetCurAmmo(self:GetHandle());
end

--- Get the max ammo of the GameObject
-- @param self GameObject instance.
-- @return Max Ammo.
function GameObject.GetMaxAmmo(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetMaxAmmo(self:GetHandle());
end

--- Set current ammo of the GameObject
-- @param self GameObject instance.
-- @param NewAmmo Ammo to set.
function GameObject.SetCurAmmo(self, NewAmmo)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammo) then error("Paramater ammo must be a number."); end
    ffi.C.SetCurAmmo(self:GetHandle(), NewAmmo);
end

--- Set max ammo of the GameObject
-- @param self GameObject instance.
-- @param NewAmmo Max ammo to set.
function GameObject.SetMaxAmmo(self, NewAmmo)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammo) then error("Paramater ammo must be a number."); end
    ffi.C.SetMaxAmmo(self:GetHandle(), NewAmmo);
end

--- Get the current local ammo of the selected weapon of the GameObject
-- @param self GameObject instance.
-- @param weaponIndex Weapon index.
-- @return Current local ammo.
function GameObject.GetCurLocalAmmo(self, weaponIndex)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(weaponIndex) then error("Paramater weaponIndex must be a number."); end
    return ffi.C.GetCurLocalAmmo(self:GetHandle(), weaponIndex)
end

--- Add to the current local ammo of the selected weapon of the GameObject
-- @param self GameObject instance.
-- @param ammount Ammo to add.
-- @param weaponIndex Weapon index.
function GameObject.AddLocalAmmo(self, ammount, weaponIndex)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammount) then error("Paramater ammount must be a number."); end
    if not isnumber(weaponIndex) then error("Paramater weaponIndex must be a number."); end
    ffi.C.AddLocalAmmo(self:GetHandle(), ammount, weaponIndex)
end

--- Get the max local ammo of the selected weapon of the GameObject
-- @param self GameObject instance.
-- @param weaponIndex Weapon index.
-- @return Max local ammo.
function GameObject.GetMaxLocalAmmo(self, weaponIndex)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(weaponIndex) then error("Paramater weaponIndex must be a number."); end
    return ffi.C.GetMaxLocalAmmo(self:GetHandle(), weaponIndex)
end

--- Set the current local ammo of the selected weapon of the GameObject
-- @param self GameObject instance.
-- @param ammount Ammo to set.
-- @param weaponIndex Weapon index.
function GameObject.SetCurLocalAmmo(self, ammount, weaponIndex)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammount) then error("Paramater ammount must be a number."); end
    if not isnumber(weaponIndex) then error("Paramater weaponIndex must be a number."); end
    ffi.C.SetCurLocalAmmo(self:GetHandle(), ammount, weaponIndex)
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Animation and Effects
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Set the aimation of the GameObject
-- @param self GameObject instance.
-- @param name name of the animation
-- @param type Animation type, referance AnimationType
-- @return max frames
function GameObject.SetAnimation(self, name, type)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(name) then error("Paramater name must be a string."); end
    if not isnumber(type) then error("Paramater type must be a number."); end
    if type == nil then type = 0; end
    type = math.clamp(type,0,1);
    return ffi.C.SetAnimation(self:GetHandle(), tocstring(name), type);
end

--- Get the aimation of the GameObject
-- @param self GameObject instance.
-- @param name name of the animation
-- @return current frame
function GameObject.GetAnimationFrame(self, name)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(name) then error("Paramater name must be a string."); end
    return ffi.C.GetAnimationFrame(self:GetHandle(), tocstring(name));
end

--- Start current aimation of the GameObject
-- @param self GameObject instance.
function GameObject.StartAnimation(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.StartAnimation(self:GetHandle());
end

--- Start current emmiter mask of the GameObject
-- [todo] check if mask is properly handled, might need to do some bitwise stuff
-- @param self GameObject instance.
function GameObject.MaskEmitter(self, mask)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(mask) then error("Paramater mask must be a number."); end
    ffi.C.MaskEmitter(self:GetHandle(), mask);
end

--- Start the emmiter in the selected slot of the GameObject
-- @param self GameObject instance.
-- @param slot Emitter slot.
function GameObject.StartEmitter(self, slot)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(slot) then error("Paramater slot must be a number."); end
    ffi.C.StartEmitter(self:GetHandle(), slot);
end

--- Start the emmiter in the selected slot of the GameObject
-- @param self GameObject instance.
-- @param slot Emitter slot.
function GameObject.StopEmitter(self, slot)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(slot) then error("Paramater slot must be a number."); end
    ffi.C.StopEmitter(self:GetHandle(), slot);
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Idel State and Animations
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Clear idel animations on this GameObject.
-- @param self GameObject instance.
function GameObject.ClearIdleAnims(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.ClearIdleAnims(self:GetHandle());
end

--- Add an idel animation to this GameObject.
-- @param self GameObject instance.
-- @param name Animation name.
function GameObject.AddIdleAnim(self, name)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(name) then error("Paramater name must be a string."); end
    ffi.C.AddIdleAnim(self:GetHandle(), tocstring(name));
end

--- Is this GameObject idel.
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsIdle(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsIdle(self:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Objectification
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Objectify the GameObject
-- @param self GameObject instance.
function GameObject.SetObjectiveOn(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    --ffi.C.SetObjectiveOn(self:GetHandle());
    SetObjectiveOn(self:GetHandle());
end

--- Objectify the GameObject
-- @param self GameObject instance.
function GameObject.SetObjectiveOff(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    --ffi.C.SetObjectiveOff(self:GetHandle());
    SetObjectiveOff(self:GetHandle());
end

--- Set the objective name of the GameObject
-- @param self GameObject instance.
-- @param name Name for the objective.
function GameObject.SetObjectiveName(self, name)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(name) then error("Paramater name must be a string."); end
    ffi.C.SetObjectiveName(self:GetHandle(), tocstring(name));
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - CTF Flag and Loot Money
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Trigger a flag steal of this object by target GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagStealBy(self, holder)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(holder) then error("Paramater holder must be GameObject instance."); end
    ffi.C.FlagSteal(self:GetHandle(), holder.GetHandle());
end

--- Trigger a flag recovery of this object by target GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagRecoverBy(self, holder)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(holder) then error("Paramater holder must be GameObject instance."); end
    ffi.C.FlagRecover(self:GetHandle(), holder.GetHandle());
end

--- Trigger a flag score of this object by target GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagScoreBy(self, holder)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(holder) then error("Paramater holder must be GameObject instance."); end
    ffi.C.FlagScore(self:GetHandle(), holder.GetHandle());
end

--- Trigger a flag steal of the target by this GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagSteal(self, flag)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(flag) then error("Paramater flag must be GameObject instance."); end
    ffi.C.FlagSteal(flag.GetHandle(), self:GetHandle());
end

--- Trigger a flag recover of the target by this GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagRecover(self, flag)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(flag) then error("Paramater flag must be GameObject instance."); end
    ffi.C.FlagRecover(flag.GetHandle(), self:GetHandle());
end

--- Trigger a flag score of the target by this GameObject
-- @param self GameObject instance.
-- @param holder Flag holder.
function GameObject.FlagScore(self, flag)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(flag) then error("Paramater flag must be GameObject instance."); end
    ffi.C.FlagScore(flag.GetHandle(), self:GetHandle());
end

--- Trigger a money score by this GameObject
-- @param self GameObject instance.
-- @param ammount Score to award.
function GameObject.MoneyScore(self, ammount)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammount) then error("Paramater ammount must be a string."); end
    ffi.C.MoneyScore(ammount, self:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Team
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get team of GameObject
-- @param self GameObject instance.
-- @return Team number.
function GameObject.GetTeamNum(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetTeamNum(self:GetHandle());
end

--- Set team of GameObject
-- @param self GameObject instance.
-- @param t Team number.
function GameObject.SetTeamNum(self, t)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(ammount) then error("Paramater ammount must be a string."); end
    ffi.C.SetTeamNum(self:GetHandle(), t);
end

--- Set team of GameObject
-- Note aliances are 1 directional, be sure to check both ways
-- @param self GameObject instance.
-- @param him GameObject instance.
-- @return Is ally bool.
function GameObject.IsAlly(self, him)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(him) then error("Paramater him must be GameObject instance."); end
    return ffi.C.IsAlly(self:GetHandle(), him);
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Deaths
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get deaths of GameObject
-- @param self GameObject instance.
-- @return Death count
function GameObject.GetDeaths(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetDeaths(self:GetHandle());
end

--- Set deaths of GameObject
-- @param self GameObject instance.
-- @param NewValue GameObject instance.
function GameObject.SetDeaths(self, NewValue)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(NewValue) then error("Paramater NewValue must be GameObject instance."); end
    ffi.C.SetDeaths(self:GetHandle(), NewValue);
end

--- Add to deaths of GameObject
-- @param delta GameObject instance.
-- @param him GameObject instance.
function GameObject.AddDeaths(self, delta)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(delta) then error("Paramater delta must be GameObject instance."); end
    ffi.C.AddDeaths(self:GetHandle(), delta)
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Kills
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get kills of GameObject
-- @param self GameObject instance.
-- @return Kill count
function GameObject.GetKills(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetKills(self:GetHandle());
end

--- Set kills of GameObject
-- @param self GameObject instance.
-- @param NewValue GameObject instance.
function GameObject.SetKills(self, NewValue)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(NewValue) then error("Paramater NewValue must be GameObject instance."); end
    return ffi.C.SetKills(self:GetHandle(), NewValue);
end

--- Add to kills of GameObject
-- @param delta GameObject instance.
-- @param him GameObject instance.
function GameObject.AddKills(self, delta)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(delta) then error("Paramater delta must be GameObject instance."); end
    ffi.C.AddKills(self:GetHandle(), delta)
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Score
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get score of GameObject
-- @param self GameObject instance.
-- @return Score count
function GameObject.GetScore(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetScore(self:GetHandle());
end

--- Set score of GameObject
-- @param self GameObject instance.
-- @param NewValue GameObject instance.
function GameObject.SetScore(self, NewValue)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(NewValue) then error("Paramater NewValue must be GameObject instance."); end
    return ffi.C.SetScore(self:GetHandle(), NewValue);
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Scavenger
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get current scrap in hold of GameObject
-- @param self GameObject instance.
function GameObject.GetScavengerCurScrap(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = ffi.C.GetScavengerCurScrap(self:GetHandle());
    if retVal == -1 then return nil; end
    return GameObject.new(retVal);
end

--- Get max scrap in hold of GameObject
-- @param self GameObject instance.
function GameObject.GetScavengerMaxScrap(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = ffi.C.GetScavengerMaxScrap(self:GetHandle());
    if retVal == -1 then return nil; end
    return GameObject.new(retVal);
end

--- Set current scrap in hold of GameObject
-- @param self GameObject instance.
-- @param aNewScrap new scrap ammount.
function GameObject.SetScavengerCurScrap(self,aNewScrap)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetScavengerCurScrap(self:GetHandle(),aNewScrap);
end

--- Set max scrap in hold of GameObject
-- @param self GameObject instance.
-- @param aNewScrap new scrap ammount.
function GameObject.SetScavengerMaxScrap(self,aNewScrap)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetScavengerMaxScrap(self:GetHandle(),aNewScrap);
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Scavenger
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Get nearest enemy to this GameObject
-- @param self GameObject instance.
-- @param ignorePilots Ignore pilots?
-- @param ignoreScavs Ignore scavengers?
-- @param maxDist Max distance to search, hard ceiling of 450.0, which is the max
-- @return Gameobject found
function GameObject.GetNearestEnemy(self, ignorePilots, ignoreScavs, maxDist)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    -- can't check for bools since nil = false and !nil = true
    if maxDist == nil then maxDist = 450.0; end
    return GameObject.new(ffi.C.GetNearestEnemy(self.handle, ignorePilots, ignoreScavs, maxDist));
end

--- Get nearest powerup to this GameObject
-- @param self GameObject instance.
-- @param ignoreSpawnpoints Ignore spawn points?
-- @param maxDist Max distance to search, hard ceiling of 450.0, which is the max
-- @return Gameobject found
function GameObject.GetNearestPowerup(self, ignoreSpawnpoints, maxDist)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    -- can't check for bools since nil = false and !nil = true
    if maxDist == nil then maxDist = 450.0; end
    return GameObject.new(ffi.C.GetNearestPowerup(self.handle, ignoreSpawnpoints, maxDist));
end

--- Get nearest person to this GameObject
-- @param self GameObject instance.
-- @param skipFriendlies Ignore friendly objects?
-- @param maxDist Max distance to search, hard ceiling of 450.0, which is the max
-- @return GameObject found.
function GameObject.GetNearestPerson(self, skipFriendlies, maxDist)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    -- can't check for bools since nil = false and !nil = true
    if maxDist == nil then maxDist = 450.0; end
    return GameObject.new(ffi.C.GetNearestPerson(self.handle, skipFriendlies, maxDist));
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Actions
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Deploy this GameObject
-- @param self GameObject instance.
function GameObject.Deploy(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.Deploy(self:GetHandle());
end

--- Annoy the target GameObject with this GameObject
-- @param self GameObject instance.
-- @param target Target GameObject instance.
function GameObject.Annoy(self, target)
  ffi.C.Annoy(self:GetHandle(), target:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Order States and General States
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Is this GameObject following something?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsFollowing(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsFollowing(self:GetHandle());
end

--- Get what this GameObject is following
-- @param self GameObject instance.
-- @return GameObject we are following.
function GameObject.WhoFollowing(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return GameObject.new(ffi.C.WhoFollowing(self:GetHandle()));
end

--- Get current command
-- @param self GameObject instance.
-- @return AiCommand enum value
function GetCurrentCommand(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetCurrentCommand(self:GetHandle());
end

--- Get the subject of this GameObject's current order
-- @param self GameObject instance.
-- @return GameObject subject of our orders.
function GameObject.GetCurrentWho(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return GameObject.new(ffi.C.GetCurrentWho(self:GetHandle()));
end

--- Is this GameObject deployed?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsDeployed(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsDeployed(self:GetHandle());
end

--- Is this GameObject selected?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsSelected(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsSelected(self:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Pilot Operations
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Eject the pilot from the GameObject
-- @param self GameObject instance.
function GameObject.EjectPilot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.EjectPilot(self:GetHandle());
end

--- Hop the pilot out from the GameObject
-- @param self GameObject instance.
function GameObject.HopOut(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.HopOut(self:GetHandle());
end

--- Kill the pilot of the GameObject
-- @param self GameObject instance.
function GameObject.KillPilot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.KillPilot(self:GetHandle());
end

--- Remove the AI of the GameObject without killing or removing the pilot
-- @param self GameObject instance.
function GameObject.RemovePilotAI(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.RemovePilotAI(self:GetHandle());
end

--- The GameObject which this GameObject hopped out of
-- @param self GameObject instance.
-- @return GameObject that this hopped out of.
function GameObject.HoppedOutOf(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return GameObject.new(ffi.C.HoppedOutOf(self:GetHandle()));
end

--- Adds a pilot if needed to the GameObject
-- @param self GameObject instance.
function GameObject.AddPilot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.AddPilotByHandle(self:GetHandle())
end

--- Sets the pilot class of he GameObject
-- @param self GameObject instance.
-- @param odf Object Definition of pilot.
function GameObject.SetPilotClass(self, odf)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(odf) then error("Paramater odf must be a string."); end
    ffi.C.SetPilotClass(self:GetHandle(), tocstring(odf));
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Status
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Is the GameObject alive?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsAlive2(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsAlive2(self:GetHandle());
end

--- Is the GameObject flying?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsFlying2(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsFlying2(self:GetHandle());
end

--- Is the GameObject alive and pilot?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsAliveAndPilot2(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsAliveAndPilot2(self:GetHandle());
end

--- Is the GameObject not dead and pilot?
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsNotDeadAndPilot2(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsNotDeadAndPilot2(self:GetHandle());
end

--- The GameObject is around
-- @param self GameObject instance.
-- @return boolean
function GameObject.IsAround(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    --return ffi.C.IsAround(self:GetHandle());
    return IsAround(self:GetHandle());
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Shooting
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Who shot this GameObject?
-- @param self GameObject instance.
-- @return Shooter GameObject.
function GameObject.GetWhoShotMe(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return Handle.new(ffi.C.GetWhoShotMe(self:GetHandle()));
end

--- Get last time this GameObject shot an enemy
-- @param self GameObject instance.
-- @return time of last event.
function GameObject.GetLastEnemyShot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetLastEnemyShot();
end

--- Get last time this GameObject shot an friend
-- @param self GameObject instance.
-- @return time of last event.
function GameObject.GetLastFriendShot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetLastFriendShot();
end


----------------------------------------------------------------------------------------------------------------------------------------------------------------
-- GameObject - Other (Too Many...)
----------------------------------------------------------------------------------------------------------------------------------------------------------------

--- Owner of this GameObject
-- @param self GameObject instance.
-- @return owner GameObject.
function GameObject.GetOwner(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return GameObject.new(ffi.C.GetOwner(self:GetHandle()));
end

--- Set the target of this GameObject
-- @param self GameObject instance.
-- @param target Target GameObject.
function GameObject.SetTarget(self, target)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(target) then error("Paramater target must be GameObject instance."); end
    GameObject.new(ffi.C.SetTarget(self:GetHandle(), target));
end

--- Set the owner of this GameObject
-- @param self GameObject instance.
-- @param owner Owner GameObject.
function GameObject.SetOwner(self, owner)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(owner) then error("Paramater owner must be GameObject instance."); end
    GameObject.new(ffi.C.SetOwner(self:GetHandle(), owner));
end

--- Is this GameObject an instance of given odf?
-- @param self GameObject instance.
-- @param odf ODF to check.
-- @return boolean, this GameObject is an instance of given odf.
function GameObject.IsOdf(self, odf)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(odf) then error("Paramater odf must be a string."); end
    local odfClean = tocstring(odf);
    return ffi.C.IsOdf(self:GetHandle(), odfClean);
end

--- Get the race character of the GameObject
-- @param self GameObject instance.
-- @return race character.
function GameObject.GetRace(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetRace(self:GetHandle());
end

--- Get the building the GameObject is in
-- @param self GameObject instance.
-- @return Building the GameObject is in.
function GameObject.InBuilding(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return Handle.new(ffi.C.InBuilding(self:GetHandle()));
end

--- Get the terminal the GameObject is at
-- @param self GameObject instance.
-- @return Terminal the GameObject is at.
function GameObject.AtTerminal(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return Handle.new(ffi.C.AtTerminal(self:GetHandle()));
end

--- Set the controls of the GameObject
-- @param self GameObject instance.
-- @param controls Controls object, nil results in clear control set.
-- @param CTRL_BRACCEL Apply braccel control? Default false.
-- @param CTRL_STEER Apply steer control? Default false.
-- @param CTRL_PITCH Apply pitch control? Default false.
-- @param CTRL_STRAFE Apply strafe control? Default false.
-- @param CTRL_JUMP Apply jump control? Default false.
-- @param CTRL_DEPLOY Apply deploy control? Default false.
-- @param CTRL_EJECT Apply eject control? Default false.
-- @param CTRL_ABANDON Apply abandon control? Default false.
-- @param CTRL_FIRE Apply fire control? Default false.
function GameObject.SetControls(self, controls, CTRL_BRACCEL, CTRL_STEER, CTRL_PITCH, CTRL_STRAFE, CTRL_JUMP, CTRL_DEPLOY, CTRL_EJECT, CTRL_ABANDON, CTRL_FIRE)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local whichControls = 0;
    if CTRL_BRACCEL then whichControls = whichControls +   1; end
    if CTRL_STEER   then whichControls = whichControls +   2; end
    if CTRL_PITCH   then whichControls = whichControls +   4; end
    if CTRL_STRAFE  then whichControls = whichControls +   8; end
    if CTRL_JUMP    then whichControls = whichControls +  16; end
    if CTRL_DEPLOY  then whichControls = whichControls +  32; end
    if CTRL_EJECT   then whichControls = whichControls +  64; end
    if CTRL_ABANDON then whichControls = whichControls + 128; end
    if CTRL_FIRE    then whichControls = whichControls + 256; end
    if controls == nil then controls = VehicleControls(); end
    ffi.C.SetControls(self.GetHandle(), controls, whichControls);
end

--- Set the independence of the GameObject
-- @param self GameObject instance.
-- @param independence 1 = high, 0 = low.
function GameObject.SetIndependence(self, independence)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetIndependence(self:GetHandle(), independence);
end

--- Set the weapon mask of the GameObject
-- @param self GameObject instance.
-- @param mask A long of bits, similar to the mask in ODFs but converted to a decimal number.
function GameObject.SetWeaponMask(self, mask)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(mask) then error("Paramater mask must be a number"); end
    ffi.C.SetWeaponMask(self:GetHandle(), mask);
end

--- Give a weapon to the GameObject
-- This function runs through normal powerup pickup logic.
-- For more control you might need to get creative with checking weapons and giving weapons.
-- @param self GameObject instance.
-- @param weapon Weapon Name.
function GameObject.GiveWeapon(self, weapon)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(weapon) then error("Paramater weapon must be a string"); end
    ffi.C.GiveWeapon(self:GetHandle(), tocstring(weapon));
end

--- Set the perceived team of the GameObject
-- @param self GameObject instance.
-- @param team
function GameObject.SetPerceivedTeam(self, team)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(team) then error("Paramater team must be a number"); end
    ffi.C.SetPerceivedTeam(self:GetHandle(), team);
end

--- Set the skill level of the GameObject
-- @param self GameObject instance.
-- @param s Skill level.
function GameObject.SetSkill(self, s)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(s) then error("Paramater s must be a number"); end
    ffi.C.SetSkill(self:GetHandle(), s);
end

--- Check if the GameObject is within given distance of another object.
-- @param self GameObject instance.
-- @param h2 Other Object.
-- @param distance Distance to check within.
function GameObject.IsWithin(self, h2, distance)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(h2) then error("Paramater h2 must be GameObject instance."); end
    if not isnumber(distance) then error("Paramater distance must be a number"); end
    return ffi.C.IsWithin(self:GetHandle(), h2:GetHandle(), distance);
end

--- Count untits near GameObject.
-- [future] test to see what the deal is with this function, can it have some things left out?
-- @param self GameObject instance.
-- @param dist Distance to check within.
-- @param team Team filter.
-- @param odf ODF
-- @return count of objects
function GameObject.CountUnitsNearObject(self, dist, team, odf)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(dist) then error("Paramater dist must be a number"); end
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isstring(odf) then error("Paramater odf must be a string"); end
    return ffi.C.CountUnitsNearObject(self:GetHandle(), dist, team, tocstring(odf));
end

--- Set avoid type of GameObject.
-- @param self GameObject instance.
-- @param avoidType See AvoidType table.
function GameObject.SetAvoidType(self, avoidType)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(avoidType) then error("Paramater avoidType must be a number"); end
    ffi.C.SetAvoidType(self:GetHandle(), avoidType);
end

--- Clear the thrust of the GameObject.
-- @param self GameObject instance.
function GameObject.ClearThrust(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.ClearThrust(self:GetHandle());
end

--- Clear the thrust of the GameObject.
-- @param self GameObject instance.
-- @param on Boolean
function GameObject.SetVerbose(self, on)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetVerbose(self:GetHandle(), on);
end

--- Clear the thrust of the GameObject.
-- @param self GameObject instance.
-- @return Here
-- @return Coming
function GameObject.CountThreats(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local here = ffi.new("int[1]");
    local coming = ffi.new("int[1]");
    ffi.C.CountThreats(self:GetHandle(), here, coming);
    return tonumber(here[0]),tonumber(coming[0]);
end

--- Is this GameObject the player?
-- @param self GameObject instance.
-- @return Boolean
function GameObject.IsPlayer(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.IsPlayer(self:GetHandle());
end

--- Get this player's name.
-- @param self GameObject instance.
-- @return Player name
function GameObject.GetPlayerName(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetPlayerName(self:GetHandle());
end

--- Make the GameObject inert.
-- This function was used on the dedicated server host back when that was attempted.
-- @param self GameObject instance.
function GameObject.MakeInert(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.MakeInert(self:GetHandle());
end

--- Get the GameObject's label (set in editor)
-- @param self GameObject instance.
-- @return Object label
function GameObject.GetLabel(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.GetLabel(self:GetHandle());
end

--- Set the GameObject's label
-- @param self GameObject instance.
-- @param pLabel New object label
function GameObject.SetLabel(self, pLabel)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(pLabel) then error("Paramater pLabel must be a string."); end
    ffi.C.SetLabel(self:GetHandle(), tocstring(pLabel));
end

--- Get the GameObject's Tap by index
-- @param self GameObject instance.
-- @param index Tap index.
-- @return Tap object
function GameObject.GetTap(self, index)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(index) then error("Paramater index must be a number."); end
    return GameObject.new(ffi.C.GetTap(self:GetHandle(), index));
end

--- Get the GameObject's Tap by index
-- @param self GameObject instance.
-- @param index Tap index.
-- @param tapObjectHandle Object to set as tap.
function GameObject.SetTap(self, index, tapObjectHandle)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(index) then error("Paramater index must be a number."); end
    if not isgameobject(tapObjectHandle) then error("Paramater tapObjectHandle must be GameObject instance."); end
    ffi.C.SetTap(self:GetHandle(), index, tapObjectHandle.GetHandle());
end

--- Create an empty craft within the two radius defined of GameObject
-- @param self GameObject instance.
-- @param ODF ODF file.
-- @param Team Team number.
-- @param MinRadiusAway Minimum radius for spawn.
-- @return New GameObject.
function GameObject.BuildEmptyCraftNear(self, ODF, Team, MinRadiusAway, MaxRadiusAway)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(ODF) then error("Paramater  ODF must be a string."); end
    if not isnumber(Team) then error("Paramater Team must be a number."); end
    if not isnumber(MinRadiusAway) then error("Paramater MinRadiusAway must be a number."); end
    if not isnumber(MaxRadiusAway) then error("Paramater MaxRadiusAway must be a number."); end
    return GameObject.new(ffi.C.BuildEmptyCraftNear(self, tocstring(ODF), Team, MinRadiusAway, MaxRadiusAway));
end

--- Set this GameObject as User for given team
-- @param self GameObject instance.
-- @param Team Team number.
function GameObject.SetAsUser(self, Team)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(Team) then error("Paramater Team must be a number."); end
    ffi.C.SetAsUser(self:GetHandle(), Team);
end

--- Set the No Scrap Flag for this GameObject
-- @param self GameObject instance.
function GameObject.SetNoScrapFlagByHandle(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.SetNoScrapFlagByHandle(self:GetHandle());
end

--- Clear the No Scrap Flag for this GameObject
-- @param self GameObject instance.
function GameObject.ClearNoScrapFlagByHandle(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.ClearNoScrapFlagByHandle(self:GetHandle());
end

--- Get the GameObject's Target.
-- @param self GameObject instance.
-- @return Targeted GameObject.
function GameObject.GetTarget(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return GameObject.new(GetTarget(self:GetHandle()));
end

--- Reset the GameObject's team slot.
-- @param self GameObject instance.
function GameObject.ResetTeamSlot(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    ffi.C.ResetTeamSlot(self:GetHandle());
end

--- Get the GameObject's category type override.
-- @param self GameObject instance.
-- @return Category Type Override, nil if invalid handle.
function GameObject.GetCategoryTypeOverride(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = ffi.C.GetCategoryTypeOverride(self:GetHandle());
    if retVal == -2 then return nil; end
    return retVal;
end

--- Get the GameObject's category type.
-- @param self GameObject instance.
-- @return Category Type, nil if invalid handle.
function GameObject.GetCategoryType(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = ffi.C.GetCategoryType(self:GetHandle());
    if retVal == -2 then return nil; end
    return retVal;
end

--- Set the GameObject group to the first non-full group matching its type.
-- @param self GameObject instance.
-- @return Group the unit was moved too, nil if any problems.
function GameObject.SetBestGroup(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    return ffi.C.SetBestGroup(self:GetHandle());
end

--- Set the GameObject's lifespan.
-- @param self GameObject instance.
-- @param timeout Timeout time, -1 or nil will clear the timeout.
function GameObject.SetLifespan(self, timout)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if timeout == nil then timeout = -1; end
    ffi.C.SetLifespan(self:GetHandle(), timeout);
end

--- The the player that is this GameObject
-- @param self GameObject instance.
-- @param pExplanationStr Reason for the kick.
-- @param banAlso Ban the player from the session.
function GameObject.KickPlayer(self, pExplanationStr, banAlso)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(pExplanationStr) then error("Paramater pExplanationStr must be a string."); end
    -- banAlso as nil would be treated as false
    ffi.C.KickPlayer(self:GetHandle(), tocstring(pExplanationStr));
end

--- Is this GameObject a person?
-- @param self GameObject instance.
-- @return Boolean
function GameObject:IsPerson(self)
    return ffi.C.IsPerson(self:GetHandle());
end

--[[
--==============================================================================================================================================================
-- ObjectInfo
--==============================================================================================================================================================

--- Get Object Info
-- @param h GameObject
-- @param type ObjectInfoType
-- @return String value
function GetObjInfo(h, type)
    if not isgameobject(h) then error("Paramater h must be GameObject instance."); end
    if not isobjectinfotype(type) then error("Paramater type must be an ObjectInfoType."); end
    local pBuffer = ffi.new("char[65]");
    ffi.fill(pBuffer, 65);
    ffi.C.GetObjInfo(h:GetHandle(),type, pBuffer);
    return tostring(pBuffer);
end

--- Get Group Info
-- @param team Team number
-- @param group Group number
-- @param type ObjectInfoType, only accepts Get_CFG, Get_ODF, or Get_GOClass_gCfg
-- @return String value
function GetGroup(team, group, type)
    if not isnumber(team) then error("Paramater team must be a number."); end
    if not isnumber(group) then error("Paramater group must be a number."); end
    if not isobjectinfotype(type) then error("Paramater type must be an ObjectInfoType."); end
    local pBuffer = ffi.new("char[65]");
    ffi.fill(pBuffer, 65);
    ffi.C.GetGroup(team, group, type, pBuffer);
    return tostring(pBuffer);
end


--==============================================================================================================================================================
-- Cockpit Timer
--==============================================================================================================================================================

--- Start the Cockpit Timer countdown
-- @param time Target time.
-- @param warn Time for warning color.
-- @param alert Time for alert color.
function StartCockpitTimer(time, warn, alert)
    if not isnumber(time) then error("Paramater time must be a number."); end
    if not isnumber(warn) then error("Paramater warn must be a number."); end
    if not isnumber(alert) then error("Paramater alert must be a number."); end
    if warn == nil then warn = LONG_MIN; end
    if alert == nil then alert = LONG_MIN; end
    ffi.C.StartCockpitTimer(time, warn, alert);
end

--- Start the Cockpit Timer countup
-- @param time Starting time.
-- @param warn Time for warning color.
-- @param alert Time for alert color.
function StartCockpitTimerUp(time, warn, alert)
    if not isnumber(time) then error("Paramater time must be a number."); end
    if not isnumber(warn) then error("Paramater warn must be a number."); end
    if not isnumber(alert) then error("Paramater alert must be a number."); end
    if warn == nil then warn = LONG_MAX; end
    if alert == nil then alert = LONG_MAX; end
    ffi.C.StartCockpitTimerUp(time, warn, alert);
end

--- Stop the Cockpit Timer
function StopCockpitTimer()
  ffi.C.StopCockpitTimer();
end

--- Hide the Cockpit Timer
function HideCockpitTimer()
  ffi.C.HideCockpitTimer();
end

--- Get the Cockpit Timer time
-- @return time of timer
function GetCockpitTimer()
  return ffi.C.GetCockpitTimer();
end


--==============================================================================================================================================================
-- End of nicely sorted stuff


--]]








--- Build Object
-- @param odf Object Definition File (without ".odf")
-- @param team Team number for the object, 0 to 15
-- @param pos Position as GameObject, Pathpoint Name, AiPath, Vector, or Matrix
-- @return Newly built GameObject
function BuildGameObject(odf, team, pos)
--    if not isstring(odf) then error("Paramater odf must be a string."); end
--    if not isnumber(team) then error("Paramater team must be a number."); end
--    local msg = tocstring(odf); -- convert lua string to cstring
    local handle = 0;
--    if isgameobject(pos) then
--        handle = ffi.C.BuildObject(msg, team, pos:GetHandle());
--    elseif isstring(pos) then
--        handle = ffi.C.BuildObjectP(msg, team, tocstring(pos));
--    --elseif type(pos) == "AiPath" then
--    --  handle = ffi.C.BuildObject(msg, team, pos)
--    elseif isvector(pos) then
--        handle = ffi.C.BuildObjectV(msg, team, pos);
--    elseif ismatrix(pos) then
--        handle = ffi.C.BuildObjectM(msg, team, pos);
--    else
--        error("BuildObject pos paramater is invalid, received " .. type(pos) .. ", expected GameObject, Path Name (string), AiPath, Vector, or Matrix");
--    end
    
    handle = BuildObject(odf, team, pos);
    
    if handle == 0 then return nil end;
    return GameObject.new(handle);
end
--[[
--- Get object by label or seq no.
-- @param name Label on GameObject.
-- @return GameObject with label
function GetHandle(name)
    local handle = 0;
    if isstring(name) then
        local cName = tocstring(name);
        handle = ffi.C.GetHandle(cName);
    elseif isstring(name) then
        handle = ffi.C.GetHandleSeq(name);
    else
        error("GetHandle paramater type invalid, received " .. type(name) .. ", expected string or int");
    end
    if handle == 0 then return nil; end
    return Handle.new(handle);
end


--- Set DefaultAllies
function DefaultAllies()
    ffi.C.DefaultAllies();
end

--- Set TeamplayAllies
function TeamplayAllies()
    ffi.C.TeamplayAllies();
end

--- Ally two teams
-- This operation is one way, be sure to call again with the team paramaters swapped for a proper alliance.
-- @param t1 Team 1
-- @param t2 Team 2
function Ally(t1, t2)
    if not isnumber(t1) then error("Paramater t1 must be a number."); end
    if not isnumber(t2) then error("Paramater t2 must be a number."); end
    ffi.C.Ally(t1, t2);
end

--- UnAlly two teams
-- This operation is one way, be sure to call again with the team paramaters swapped for a proper un-alliance.
-- @param t1 Team 1
-- @param t2 Team 2
function UnAlly(t1, t2)
    if not isnumber(t1) then error("Paramater t1 must be a number."); end
    if not isnumber(t2) then error("Paramater t2 must be a number."); end
    ffi.C.UnAlly(t1, t2);
end


--- Play audio message
-- @param msg Filename of audio message.
-- @param purge Optional bool to purge other sounds.
-- @return AudioMessage ID.
function AudioMessage(msg, purge)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    return ffi.C.AudioMessage(tocstring(msg), purge);
end

--- Is the audio message done?
-- @param id AudioMessage ID.
-- @return Boolean
function IsAudioMessageDone(id)
    if not isnumber(id) then error("Paramater msg must be a isnumber."); end
  return ffi.C.IsAudioMessageDone(id);
end

--- Is the audio message done?
-- @param id AudioMessage ID.
function StopAudioMessage(id)
    if not isnumber(id) then error("Paramater msg must be a isnumber."); end
    ffi.C.StopAudioMessage(id);
end

--- Preload the audio message file
-- @param msg Filename of audio message.
function PreloadAudioMessage(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.PreloadAudioMessage(tocstring(msg));
end

--- Purge the audio message file
-- @param msg Filename of audio message.
function PurgeAudioMessage(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.PurgeAudioMessage(tocstring(msg));
end

--- Preload the music message file
-- @param msg Filename of music message.
function PreloadMusicMessage(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.PreloadMusicMessage(tocstring(msg));
end

--- Purge the music message file
-- @param msg Filename of music message.
function PurgeMusicMessage(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.PurgeMusicMessage(tocstring(msg));
end

--- Play music message
-- @param msg Filename of music message.
function LoadJukeFile(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.LoadJukeFile(tocstring(msg));
end

--- Set music message intensity
-- @param intensity Music message intensity.
function SetMusicIntensity(intensity)
    if not isnumber(intensity) then error("Paramater intensity must be a number."); end
    ffi.C.SetMusicIntensity(intensity);
end

--- Is the object the local user is inspecting defined by this object definition?
-- @param odf The definition file of the object being inspected.
-- @return Boolean
function IsInfo(odf)
    if not isstring(intensity) then error("Paramater intensity must be a string."); end
    return ffi.C.IsInfo(tocstring(odf));
end

--- Start a quake
-- @param magnitude Magnitude of the quake.
function StartEarthQuake(magnitude)
    if not isnumber(magnitude) then error("Paramater magnitude must be a number."); end
    ffi.C.StartEarthQuake(magnitude);
end
--- Change the quake magnitude
-- @param magnitude Magnitude of the quake.
function UpdateEarthQuake(magnitude)
    if not isnumber(magnitude) then error("Paramater magnitude must be a number."); end
    ffi.C.UpdateEarthQuake(magnitude);
end

--- Stop the quake
function StopEarthQuake()
  ffi.C.StopEarthQuake();
end

--- Get the world the DLL is currently executing in
-- [future] document what world number is what.
-- @return World number
function GetCurWorld()
    return ffi.C.GetCurWorld();
end

--- Get string variable by name
-- @param VarItemName Variable name.
-- @return Variable value.
function GetVarItemStr(VarItemName)
    if not isstring(VarItemName) then error("Paramater VarItemName must be a string."); end
    return tostring(ffi.C.GetVarItemStr(tocstring(VarItemName)));
end

--- Get integer variable by name
-- @param VarItemName Variable name.
-- @return Variable value.
function GetVarItemInt(VarItemName)
    if not isstring(VarItemName) then error("Paramater VarItemName must be a string."); end
    return ffi.C.GetVarItemStr(GetVarItemInt(VarItemName));
end

--- Get client integer variable by index and team
-- @param TeamNum Team number.
-- @param Idx civar index.
-- @return Variable value.
function GetCVarItemInt(TeamNum, Idx)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    if not isnumber(Idx) then error("Paramater Idx must be a number."); end
    return ffi.C.GetCVarItemInt(TeamNum, Idx);
end

--- Get client string variable by index and team
-- @param TeamNum Team number.
-- @param Idx civar index.
-- @return Variable value.
function GetCVarItemStr(TeamNum, Idx)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    if not isnumber(Idx) then error("Paramater Idx must be a number."); end
    return tostring(ffi.C.GetCVarItemStr(TeamNum, Idx));
end

--- Preload an ODF by filename
-- @param cfg Object Definition filename.
function PreloadODF(cfg)
    if not isstring(cfg) then error("Paramater cfg must be a string."); end
    ffi.C.PreloadODF(tocstring(cfg));
end

--- Terrain floor at X/Z
-- @param x X coordinate.
-- @param z Z coordinate.
-- @return Y coordinate of terrain at that X/Z.
function TerrainFindFloor(x, z)
    if not isnumber(x) then error("Paramater x must be a number."); end
    if not isnumber(z) then error("Paramater z must be a number."); end
    return ffi.C.TerrainFindFloor(x, z);
end


--- Get Random Float
-- Safe for MP, portable function.
-- @param range upper bound of random range.
-- @return Random float within range.
function GetRandomFloat(range)
    if not isnumber(range) then error("Paramater range must be a number."); end
    --return ffi.C.GetRandomFloat(range);
    return GetRandomFloat(range);
end

--- Calculate the CRC of a string
-- @param n Input string.
-- @return long CRC value.
function CalcCRC(n)
    if not isstring(n) then error("Paramater n must be a string."); end
    return ffi.C.CalcCRC(tocstring(n));
end

--- Set AI Plan for given team
-- @param cfg AIP filename.
-- @param team Team number, defaults to -1.
function SetPlan(cfg, team)
    if not isstring(cfg) then error("Paramater (cfg must be a string."); end
    if team == nil then team = -1; end
    ffi.C.SetSkill(tocstring(cfg), team);
end

--- Log a float
-- @param v A decimal number.
function LogFloat(v)
    ffi.C.LogFloat(v);
end

--- Get Instation Option 'My Force'
-- @param Integer value
function GetInstantMyForce()
    return ffi.C.GetInstantMyForce();
end

--- Get Instation Option 'Computer Force'
-- @param Integer value
function GetInstantCompForce()
    return ffi.C.GetInstantCompForce();
end

--- Get Instation Option 'Difficulty'
-- @param Integer value
function GetInstantDifficulty()
    return ffi.C.GetInstantDifficulty();
end

--- Get Instation Option 'Goal'
-- @param Integer value
function GetInstantGoal()
    return ffi.C.GetInstantGoal();
end

--- Get Instation Option 'My Force'
-- @param Integer value
function GetInstantType()
    return ffi.C.GetInstantType();
end

--- Get Instation Option 'Flag'
-- @param Integer value
function GetInstantFlag()
    return ffi.C.GetInstantFlag();
end

--- Get Instation Option 'My Side'
-- @param Integer value
function GetInstantMySide()
  return ffi.C.GetInstantMySide();
end

--- Ignore Sync
-- @param on Boolean, turn on?
--function IgnoreSync(on)
--    ffi.C.IgnoreSync(on);
--end

--- Is recording?
-- @return Boolean
--function IsRecording()
--    return ffi.C.IsRecording();
--end

--- Clear Text Ojectives
function ClearObjectives()
    ffi.C.ClearObjectives();
end

--- Add Objective Text
-- @param name Text to display
-- @param color Color for the text
-- @param showTime Time to show the objective box, defaults to 8.0
function AddObjective(name, color, showTime)
    if not isstring(name) then error("Paramater name must be a string."); end
    if not iscolor(color) then error("Paramater color must be an instance of color."); end
    if showTime == nil then showTime = 8.0; end
    ffi.C.AddObjective(tocstring(name),color:ToColorLong(),showTime);
end

--- Set color fade
-- @param ratio Ratio
-- @param rate Rate
-- @param color Color to fade
function SetColorFade(ratio, rate, color)
    if not isnumber(ratio) then error("Paramater ratio must be a number."); end
    if not isnumber(rate) then error("Paramater rate must be a number."); end
    if not iscolor(color) then error("Paramater color must be an instance of color."); end
    ffi.C.SetColorFade(ratio, rate, color:ToColorLong());
end

--- Stop cheats
function StopCheats()
    ffi.C.StopCheats();
end

--- Recalculate Cliffs between two objects
-- @param h1 First Object
-- @param h2 Last Object
-- @param radius Radius
function CalcCliffs(h1, h2, radius)
    if not isgameobject(h1) then error("Paramater h1 must be GameObject instance."); end
    if not isgameobject(h2) then error("Paramater h2 must be GameObject instance."); end
    if not isnumber(radius) then error("Paramater radius must be a number."); end
    ffi.C.CalcCliffs(tocstring(path));
end

--- Recalculate Cliffs along path
-- @param path Path
function CalcCliffsP(path)
    if not isstring(path) then error("Paramater path must be a string."); end
    ffi.C.CalcCliffsP(tocstring(path));
end

--- Start sound effect
-- @param file Filename
-- @param h Optional handle
-- @return Sound effect ID
function StartSoundEffect(file, h)
    if not isstring(path) then error("Paramater path must be a string."); end
    local handle = 0;
    if isgameobject(h) then handle = h:GetHandle(); end
    return ffi.C.StartSoundEffect(tocstring(file), h);
end

--- Start sound effect
-- @param file Filename
-- @param h Optional handle
-- @return Sound effect ID
function FindSoundEffect(file, h)
    if not isstring(path) then error("Paramater path must be a string."); end
    local handle = 0;
    if isgameobject(h) then handle = h:GetHandle(); end
    return ffi.C.FindSoundEffect(tocstring(file), h);
end

-- Stop sound effect by ID
-- @param sound Sound Effect ID
function StopSoundEffect(sound)
    if not isnumber(sound) then error("Paramater sound must be a number."); end
    ffi.C.StopSoundEffect(sound);
end

--- Get object by team slot and team
-- @param TeamNum Team number
-- @param Slot Slot, see Slots table
-- @return Found GameObject
function GetObjectByTeamSlot(TeamNum, Slot)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    if not isnumber(Slot) then error("Paramater Slot must be a number."); end
    return GameObject.new(ffi.C.GetObjectByTeamSlot(TeamNum, Slot));
end

--- sin (Network Safe)
-- @param ang Angle
-- @return Number, math operation result
function portable_sin(ang)
    if not isnumber(ang) then error("Paramater ang must be a number."); end
    return ffi.C.portable_sin(ang);
end

--- cosin (Network Safe)
-- @param ang Angle
-- @return Number, math operation result
function portable_cos(ang)
    if not isnumber(ang) then error("Paramater ang must be a number."); end
    return ffi.C.portable_cos(ang);
end

--- arctangent (Network Safe)
-- @param x X
-- @param y Y
-- @return Number, math operation result
function portable_atan2(x, y)
    if not isnumber(x) then error("Paramater x must be a number."); end
    if not isnumber(y) then error("Paramater y must be a number."); end
    return ffi.C.portable_atan2(x, y);
end

--- arcsin (Network Safe)
-- @param x X
-- @return Number, math operation result
function portable_asin(x)
    if not isnumber(x) then error("Paramater x must be a number."); end
    return ffi.C.portable_asin(x);
end

--- arccosin (Network Safe)
-- @param x X
-- @return Number, math operation result
function portable_acos(x)
    if not isnumber(x) then error("Paramater x must be a number."); end
    return ffi.C.portable_acos(x);
end

--- Is the network on?
-- @return Boolean
function IsNetworkOn()
    return ffi.C.IsNetworkOn();
end

--- Is this the server?
-- @return Boolean
function ImServer()
    return ffi.C.ImServer();
end

--- Is this a dedicated server?
-- @return Boolean
function ImDedicatedServer()
    return ffi.C.ImDedicatedServer();
end

--- Is teamplay on?
-- @return Boolean
function IsTeamplayOn()
    return ffi.C.IsTeamplayOn();
end

--- Get count of active players
-- @return Count of players active
function CountPlayers()
    return ffi.C.CountPlayers()
end
--]]
--- Get player handle by team
-- @param team Team number
-- @return GameObject of player
function GetPlayerObject(team)
    --if not isnumber(team) then error("Paramater team must be a number."); end
    --return GameObject.new(ffi.C.GetPlayerHandle(team));
    return GameObject.new(GetPlayerHandle(team));
end
--[[
--- Get team race
-- @param TeamNum Team number
-- @return Race letter of team
function GetRaceOfTeam(TeamNum)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return ffi.C.GetRaceOfTeam(TeamNum);
end

--- Get teamgroup of given team
-- @param Team Team number
-- @return Teamgroup number, nil if the team doesn't fit into any normal group
function WhichTeamGroup(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    local retVal = ffi.C.WhichTeamGroup(Team);
    if retVal == 0xA5A5A5A5 then return nil; end
    return retVal;
end

--- Get count of ally teams (teamgroup based)
-- @param Team Team number
-- @return Count of ally teams (or 0 if teamgroups are off)
function CountAllies(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return ffi.C.CountAllies(Team);
end

--- Get commander team
-- @para Team
-- @return Commander's Team
function GetCommanderTeam(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return ffi.C.GetCommanderTeam(Team);
end

--- Get first allied team
-- @para Team
-- @return First Allied Team
function GetFirstAlliedTeam(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return ffi.C.GetFirstAlliedTeam(Team);
end

--- Get last allied team
-- @para Team
-- @return Last Allied Team
function GetLastAlliedTeam(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return ffi.C.GetLastAlliedTeam(Team);
end

--- Get allied team range
-- @para Team
-- @return First Allied Team
-- @return Last Allied Team
function GetAlliedTeamRange(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    return GetFirstAlliedTeam(Team),GetLastAlliedTeam(Team);
end

--- Get Teamplay Ranges
-- @para Team
-- @return DefenseTeamNum
-- @return OffenseMinTeamNum
-- @return OffenseMaxTeamNum
function GetTeamplayRanges(Team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    local DefenseTeamNum = ffi.new("int[0]");
    local OffenseMinTeamNum = ffi.new("int[0]");
    local OffenseMaxTeamNum = ffi.new("int[0]");
    ffi.C.GetTeamplayRanges(Team,DefenseTeamNum,OffenseMinTeamNum,OffenseMaxTeamNum);
    return tonumber(DefenseTeamNum),tonumber(OffenseMinTeamNum),tonumber(OffenseMaxTeamNum);
end

--- Clear team colors
function ClearTeamColors()
    ffi.C.ClearTeamColors();
end

--- Set default team colors
function DefaultTeamColors()
    ffi.C.DefaultTeamColors();
end

--- Set teamplay team colors
function TeamplayTeamColors()
    ffi.C.TeamplayTeamColors();
end

--- Set team color
-- @param team Team Number
-- @param color Color
function SetTeamColor(team, color)
    if not isnumber(team) then error("Paramater team must be a number."); end
    if not iscolor(color) then error("Paramater color must be a Color instance."); end
    local r;
    local g;
    local b;
    r,g,b = color.ToRGB();
    ffi.C.SetTeamColor(team, r, g, b);
end

--- Clear team color on given team
-- @param team Team Number
function ClearTeamColor(team)
    if not isnumber(team) then error("Paramater team must be a number."); end
    ffi.C.ClearTeamColor(team);
end

--- Get a position near
-- @param Pos Vector location
-- @param MinRadiusAway Minimum radius
-- @param MaxRadiusAway Maximum radius
-- @return Vector location
function GetPositionNear(Pos, MinRadiusAway, MaxRadiusAway)
    if not isvector(Pos) then error("Paramater Pos must be a Vector."); end
    if not isnumber(MinRadiusAway) then error("Paramater MinRadiusAway must be a number."); end
    if not isnumber(MaxRadiusAway) then error("Paramater MaxRadiusAway must be a number."); end
    return ffi.C.GetPositionNear(Pos, MinRadiusAway, MaxRadiusAway);
end

--- Get the player ODF
-- @param TeamNum Team number
-- @param RType RandomizeType
-- @return Player ODF
function GetPlayerODF(TeamNum, RType)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    if RType == null then RType = RandomizeType.Randomize_None; end
    if not israndomizetype(RType) then error("Paramater RType must be a RandomizeType."); end
    return tostring(ffi.C.GetPlayerODF(TeamNum, RType));
end

--- Get a position at a radius from the given location along an angle
-- @param CenterPos Vector location
-- @param Radius Radius
-- @param Angle Angle
-- @return Vector location
function SetCircularPos(CenterPos, Radius, Angle)
    --if not isvector(CenterPos) then error("Paramater CenterPos must be a Vector."); end
    --if not isnumber(Radius) then error("Paramater Radius must be a number."); end
    --if not isnumber(Angle) then error("Paramater Angle must be a number."); end
    --local NewPos = Vector();
    --ffi.C.SetCircularPos(CenterPos, Radius, Angle, NewPos);
    --return NewPos;
    
    return GetCircularPos(CenterPos, Radius, Angle);
end

function GetCircularPos(CenterPos, Radius, Angle)
    return GetCircularPos(CenterPos, Radius, Angle);
end

--- Get safest spawnpoint vector
-- @return Vector location
function GetSafestSpawnpoint()
    return ffi.C.GetSafestSpawnpoint();
end

--- Get spawnpoint vector for team
-- @param TeamNum Team number
-- @return Vector location
function GetSpawnpoint(TeamNum)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    return ffi.C.GetSpawnpoint(TeamNum);
end

--- Get spawnpoint handle for team
-- @param TeamNum Team number
-- @return Handle spawn point
function GetSpawnpointHandle(TeamNum)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    return GameObject.new(ffi.C.GetSpawnpointHandle(TeamNum));
end

--- Returns a random spawnpoint that 'looks' safe. [Nobody within 100 meters]
-- @param TeamNum Team number, -1 default
-- @return Vector location
function GetRandomSpawnpoint(TeamNum)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    if TeamNum == nil then TeamNum = -1; end
    return ffi.C.GetRandomSpawnpoint(TeamNum);
end

--- Set the message in the timer box
-- @param message
function SetTimerBox(message)
    if not isstring(message) then error("Paramater message must be a string."); end
    ffi.C.SetTimerBox(tocstring(message));
end

--- Add message to the message box
-- @param msg
function AddToMessagesBox(msg)
    if not isstring(msg) then error("Paramater msg must be a string."); end
    ffi.C.AddToMessagesBox(tocstring(msg));
end

--- Add to score of GameObject
-- @param team GameObject instance.
-- @param delta Score to add.
function AddScore(team, delta)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isnumber(delta) then error("Paramater delta must be GameObject instance."); end
    ffi.C.AddScore(team, delta)
end

--- Get local player team number
-- @return Team number
function GetLocalPlayerTeamNumber()
    return ffi.C.GetLocalPlayerTeamNumber();
end

--- Get local player's DPID
-- @return DPID / DWORD / unsigned long
function GetLocalPlayerDPID()
    return ffi.C.GetLocalPlayerDPID();
end

--- Note game over by timelimit
function NoteGameoverByTimelimit()
    ffi.C.NoteGameoverByTimelimit()
end

--- Note game over by kill limit
-- @param h GameObject
function NoteGameoverByKillLimit(h)
    if not isgameobject(h) then error("Paramater h must be GameObject instance."); end
    ffi.C.NoteGameoverByKillLimit(h.handle)
end

--- Note game over by score
-- @param h GameObject
function NoteGameoverByScore(h)
    if not isgameobject(h) then error("Paramater h must be GameObject instance."); end
    ffi.C.NoteGameoverByScore(h.handle)
end

--- Note game over by last with base
-- @param h GameObject
function NoteGameoverByLastWithBase(h)
    if not isgameobject(h) then error("Paramater h must be GameObject instance."); end
    ffi.C.NoteGameoverByLastWithBase(h.handle)
end

--- Note game over by last team with base
-- @param Teamgroup Team number
function NoteGameoverByLastTeamWithBase(Teamgroup)
    if not isnumber(Teamgroup) then error("Paramater Teamgroup must be a number."); end
    ffi.C.NoteGameoverByLastTeamWithBase(Teamgroup)
end

--- Note game over by no bases
function NoteGameoverByNoBases()
    ffi.C.NoteGameoverByNoBases()
end

--- Do gameover after X time
-- @param SecondsFromNow Seconds from now
function DoGameover(SecondsFromNow)
    if not isnumber(SecondsFromNow) then error("Paramater SecondsFromNow must be a number."); end
    ffi.C.DoGameover(SecondsFromNow)
end

--- Lock in MP race because someone started that team group
-- In Teamplay, this is a reporting by the DLL that a teamgroup (0,1) has built a recycler (or otherwise locked in a race) of the given race ('i' or 'f')
-- @param WhichTeamGroup Teamgroup
-- @param RaceIdentifier Race character
function SetMPTeamRace(WhichTeamGroup, RaceIdentifier)
    if not isnumber(WhichTeamGroup) then error("Paramater WhichTeamGroup must be a number."); end
    if not isstring(RaceIdentifier) then error("Paramater RaceIdentifier must be a string / character."); end
    ffi.C.SetMPTeamRace(WhichTeamGroup, RaceIdentifier);
end

--- Get user target
-- @param TeamNum Team number
-- @return User on team's target GameObject
function GetUserTarget(TeamNum)
    if not isnumber(TeamNum) then error("Paramater TeamNum must be a number."); end
    return GameObject.new(ffi.C.GameUserTarget(TeamNum));
end

--- Execute console command
-- @param pStr Command
-- @param bShowOutput Show output, optional, default false
function IFace_ConsoleCmd(pStr, bShowOutput)
    if not isstring(pStr) then error("Paramater pStr must be a string."); end
    ffi.C.IFace_ConsoleCmd(tocstring(pStr), not bShowOutput);
end

--- Print text in chat box with color
-- @param message Text
-- @param color Color for text
function AddToMessagesBox2(message, color)
    if not isstring(message) then error("Paramater message must be a string."); end
    if not iscolor(color) then error("Paramater color must be an instance of Color."); end
    ffi.C.AddToMessagesBox2(tocstring(message), color:ToColorLong())
end

--- Set string across network
-- Only has effect on host, could take as long as 150 turns to send data
-- Do not use this unless you are damn sure you know what you are doing
-- This might not even work anymore in 1.3
-- @param name Name of the variable
-- @param value Value for the variable
function Network_SetString(name, value)
    if not isstring(name) then error("Paramater name must be a string."); end
    if not isstring(value) then error("Paramater value must be a string."); end
    ffi.C.Network_SetString(tocstring(name), tocstring(value));
end

--- Set integer across network
-- Only has effect on host, could take as long as 150 turns to send data
-- Do not use this unless you are damn sure you know what you are doing
-- This might not even work anymore in 1.3
-- @param name Name of the variable
-- @param value Value for the variable
function Network_SetInteger(name, value)
    if not isstring(name) then error("Paramater name must be a string."); end
    if not isnumber(value) then error("Paramater value must be a number."); end
    ffi.C.Network_SetInteger(tocstring(name), value);
end

--- Get the first empty group
-- @param t Team number
-- @return Group number
function GetFirstEmptyGroup(t)
    if not isnumber(t) then error("Paramater t must be a number."); end
    return ffi.C.GetFirstEmptyGroup(t);
end


--- Does this odf exist?
-- @param odf Filename, omit the file extension
-- @return Boolean
function DoesODFExist(odf)
    if not isstring(odf) then error("Paramater odf must be a string."); end
    return ffi.C.DoesODFExist(tocstring(odf));
end

--- Look up string in localize table
-- @param Src Localize key
-- @param size Max size of string, defaults to 1024
-- @return Lookup result string
function TranslateString2(Src, size)
    if not isstring(Src) then error("Paramater Src must be a string."); end
    local passIn;
    if size == nil then size = 1024; end
    if size < 1025 then
        passIn = msgBuffer;
    else
        passIn = ffi.new("char[?]",size + 1);
    end
    ffi.fill(passIn,size + 1);
    ffi.C.TranslateString2(passIn, size, tocstring(Src));
    return tostring(passIn);
end

--- We want PlayerEjected messages for all units and not just players
-- This is for 1.3 Bot DMs
function WantBotKillMessages()
    ffi.C.WantBotKillMessages();
end

--- Set the TPS of the mission/game
-- @param newRate New TPS value, accepts 10, 15, 20, or 30
function EnableHighTPS(newRate)
    if not isnumber(newRate) then error("Paramater newRate must be a number."); end
    if newRate ~= 10 and newRate ~= 15 and newRate ~= 20 and newRate ~= 30 then error("Paramater newRate must be 10, 15, 20, or 30."); end
    ffi.C.EnableHighTPS(newRate);
end

--- Get object being inspected by local user
-- This is not MP safe unless you are very carful with what you are doing
-- @return GameObject being inspected
function GetLocalUserInspectHandle()
    return GameObject.new(ffi.C.GetLocalUserInspectHandle());
end

--- Get object being selected by local user
-- This is not MP safe unless you are very carful with what you are doing
-- @return GameObject being selected
function GetLocalUserSelectHandle()
    return GameObject.new(ffi.C.GetLocalUserSelectHandle());
end

--- Note gameover with custom message
-- @param pString Message
function NoteGameoverWithCustomMessage(pString)
    if not isstring(pString) then error("Paramater pString must be a string."); end
    ffi.C.NoteGameoverWithCustomMessage(tocstring(pString));
end

--- Get group count
-- @param team Team number
-- @param group Group number
-- @return Count of items in group
function GetGroupCount(team, group)
    if not isnumber(team) then error("Paramater team must be a number."); end
    if not isnumber(group) then error("Paramater group must be a number."); end
    return ffi.C.GetGroupCount(team, group);
end

--- Does this file exist in loadable assets?
-- @param filename Filename
-- @return Boolean does the file exist?
function DoesFileExist(filename)
    if not isstring(filename) then error("Paramater filename must be a string."); end
    return ffi.C.DoesFileExist(tocstring(filename));
end

--- Load a file and return itx text
-- @param filename Filename
-- @return File contents as string, nil if not found
function LoadFile(filename)
    local filenameC = tocstring(LoadFile);
    local bufSize = ffi.new("unsigned int[1]");
    local success = ffi.C.LoadFile(filename, nil, bufSize);
    if not success then return nil; end
    local pData = ffi.new("char[?]",tonumber(bufSize) + 1);
    ffi.fill(pData, tonumber(bufSize) + 1);
    success = ffi.C.LoadFile(filename, pData, bufSize);
    if success then
        return tostring(pData);
    end
    return nil;
end

--- Set the map gravity
-- @param gravity Gravity to set, defaults to 12.5 if nil
function SetGravity(gravity)
    if gravity == nil then gravity = 12.5; end
    ffi.C.SetGravity(gravity);
end

--- Set if objects build from BuildObject are automaticly grouped
-- @param stopAutoGroup
function SetAutoGroupUnits(stopAutoGroup)
  ffi.C.SetAutoGroupUnits(not stopAutoGroup);
end

--- Is this terrain cell water?
-- @param posORx Position vector, or X coordinate
-- @param z Z coordinate if posORx is not a vector
-- @return Boolean Is this terrain cell water?
function TerrainIsWater(posORx, z)
    if isvector(posORx) then
        return ffa.C.TerrainIsWaterV(posOrx);
    elseif isnumber(posORx) and isnumber(z) then
        return ffa.C.TerrainIsWater(posOrx, z);
    else
        error("posORx must be vector or posORx and z must be numbers");
    end
end

--- Get various terrain data
-- @param pos Position vector
-- @param useWater Use water? defaults to false
-- @return Terrain (or water if selected) height
-- @return Normal vector
function TerrainGetHeightAndNormal(pos, useWater)
    if not isvector(pos) then error("Paramater pos must be a Vector."); end
    local outHeight = ffi.new("float[1]");
    local outNormal = Vector();--ffi.new("Vector[1]");
    local success = ffi.C.TerrainGetHeightAndNormal(pos, outHeight, outNormal, useWater);
    if success ~= nil then return nil; end
    return tonumber(outHeight), outNormal;
end

--- Get a usable directory for writing files, should be in mydocs
-- @return Path
function GetOutputPath()
    local bufSize = ffi.new("unsigned int[1]");
    local success = ffi.C.GetOutputPath(bufSize, nil);
    if not success then return nil; end
    local pData = ffi.new("char[?]",tonumber(bufSize) + 1);
    success = ffi.C.GetOutputPath(bufSize, pData);
    if not success then return nil; end
    return tostring(pData);
end

--- Get pathpoints
-- @param path Path name
-- @return Array of VECTOR_2Ds
function GetPathPoints(path)
    if not isstring(path) then error("Paramater path must be a string."); end
    local pathC = tocstring(path);
    local bufSize = ffi.new("int[0]");
    local sucess = ffi.C.GetPathPoints(pathC, bufSize, nil);
    if not success then return nil; end
    local pData = ffi.new("float[?]", tonumber(bufSize) * 2);
    sucess = ffi.C.GetPathPoints(pathC, bufSize, pData);
    if not success then return nil; end
    local paths = {};
    for itr = 1,tonumber(bufSize) + 1,2 do
        table.insert(paths, VECTOR_2D(pData[itr+1],pData[iter+2]));
    end
    return paths;
end

--- AllowRandomTracks if music, rather than those listed in the TRN
-- Must be called immediatly
-- @param allow Boolean to allow or not, defaults to true in the C++ DLL but here it defaults false
function AllowRandomTracks(allow)
    ffi.C.AllowRandomTracks(allow)
end

--- Set FFA team colors
-- @param type TEAMCOLOR_TYPE
function SetFFATeamColors(type)
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    ffi.C.SetFFATeamColors(type);
end

--- Set TeamStrat team colors
-- @param type TEAMCOLOR_TYPE
function SetTeamStratColors(type)
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    ffi.C.SetTeamStratColors(type);
end

--- Get FFA team colors
-- @param type TEAMCOLOR_TYPE
-- @param team Team number
-- @return Color
function GetFFATeamColor(type, team)
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    local r = ffi.new("int[1]");
    local g = ffi.new("int[1]");
    local b = ffi.new("int[1]");
    ffi.C.GetFFATeamColor(type, team, r, g, b);
    return Color.new(tonumber(r),tonumber(g),tonumber(b));
end

--- Get TeamStrat team colors
-- @param type TEAMCOLOR_TYPE
-- @param team Team number
-- @return Color
function GetTeamStratColor(type, team)
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    if not isnumber(team) then error("Paramater team must be a number"); end
    local r = ffi.new("int[1]");
    local g = ffi.new("int[1]");
    local b = ffi.new("int[1]");
    ffi.C.GetTeamStratColor(type, team, r, g, b);
    return Color.new(tonumber(r),tonumber(g),tonumber(b));
end

--- Swap TeamStrat colors
function SwapTeamStratColors()
    ffi.C.SwapTeamStratColors();
end

--- Are team colors FFA?
-- @return Boolean
function GetTeamColorsAreFFA()
    return ffi.C.GetTeamColorsAreFFA();
end

--- Set team colors
-- @param type TEAMCOLOR_TYPE
function SetTeamColors(type)
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    return ffi.C.SetTeamColors(type);
end

--- Add power
-- @param team Team
-- @param v Add power
-- @return Power level
function AddPower(team, v)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(v) then error("Paramater v must be a number"); end
    return ffi.C.AddPower(team, v)
end

--- Set power
-- @param team Team
-- @param v New power
-- @return Power level
function SetPower(team, v)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(v) then error("Paramater v must be a number"); end
    return ffi.C.SetPower(team, v)
end

--- Get power
-- @param team Team
-- @return Power level
function GetPower(team)
    if not isnumber(team) then error("Paramater team must be a number"); end
    return ffi.C.GetPower(team)
end

--- Get max power
-- @param team Team
-- @return Power level
function GetMaxPower(team)
    if not isnumber(team) then error("Paramater team must be a number"); end
    return ffi.C.GetMaxPower(team)
end

--- Add max scrap
-- @param team Team
-- @param ammount Ammount
function AddMaxScrap(team, ammount)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(ammount) then error("Paramater ammount must be a number"); end
    ffi.C.AddMaxScrap(team, ammount)
end

--- Add max power
-- @param team Team
-- @param ammount Ammount
function AddMaxPower(team, ammount)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(ammount) then error("Paramater ammount must be a number"); end
    ffi.C.AddMaxPower(team, ammount)
end

--- Set max scrap
-- @param team Team
-- @param ammount Ammount
function SetMaxScrap(team, ammount)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(ammount) then error("Paramater ammount must be a number"); end
    ffi.C.SetMaxScrap(team, ammount)
end

--- Set max power
-- @param team Team
-- @param ammount Ammount
function SetMaxPower(team, ammount)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isnumber(ammount) then error("Paramater ammount must be a number"); end
    ffi.C.SetMaxPower(team, ammount)
end

--- Get TeamStrat Individual Color
-- @param type TEAMCOLOR_TYPE
-- @param team Team number
-- @return Color
function GetTeamStratIndividualColor(type, team)
    if not isnumber(team) then error("Paramater team must be a number"); end
    if not isteamcolortype(type) then error("Paramater type must be a TEAMCOLOR_TYPE."); end
    local r = ffi.new("int[1]");
    local g = ffi.new("int[1]");
    local b = ffi.new("int[1]");
    ffi.C.GetTeamStratIndividualColor(type, team, r, g, b);
    return Color.new(tonumber(r),tonumber(g),tonumber(b));
end
--]]

--[[
--- Is this team allied?
-- @param t1 Team 1
-- @param t2 Team 2
-- @return Boolean
function IsTeamAllied(t1, t2)
    if not isnumber(t1) then error("Paramater t1 must be a number"); end
    if not isnumber(t2) then error("Paramater t2 must be a number"); end
    return ffi.C.IsTeamAllied(t1, t2);
end

--- Spawn birds
-- @param group Group number
-- @param count Count of birds
-- @param odf ODF Filename
-- @param t TeamNum
-- @param path Path name
function SpawnBirds(group, count, odf, t, path)
    if not isnumber(group) then error("Paramater group must be a number"); end
    if not isnumber(count) then error("Paramater count must be a number"); end
    if not isstring(odf) then error("Paramater odf must be a string"); end
    if not isnumber(t) then error("Paramater t must be a number"); end
    if not isstring(path) then error("Paramater path must be a string"); end
    ffi.C.SpawnBirds(group, count, tocstring(odf), t, tocstring(path));
end

--- Spawn birds
-- @param group Group number
-- @param count Count of birds
-- @param odf ODF Filename
-- @param t TeamNum
-- @param startObj Starting GameObject
-- @param destObj Destination GameObject
function SpawnBirds(group, count, odf, t, startObj, destObj)
    if not isnumber(group) then error("Paramater group must be a number"); end
    if not isnumber(count) then error("Paramater count must be a number"); end
    if not isstring(odf) then error("Paramater odf must be a string"); end
    if not isnumber(t) then error("Paramater t must be a number"); end
    if not isgameobject(startObj) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(destObj) then error("Paramater self must be GameObject instance."); end
    ffi.C.SpawnBirds(group, count, tocstring(odf), t, startObj, destObj);
end

--- Remove birds
-- @param group Group number
function RemoveBirds(group)
    if not isnumber(group) then error("Paramater group must be a number"); end
    ffi.C.RemoveBirds(group);
end

--- Get Camera Position and Heading
-- @return Camera Position
-- @return Camera Direction
function GetCameraPosition()
    local pos = Vector();
    local dir = Vector();
    ffi.C.GetCameraPosition(pos, dir);
    return pos,dir;
end

--- Set Camera Position and Heading
-- @param pos Camera Position
-- @param dir Camera Direction
function SetCameraPosition(pos, dir)
    if not isvector(pos) then error("Paramater pos must be Vector instance."); end
    if not isvector(dir) then error("Paramater dir must be Vector instance."); end
    ffi.C.SetCameraPosition(pos, dir);
end
 
--- Reset camera position
function ResetCameraPosition()
    ffi.C.ResetCameraPosition();
end
 
 --- Is the Camera Ready?
function CameraReady()
    return ffi.C.CameraReady();
end

--- Camera Path
-- @param path_name Path Name
-- @param height Height
-- @param speed Speed
-- @param target_handle Target GameObject
-- @return True when done
function CameraPath(path_name, height, speed, target_handle)
    if not isstring(path_name) then error("Paramater path_name must be a string"); end
    if not isnumber(height) then error("Paramater height must be a number"); end
    if not isnumber(speed) then error("Paramater speed must be a number"); end
    if not isgameobject(target_handle) then error("Paramater target_handle must be GameObject instance."); end
    return ffi.C.CameraPath(tocstring(path_name), height, speed, target_handle:GetHandle());
end

--- Camera Path Dir
-- @param path_name Path Name
-- @param height Height
-- @param speed Speed
-- @return True when done
function CameraPathDir(path_name, height, speed, target_handle)
    if not isstring(path_name) then error("Paramater path_name must be a string"); end
    if not isnumber(height) then error("Paramater height must be a number"); end
    if not isnumber(speed) then error("Paramater speed must be a number"); end
    return ffi.C.CameraPathDir(tocstring(path_name), height, speed);
end

--- PanDone
-- @return Boolean
function PanDone()
    return ffi.C.PanDone();
end

--- CameraObject
-- @param object_handle Object to Camera
-- @param i offset in centimeters
-- @param j offset in centimeters
-- @param k offset in centimeters
-- @param target_handle Target Object
-- @return Boolean
function CameraObject(object_handle, i, j, k, target_handle)
    if not isgameobject(object_handle) then error("Paramater object_handle must be GameObject instance."); end
    if not isnumber(i) then error("Paramater i must be a number"); end
    if not isnumber(j) then error("Paramater j must be a number"); end
    if not isnumber(k) then error("Paramater k must be a number"); end
    if not isgameobject(target_handle) then error("Paramater target_handle must be GameObject instance."); end
    return ffi.C.CameraObject(object_handle, i, j, k, target_handle);
end

--- CameraFinish
-- @return Boolean
function CameraFinish()
    return ffi.C.CameraFinish();
end

--- CameraCancelled
-- @return Boolean
function CameraCancelled()
    return ffi.C.CameraCancelled();
end

--- FreeCamera
-- @return Boolean
function FreeCamera()
    return ffi.C.FreeCamera();
end

--- FreeFinish
-- @return Boolean
function FreeFinish()
    return ffi.C.FreeFinish();
end

--- PlayMovie
-- @param name Name of max 20 characters
-- @return Boolean
function PlayMovie(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    local nameIn = ffi.new("char[20]");
    ffi.copy(nameIn, name); -- see if I need to truncate this beforehand
    return ffi.C.PlayMovie(nameIn);
end

--- StopMovie
function StopMovie()
    ffi.C.StopMovie();
end

--- PlayMove
-- @return Boolean
function PlayMove()
    return ffi.C.PlayMove();
end

--- PlayRecording
-- @param name Name of max 20 characters
-- @return Boolean
function PlayRecording(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    local nameIn = ffi.new("char[20]");
    ffi.copy(nameIn, name); -- see if I need to truncate this beforehand
    return ffi.C.PlayRecording(nameIn);
end

--- PlayRecordingUpdateCam
-- @param name Name of max 20 characters
-- @param updateCam Update Camera
-- @return Boolean
function PlayRecordingUpdateCam(name, updateCam)
    if not isstring(name) then error("Paramater name must be a string"); end
    local nameIn = ffi.new("char[20]");
    ffi.copy(nameIn, name); -- see if I need to truncate this beforehand
    return ffi.C.PlayRecordingU(nameIn, updateCam);
end

--- PlaybackVehicle
-- @param name Name of max 20 characters
-- @return Boolean
function PlaybackVehicle(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    local nameIn = ffi.new("char[20]");
    ffi.copy(nameIn, name); -- see if I need to truncate this beforehand
    return ffi.C.PlaybackVehicle(nameIn);
end


--- load an interface definition
-- @param n String name
function IFace_Exec(n)
    if not isstring(n) then error("Paramater n must be a string"); end
    ffi.C.IFace_Exec(tocstring(n));
end

--- activate a control
-- @param n String name
function IFace_Activate(n)
    if not isstring(n) then error("Paramater n must be a string"); end
    ffi.C.IFace_Activate(tocstring(n));
end

--- deactivate a control
-- @param n String name
function IFace_Deactivate(n)
    if not isstring(n) then error("Paramater n must be a string"); end
    ffi.C.IFace_Deactivate(tocstring(n));
end

--- create a mission command
-- @param n String name
function IFace_CreateCommand(n)
    if not isstring(n) then error("Paramater n must be a string"); end
    ffi.C.IFace_CreateCommand(tocstring(n));
end

--- create a mission string variable
-- @param name Var name
-- @param value Value
function IFace_CreateString(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isstring(value) then error("Paramater value must be a string"); end
    ffi.C.IFace_CreateString(tocstring(name), tocstring(value));
end

--- set a mission string variable
-- @param name Var name
-- @param value Value
function IFace_SetString(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isstring(value) then error("Paramater value must be a string"); end
    ffi.C.IFace_SetString(tocstring(name), tocstring(value));
end

--- get a mission string variable
-- @param name Var name
-- @param maxSize Maximum length of string
-- @return Value
function IFace_GetString(name, maxSize)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(maxSize) then error("Paramater maxSize must be a number"); end
    local passIn;
    if size == nil then size = 1024; end
    if size < 1025 then
        passIn = msgBuffer;
    else
        passIn = ffi.new("char[?]",size + 1);
    end
    ffi.fill(passIn,size + 1);
    ffi.C.IFace_GetString(tocstring(name), passIn, maxSize);
    return tostring(passIn);
end

--- create a mission integer variable
-- @param name Var name
-- @param value Value
function IFace_CreateInteger(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(value) then error("Paramater value must be a number"); end
    ffi.C.IFace_CreateInteger(tocstring(name), tocstring(value));
end

--- set a mission integer variable
-- @param name Var name
-- @param value Value
function IFace_SetInteger(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(value) then error("Paramater value must be a number"); end
    ffi.C.IFace_SetInteger(tocstring(name), tocstring(value));
end

--- get a mission integer variable
-- @param name Var name
-- @return Value
function IFace_SetInteger(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    ffi.C.IFace_GetInteger(tocstring(name));
end

-- set the range of a mission integer variable
-- @param name Var name
-- @param low Low
-- @param high High
function IFace_SetIntegerRange(name, low, high)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(low) then error("Paramater low must be a number"); end
    if not isnumber(high) then error("Paramater high must be a number"); end
    ffi.C.IFace_SetIntegerRange(tocstring(name), low, high);
end


--- create a mission float variable
-- @param name Var name
-- @param value Value
function IFace_CreateFloat(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(value) then error("Paramater value must be a number"); end
    ffi.C.IFace_CreateFloat(tocstring(name), tocstring(value));
end

--- set a mission float variable
-- @param name Var name
-- @param value Value
function IFace_SetFloat(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(value) then error("Paramater value must be a number"); end
    ffi.C.IFace_SetFloat(tocstring(name), tocstring(value));
end

--- get a mission float variable
-- @param name Var name
-- @return Value
function IFace_GetFloat(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    ffi.C.IFace_GetFloat(tocstring(name));
end

--- Clear listbox
-- @param name Listbox Name
function IFace_ClearListBox(name)
    if not isstring(name) then error("Paramater name must be a string"); end
    ffi.C.IFace_ClearListBox(tocstring(name));
end

--- Add listbox item
-- @param name Listbox Name
-- @param value Listbox Item
function IFace_AddTextItem(name, value)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isstring(value) then error("Paramater name must be a string"); end
    ffi.C.IFace_AddTextItem(tocstring(name));
end

--- Get selected listbox item
-- @param name Listbox Name
-- @param maxSize Maximum length of string
-- @return Value
function IFace_GetSelectedItem(name, maxSize)
    if not isstring(name) then error("Paramater name must be a string"); end
    if not isnumber(maxSize) then error("Paramater maxSize must be a number"); end
    local passIn;
    if size == nil then size = 1024; end
    if size < 1025 then
        passIn = msgBuffer;
    else
        passIn = ffi.new("char[?]",size + 1);
    end
    ffi.fill(passIn,size + 1);
    ffi.C.IFace_GetSelectedItem(tocstring(name), passIn, maxSize);
    return tostring(passIn);
end



--- FailMission
-- @param t Time of failure
-- @param fileName Debriefing name (optional)
function FailMission(t, fileName)
    if not isnumber(t) then error("Paramater t must be a number"); end
    if fileName ~= nil and not isstring(fileName) then error("Paramater fileName must be a string or nil"); end
    MisnImport.FailMission(t, fileName);
end

--- SucceedMission
-- @param t Time of success
-- @param fileName Debriefing name (optional)
function SucceedMission(t, fileName)
    if not isnumber(t) then error("Paramater t must be a number"); end
    if fileName ~= nil and not isstring(fileName) then error("Paramater fileName must be a string or nil"); end
    MisnImport.SucceedMission(t, fileName);
end

--- ChangeSide
function SucceedMission()
    MisnImport.ChangeSide();
end

--- SucceedMission
-- @param t Time of success
-- @param fileName Debriefing name (optional)
function SucceedMission(t, fileName)
    if not isnumber(t) then error("Paramater t must be a number"); end
    if fileName ~= nil and not isstring(fileName) then error("Paramater fileName must be a string or nil"); end
    MisnImport.SucceedMission(t, fileName);
end

--- AddScrap
-- @param t Team
-- @param v Ammount
-- @return Current
function AddScrap(t, v)
    if not isnumber(t) then error("Paramater t must be a number"); end
    if not isnumber(v) then error("Paramater v must be a number"); end
    return MisnImport.AddScrap(t, v);
end

--- SetScrap
-- @param t Team
-- @param v Ammount
-- @return Current
function SetScrap(t, v)
    if not isnumber(t) then error("Paramater t must be a number"); end
    if not isnumber(v) then error("Paramater v must be a number"); end
    return MisnImport.SetScrap(t, v);
end

--- GetScrap
-- @param t Team
-- @return Current
function GetScrap(t)
    if not isnumber(t) then error("Paramater t must be a number"); end
    return MisnImport.GetScrap(t);
end

--- GetMaxScrap
-- @param t Team
-- @return Current
function GetMaxScrap(t)
    if not isnumber(t) then error("Paramater t must be a number"); end
    return MisnImport.GetMaxScrap(t);
end

--- GetTug
-- @param self GameObject instance.
-- @return Tug of GameObject, or nil
function GameObject.GetTug(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
	local retVal = MisnImport.GetTug(self:GetHandle());
	if retVal == 0 then return nil; end
	return GameObject.new(retVal);
end

--- GetTug
-- @param self GameObject instance.
-- @return Tug of GameObject, or nil
function GameObject.HasCargo(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
	return MisnImport.HasCargo(self:GetHandle());
end

--- GetDistanceObject
-- @param self GameObject instance.
-- @param h2 GameObject instance.
-- @return distance
function GameObject.GetDistanceObject(self, h2)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isgameobject(h2) then error("Paramater h2 must be GameObject instance."); end
	return MisnImport.GetDistanceObject(self:GetHandle(), h2:GetHandle());
end

--- GetDistancePath
-- @param self GameObject instance.
-- @param path Path name.
-- @param point Path point number.
-- @return distance
function GameObject.GetDistancePath(self, path, point)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    if not isstring(path) then error("Paramater path must be a string"); end
    if not isnumber(point) then error("Paramater point must be a number"); end
	return MisnImport.GetDistancePath(self:GetHandle(), path, point);
end

--	float MisnImport.GetDistancePathPtr(int &h1, AiPath *path, int point);

--- GetDistanceObject
-- @param self GameObject instance.
-- @return Near Object
function GameObject.GetNearestObject(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = MisnImport.GetNearestObject(self:GetHandle());
    if retVal == 0 then return nil; end
    return GameObject.new(retVal);
end

--- GetNearestVehicleObject
-- @param self GameObject instance.
-- @return Near Object
function GameObject.GetNearestVehicleObject(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = MisnImport.GetNearestVehicleObject(self:GetHandle());
    if retVal == 0 then return nil; end
    return GameObject.new(retVal);
end

--- GetNearestVehiclePath
-- @param path Path name.
-- @param point Path point number.
-- @return Near Object
function GameObject.GetNearestVehiclePath(path, point)
    if not isstring(path) then error("Paramater path must be a string"); end
    if not isnumber(point) then error("Paramater point must be a number"); end
    local retVal = MisnImport.GetNearestVehiclePath(path, point);
    if retVal == 0 then return nil; end
    return GameObject.new(retVal);
end

--- GetNearestBuilding
-- @param self GameObject instance.
-- @return Near Object
function GameObject.GetNearestBuilding(self)
    if not isgameobject(self) then error("Paramater self must be GameObject instance."); end
    local retVal = MisnImport.GetNearestBuilding(self:GetHandle());
    if retVal == 0 then return nil; end
    return GameObject.new(retVal);
end
	
function GameObject.GetNearestEnemyH(self)
	return MisnImport.GetNearestEnemy(self:GetHandle());
end

--]]




local CustomSavableTypes = {};
local TemporarySavableTypeMap = nil; -- maps name to ID number

function RegisterCustomSavableType(obj)
    if obj == nil or obj.__type == nil then error("Custom type malformed, no __type"); end
    local typeT = {};
    if obj.Save ~= nil then
        typeT.Save = obj.Save;
    else
        typeT.Save = function() end
    end
    if obj.Load ~= nil then
        typeT.Load = obj.Load;
    else
        typeT.Load = function() end
    end
    if obj.PostLoad ~= nil then
        typeT.PostLoad = obj.PostLoad;
    else
        typeT.PostLoad = function() end
    end
    if obj.BulkSave ~= nil then
        typeT.BulkSave = obj.BulkSave;
    else
        typeT.BulkSave = function() end
    end
    if obj.BulkLoad ~= nil then
        typeT.BulkLoad = obj.BulkLoad;
    else
        typeT.BulkLoad = function() end
    end
    CustomSavableTypes[obj.__type] = typeT;
end

function SimplifyForSave(...)
    local output = {}; -- output array
    local count = select ("#", ...); -- get count of params
    for k = 1,count,1 do  -- loop params via count
        local v = select(k,...); -- get Kth paramater, store in v
        if istable(v) then -- it's a table, start special logic
            if CustomSavableTypes[v.__type] ~= nul then
                local specialTypeTable = {};
                local typeIndex = TemporarySavableTypeMap[v.__type];
                PrintConsoleMessage("Type index for " .. v.__type .. " is " .. tostring(typeIndex));
                specialTypeTable["*custom_type"] = typeIndex;
                specialTypeTable["*data"] = {CustomSavableTypes[v.__type].Save(v)};
                table.insert(output, specialTypeTable);
            else
                local newTable = {};
                for k2, v2 in pairs( v ) do 
                    newTable[k2] = SimplifyForSave(v2);
                end
                table.insert(output, newTable);
            end
        else -- it's not a table, really simple
            table.insert(output, v);
        end
    end
    return unpack(output);
end

function DeSimplifyForLoad(...)
    local output = {}; -- output array
    local count = select ("#", ...); -- get count of params
    for k = 1,count,1 do  -- loop params via count
        local v = select(k,...); -- get Kth paramater, store in v
        if istable(v) then -- it's a table, start special logic
            if v["*custom_type"] ~= nil then
                local typeName = TemporarySavableTypeMap[v["*custom_type"]];
                local typeObj = CustomSavableTypes[typeName];
                table.insert(output, typeObj.Load(unpack(v["*data"])));
            else
                local newTable = {};
                for k2, v2 in pairs( v ) do 
                    newTable[k2] = DeSimplifyForLoad(v2);
                end
                table.insert(output, newTable);
            end
        else -- it's not a table, really simple
            table.insert(output, v);
        end
    end
    return unpack(output);
end

function InitialSetup()
    --local here;
    --local coming;
    --here,coming = friendTank:CountThreats();
    --PrintConsoleMessage("Here: " .. here .. " Coming: " .. coming);
    
    --MissionData.TestData = {};
    --MissionData.TestData.Spawned = false;
    --MissionData.TestData.FreshLoad = false;
    hook.CallAllNoReturn( "InitialSetup" );
end

function Update(timeStep)
  -- be sure to pass 1/tps to simulate the timeStep bz1 would normally provide
end

function Execute()
	--io.write("Update, from ",_VERSION,"!\n")
  --PrintConsoleMessage("Update, from " .. _VERSION .. "!\n");
  
    --PrintConsoleMessage(tostring(MisnImport.GetNearestBuilding(GetPlayerHandle(1):GetHandle())));
    
    --if not MissionData.TestData.Spawned then
    --    MissionData.TestData.friendTank = BuildObject("ivtank",1,Vector());
    --    MissionData.TestData.enemyTank = BuildObject("ivtank",2,Vector(100,100,100));
    --    MissionData.TestData.enemyTank:Attack(MissionData.TestData.friendTank);
    --    MissionData.TestData.Spawned = true;
    --end
    
    --if MissionData.TestData.FreshLoad then
    --    MissionData.TestData.friendTank:EjectPilot();
    --    MissionData.TestData.enemyTank:EjectPilot();
    --end
        
    hook.CallAllNoReturn( "Execute" );
    --MissionData.GameTurn = MissionData.GameTurn + 1;
end

function Save()
    TemporarySavableTypeMap = {};

    PrintConsoleMessage("Beginning save code");

    local saveData = {};
    PrintConsoleMessage("Save Data Container ready");

    PrintConsoleMessage("Saving custom types map");
    local CustomSavableTypesCounter = 1;
    local CustomSavableTypeTmpTable = {};
    for k,v in pairs(CustomSavableTypes) do
        CustomSavableTypeTmpTable[CustomSavableTypesCounter] = k;
        TemporarySavableTypeMap[k] = CustomSavableTypesCounter;
        PrintConsoleMessage("[" .. CustomSavableTypesCounter .. "] = " .. k);
        CustomSavableTypesCounter = CustomSavableTypesCounter + 1;
    end
    table.insert(saveData, CustomSavableTypeTmpTable); -- Write TmpID -> Name map
    
    local CustomSavableTypeDataTmpTable = {};
    for idNum,name in ipairs(CustomSavableTypeTmpTable) do
        local entry = CustomSavableTypes[name];
        if entry.BulkSave ~= nil and isfunction(entry.BulkSave) then
          CustomSavableTypeDataTmpTable[idNum] = {SimplifyForSave(entry.BulkSave())};
        else
          CustomSavableTypeDataTmpTable[idNum] = {};
        end
    end
    table.insert(saveData, CustomSavableTypeDataTmpTable); -- Write TmpID -> Data map
    CustomSavableTypeDataTmpTable = nil;
    CustomSavableTypeTmpTable = nil;
    
    PrintConsoleMessage("Calling all hooked save functions");
    local hookResults = hook.CallSave();
    if hookResults ~= nil then
      table.insert(saveData, {SimplifyForSave(hookResults)});
    else
      table.insert(saveData, {});
    end
    
    PrintConsoleMessage("Unpacking and returning Save Data Container");
    
    TemporarySavableTypeMap = nil;
    
    return unpack(saveData);
end

function Load(...)
    local args = {...};

    TemporarySavableTypeMap = {};
    
    PrintConsoleMessage("Beginning load code");

    PrintConsoleMessage("Loading custom types map");
    TemporarySavableTypeMap = args[1];
    --local CustomSavableTypeTmpTable = args[1];
    --for k,v in pairs(CustomSavableTypeTmpTable) do
    --    TemporarySavableTypeMap[v] = k;
    --    PrintConsoleMessage("[" .. k .. "] = " .. v);
    --end
    PrintConsoleMessage("Loading custom types data");
    local CustomSavableTypeDataTmpTable = args[2];
    for idNum,data in ipairs(CustomSavableTypeDataTmpTable) do
        --local entry = CustomSavableTypes[CustomSavableTypeTmpTable[idNum]];
        local entry = CustomSavableTypes[TemporarySavableTypeMap[idNum]];
        if entry.BulkLoad ~= nil and isfunction(entry.BulkLoad) then
          entry.BulkLoad(DeSimplifyForLoad(unpack(data)));
        end
    end
    
    for k,y in pairs(GameObjectAltered) do
      PrintConsoleMessage(tostring(k) .. " = " .. tostring(y));
    end
    
    PrintConsoleMessage("Calling all hooked load functions");
    local LoadHookData = args[3];
    hook.CallLoad(DeSimplifyForLoad(unpack(LoadHookData)));

    TemporarySavableTypeMap = nil;
end

function PostLoad()

end

function AddObject(h)
    --hook.Call( "CreateObject", GameObject.new(h) );
    --hook.Call( "AddObject", GameObject.new(h) );
end

function DeleteObject(h)
    local object = GameObject.new(h);
    --hook.Call( "DeleteObject", object );
    if GameObjectAltered[object:GetHandle()] ~= nil then GameObjectAltered[object:GetHandle()] = nil; end
end

function PostRun()
    --hook.Call( "PostRun", GameObject.new(h) );
end

function AddPlayer(id, team, isNewPlayer)
    local returnValue = true;
    local hookResults = { hook.CallAll( "AddPlayer", id, team, isNewPlayer ) };
    if hookResults ~= nil then
      for k,v in pairs(hookResults) do
        returnValue = returnValue and v;
      end
    end
    
    return returnValue;
end

function DeletePlayer(id)
    hook.Call( "DeletePlayer", id );
end

--[[function PlayerEjected(deadObjectHandle)
    local retVal = hook.Call( "PlayerEjected", GameObject.new(deadObjectHandle));
    if retVal ~= nil then return retVal; else return 0; end
end

function ObjectKilled(deadObjectHandle, killersHandle)
    local retVal = hook.Call( "ObjectKilled", GameObject.new(deadObjectHandle), GameObject.new(killersHandle));
    if retVal ~= nil then return retVal; else return 1; end
end

function ObjectSniped(deadObjectHandle, killersHandle)
    local retVal = hook.Call( "ObjectSniped", GameObject.new(deadObjectHandle), GameObject.new(killersHandle));
    if retVal ~= nil then return retVal; else return 2; end
end

function GetNextRandomVehicleODF(team)
    local retVal = hook.Call( "GetNextRandomVehicleODF", team );
    if retVal ~= nil then return retVal; end
    return "someVehicle"
end

function SetWorld(nextWorld)
    --hook.Call( "SetWorld", nextWorld );
end

function ProcessCommand(crc)
    --hook.Call( "ProcessCommand", crc );
end

function SetRandomSeed(seed)
    --hook.Call( "SetRandomSeed", seed );
end
--]]

    --MissionData.GameTurn = 0;
    
    --RegisterCustomSavableType("Color", SaveColor, LoadColor);
    --RegisterCustomSavableType("AudioHandle", SaveAudioHandle, LoadAudioHandle);
    --RegisterCustomSavableType("ObjectDefinition", SaveObjectDefinition, LoadObjectDefinition);
    --RegisterCustomSavableType("GameObject", SaveGameObject, LoadGameObject, PostLoadGameObject);
    RegisterCustomSavableType(GameObject);
    --RegisterCustomSavableType("VECTOR_2D", SaveVECTOR_2D, LoadVECTOR_2D);
    --RegisterCustomSavableType("Vector", SaveVector, LoadVector);
    --RegisterCustomSavableType("Matrix", SaveMatrix, LoadMatrix);
    PrintConsoleMessage(tostring(#CustomSavableTypes) .. " Custom DataTypes");
    for k, v in ipairs(CustomSavableTypes) do
        local v = CustomSavableTypes[k];
        PrintConsoleMessage(tostring(SavableTypeNameToIndex[v.name]) .. " " .. tostring(v.name));
    end
    --PrintConsoleMessage("Loading TRN Data");
    --local MapDataFile = GetMapTRNFilename();
    --PrintConsoleMessage("TRN File: " .. MapDataFile);
    --local MapData = ObjectDefinition(MapDataFile);
    --MapData:Open();
    --local TRNScriptName = MapData:GetString("DLL","LuaMission");
    --MapData:Close();
    --PrintConsoleMessage("Script File: " .. TRNScriptName);
    --if TRNScriptName ~= nil and string.len(TRNScriptName) > 0 then
    --    Core.LoadFromBZ2(TRNScriptName);
    --else
    --    PrintConsoleMessage("TRN Script Not Set");
    --    Core.LoadFromBZ2("Test.lua");
    --    PrintConsoleMessage("Loaded Test.lua");
    --end
    
    PrintConsoleMessage("Loading Modules");
    
    --LogStack();
    require("hook");
    --LogStack();
    
    local MapLuaFile = string.gsub(GetMapTRNFilename(),"%.trn$",".lua");
    PrintConsoleMessage("Loading Lua: " .. MapLuaFile);
    if requireAsset(MapLuaFile) then
      PrintConsoleMessage("Loaded map lua");
    else
      PrintConsoleMessage("Failed to load map lua");
    end