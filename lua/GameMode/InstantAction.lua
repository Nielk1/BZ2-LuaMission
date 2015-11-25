local _G = _G;
local hook = hook;

_G.PrintConsoleMessage("Loading GameMode.InstantAction");
module( "InstantAction" )

_G.MissionData.Text = "STRING";

hook.Add( "Update", "Chilli_Custom_Update", function( GameTurn )
  if ( _G.MissionData.TargetTest == nil
     or not _G.MissionData.TargetTest:IsAround() ) then
    SpawnBox();
  end
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

function SpawnBox()
  local player = _G.GetPlayerHandle(1);
  local PlayerLocation = player:GetPositionV();
  --_G.PrintConsoleMessage(PlayerLocation.x .. ", " .. PlayerLocation.y .. ", " .. PlayerLocation.z);
  local vectorLoc = _G.SetCircularPos(PlayerLocation, _G.GetRandomFloat(10)+20, _G.GetRandomFloat(6.28318530718));
  --_G.PrintConsoleMessage(vectorLoc.x .. ", " .. vectorLoc.y .. ", " .. vectorLoc.z);
  _G.MissionData.TargetTest = _G.BuildObject("ibcrat00", 2, vectorLoc);
  _G.MissionData.TargetTestIsObjectified = false;
end

hook.Add( "InitialSetup", "Chilli_Custom_InitialSetup", function()
  SpawnBox();
end )

_G.PrintConsoleMessage("Loadinged GameMode.InstantAction");