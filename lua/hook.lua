-- Hook module stolen from Garrysmod
local PrintConsoleMessage = PrintConsoleMessage;
PrintConsoleMessage("Loading Hook Module");

local pairs                        = pairs;
local table = table;
local isfunction        = isfunction;
local isstring                = isstring;

module( "hook" )

local Hooks = {}

--[[---------------------------------------------------------
    Name: GetTable
    Desc: Returns a table of all hooks.
-----------------------------------------------------------]]
function GetTable() return Hooks end


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
function Call( name, ... )

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
	
end

--[[---------------------------------------------------------
    Name: Run
    Args: string hookName, vararg args
    Desc: Calls hooks associated with the hook name.
-----------------------------------------------------------]]
function CallAll( name, ... )

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
	
end

PrintConsoleMessage("Loaded Hook Module");