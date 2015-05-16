PrintConsoleMessage("This line is in chilli.lua");

hook.Add( "Update", "Chilli_Custom_Update", function( )
	PrintConsoleMessage("In Update");
end )