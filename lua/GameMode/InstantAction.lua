local _G = _G;
local hook = hook;

_G.PrintConsoleMessage("Loading GameMode.InstantAction");
module( "InstantAction" )

_G.MissionData.Text = "STRING";

hook.Add( "Update", "Chilli_Custom_Update", function( GameTurn )
  if GameTurn % 10 == 0 then
    _G.PrintConsoleMessage("In Update Turn: " .. GameTurn);
  end
end )

_G.PrintConsoleMessage("Loadinged GameMode.InstantAction");