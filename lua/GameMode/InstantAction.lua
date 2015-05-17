local _G = _G;
local hook = hook;

_G.PrintConsoleMessage("Loading GameMode.InstantAction");
module( "InstantAction" )

_G.MissionData.Text = "STRING";

hook.Add( "Update", "Chilli_Custom_Update", function( GameTurn )
  if GameTurn % 10 == 0 then
    --_G.PrintConsoleMessage("In Update Turn: " .. GameTurn);
    if _G.MissionData.TargetTestIsObjectified then
      _G.MissionData.TargetTest:SetObjectiveOff();
    else
      _G.MissionData.TargetTest:SetObjectiveOn();
    end
    _G.MissionData.TargetTestIsObjectified = not _G.MissionData.TargetTestIsObjectified;
  end
end )

hook.Add( "InitialSetup", "Chilli_Custom_InitialSetup", function()
  local player = _G.GetPlayerHandle(0);
  local vectorLoc = _G.GetPositionNear(player:GetPositionV(), 10, 10);
  _G.MissionData.TargetTest = _G.BuildObject("ibcrate", 0, vectorLoc);
  _G.MissionData.TargetTestIsObjectified = false;
end )

_G.PrintConsoleMessage("Loadinged GameMode.InstantAction");