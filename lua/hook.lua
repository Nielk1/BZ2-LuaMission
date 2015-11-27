-- Hook module stolen from Garrysmod
local PrintConsoleMessage = PrintConsoleMessage;
PrintConsoleMessage("Loading Hook Module");

local pairs      = pairs;
local table      = table;
local isfunction = isfunction;
local isstring   = isstring;
local unpack     = unpack;

module( "hook" )

local Hooks = {}
local SaveLoadHooks = {}

--[[---------------------------------------------------------
    Name: GetTable
    Desc: Returns a table of all hooks.
-----------------------------------------------------------]]
--function GetTable() return Hooks end


--[[---------------------------------------------------------
    Name: Add
    Args: string hookName, any identifier, function func
    Desc: Add a hook to listen to the specified event.
-----------------------------------------------------------]]
function Add( event_name, name, func )

	if ( not isfunction( func ) ) then return end
	if ( not isstring( event_name ) ) then return end

	if (Hooks[ event_name ] == nil) then
			Hooks[ event_name ] = {}
	end

	Hooks[ event_name ][ name ] = func

end

--[[---------------------------------------------------------
    Name: AddSaveLoad
    Args: string identifier, function save, function load, function postload
    Desc: Add a hook to listen to the specified event.
-----------------------------------------------------------]]
function AddSaveLoad( id, save, load, postload )
	if ( not isstring( id ) ) then error("Library ID for Save/Load hook required"); end
  --if ( save == nil and load == nil and postload == nil) then error("All function paramaters nil, if you would like to remove a Save/Load hook, use RemoveSaveLoad(id)"); end
  if ( save == nil and load == nil and postload == nil) then return end
  if ( save ~= nil and not isfunction( save ) ) then return end
  if ( load ~= nil and not isfunction( load ) ) then return end
  if ( postload ~= nil and not isfunction( postload ) ) then return end
  
	if (SaveLoadHooks[ id ] == nil) then
			SaveLoadHooks[ id ] = {};
	end

	SaveLoadHooks[ id ]['Save'] = save;
  SaveLoadHooks[ id ]['Load'] = load;
  SaveLoadHooks[ id ]['PostLoad'] = postload;
  
  PrintConsoleMessage("Added Save/Load hooks for " .. id);
end

--[[---------------------------------------------------------
    Name: RemoveSaveLoad
    Args: string identifier
    Desc: Removes the hook with the given indentifier.
-----------------------------------------------------------]]
function Remove( identifier )

	if ( not isstring( identifier ) ) then error("Library ID for Save/Load hook required"); end
	if ( not SaveLoadHooks[ identifier ] ) then return; end

	SaveLoadHooks[ identifier ] = nil;

end

--[[---------------------------------------------------------
    Name: CallSave
    Desc: Calls hooks associated with save.
-----------------------------------------------------------]]
function CallSave()

	--
	-- Run hooks
	--
	if ( SaveLoadHooks ~= nil ) then

    local ret = {};
    
		for k, v in pairs( SaveLoadHooks ) do 
			
			--if ( isstring( k ) ) then
				
				--
				-- If it's a string, it's cool
				--
        if v.Save ~= nil and isfunction(v.Save) then
          ret[k] = {v.Save()};
        else
          ret[k] = {};
        end

			--else

				--
				-- If the key isn't a string - we assume it to be an entity
				--
      --  SaveLoadHooks[ k ] = nil
			--end
		end
    
    return ret
	end
  
  return
	
end

--[[---------------------------------------------------------
    Name: CallLoad
    Desc: Calls hooks associated with load.
-----------------------------------------------------------]]
function CallLoad(SaveData)

	--
	-- Run hooks
	--
	if ( SaveLoadHooks ~= nil ) then

    local ret = {};
    
		for k, v in pairs( SaveLoadHooks ) do 
			
			--if ( isstring( k ) ) then
				
				--
				-- If it's a string, it's cool
				--
        if v.Load ~= nil and isfunction(v.Load) then
          v.Load(unpack(SaveData[k]));
        end

			--else

				--
				-- If the key isn't a string - we assume it to be an entity
				--
      --  SaveLoadHooks[ k ] = nil
			--end
		end
    
    return ret
	end
  
  return
	
end

--[[---------------------------------------------------------
    Name: Remove
    Args: string hookName, identifier
    Desc: Removes the hook with the given indentifier.
-----------------------------------------------------------]]
function Remove( event_name, name )

	if ( not isstring( event_name ) ) then return end
	if ( not Hooks[ event_name ] ) then return end

	Hooks[ event_name ][ name ] = nil

end

--[[---------------------------------------------------------
    Name: Run
    Args: string hookName, vararg args
    Desc: Calls hooks associated with the hook name.
-----------------------------------------------------------]]
--[[function Call( name, ... )

	--
	-- Run hooks
	--
	local HookTable = Hooks[ name ]
	if ( HookTable ~= nil ) then
	
		local a, b, c, d, e, f;
    
		for k, v in pairs( HookTable ) do 
			
			if ( isstring( k ) ) then
				
				--
				-- If it's a string, it's cool
				--
				a, b, c, d, e, f = v( ... )

			else

				--
				-- If the key isn't a string - we assume it to be an entity
				--
        HookTable[ k ] = nil
			end

			--
			-- Hook returned a value - it overrides the gamemode function
			--
			if ( a ~= nil ) then
				return a, b, c, d, e, f
			end
				
		end
	end
  
  return
	
end--]]

--[[---------------------------------------------------------
    Name: Run
    Args: string hookName, vararg args
    Desc: Calls hooks associated with the hook name.
-----------------------------------------------------------]]
--[[function CallAll( name, ... )

	--
	-- Run hooks
	--
	local HookTable = Hooks[ name ]
	if ( HookTable ~= nil ) then
	
		local a, b, c, d, e, f;

    local ret = {};
    
		for k, v in pairs( HookTable ) do 
			
			if ( isstring( k ) ) then
				
				--
				-- If it's a string, it's cool
				--
				a, b, c, d, e, f = v( ... )

			else

				--
				-- If the key isn't a string - we assume it to be an entity
				--
        HookTable[ k ] = nil
			end

			--
			-- Hook returned a value - it overrides the gamemode function
			--
			--if ( a ~= nil ) then
			--	return a, b, c, d, e, f
			--end
      
      if ( a ~= nil ) then
				table.insert(ret,a);
			end
				
		end
    
    if table.getn(ret) > 0 then return unpack(ret) end
	end
  
  return
	
end--]]

--[[---------------------------------------------------------
    Name: CallPreserveReturn
    Args: string hookName, vararg args
    Desc: Calls hooks associated with the hook name.
-----------------------------------------------------------]]
--[[function CallPreserveReturn( name, ... )

	--
	-- Run hooks
	--
	local HookTable = Hooks[ name ]
	if ( HookTable ~= nil ) then

    local ret = {};
    
		for k, v in pairs( HookTable ) do 
			
			if ( isstring( k ) ) then
				
				--
				-- If it's a string, it's cool
				--
				table.insert(ret,{v( ... )});

			else

				--
				-- If the key isn't a string - we assume it to be an entity
				--
        HookTable[ k ] = nil
			end
		end
    
    return ret
	end
  
  return
	
end--]]



--[[---------------------------------------------------------
    Name: CallPreserveReturn
    Args: string hookName, vararg args
    Desc: Calls hooks associated with the hook name.
-----------------------------------------------------------]]
--[[function CallPreserveReturn( name, ... )

	--
	-- Run hooks
	--
	local HookTable = Hooks[ name ]
	if ( HookTable ~= nil ) then

    local ret = {};
    
		for k, v in pairs( HookTable ) do 
			
			if ( isstring( k ) ) then
				
				--
				-- If it's a string, it's cool
				--
				table.insert(ret,{v( ... )});

			else

				--
				-- If the key isn't a string - we assume it to be an entity
				--
        HookTable[ k ] = nil
			end
		end
    
    return ret
	end
  
  return
	
end--]]


--[[---------------------------------------------------------
    Name: Run
    Args: string hookName, vararg args
    Desc: Calls hooks associated with the hook name.
-----------------------------------------------------------]]
function CallAllNoReturn( name, ... )

	--
	-- Run hooks
	--
	local HookTable = Hooks[ name ]
	if ( HookTable ~= nil ) then
	
		for k, v in pairs( HookTable ) do 
			
			if ( isstring( k ) ) then
				
				--
				-- If it's a string, it's cool
				--
				v( ... )

			else

				--
				-- If the key isn't a string - we assume it to be an entity
				--
        HookTable[ k ] = nil
			end

			--
			-- Hook returned a value - it overrides the gamemode function
			--
			--if ( a ~= nil ) then
			--	return a, b, c, d, e, f
			--end

				
		end
    
    --if table.getn(ret) > 0 then return unpack(ret) end
	end
	
end

PrintConsoleMessage("Loaded Hook Module");