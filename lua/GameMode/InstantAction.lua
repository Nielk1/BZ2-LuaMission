PrintConsoleMessage("Loading GameMode.InstantAction");
module( "InstantAction", package.seeall )

local MissionData = {
    GameTurn = 0;
};

function Execute()
  if ( MissionData.TargetTest == nil
     or not MissionData.TargetTest:IsAround() ) then
    SpawnBox();
  end
  if MissionData.GameTurn % 10 == 0 then
    if MissionData.TargetTestIsObjectified then
      MissionData.TargetTest:SetObjectiveOff();
    else
      MissionData.TargetTest:SetObjectiveOn();
    end
    MissionData.TargetTestIsObjectified = not MissionData.TargetTestIsObjectified;
  end
  
  MissionData.GameTurn = MissionData.GameTurn + 1;
end

function SpawnBox()
  local player = GetPlayerObject(1);
  local PlayerLocation = player:GetPositionV();
  --PrintConsoleMessage(PlayerLocation.x .. ", " .. PlayerLocation.y .. ", " .. PlayerLocation.z);
  local vectorLoc = GetCircularPos(PlayerLocation, GetRandomFloat(10)+20, GetRandomFloat(6.28318530718));
  --PrintConsoleMessage(vectorLoc.x .. ", " .. vectorLoc.y .. ", " .. vectorLoc.z);
  MissionData.TargetTest = BuildGameObject("ibcrat00", 2, vectorLoc);
  MissionData.TargetTest.TestData = "TEST STRING";
  MissionData.TargetTestIsObjectified = false;
end


function Save()
    return MissionData;
end

function Load(savedData)
    MissionData = savedData;
end

function PostLoad()
end

hook.AddSaveLoad("InstantAction", Save, Load, PostLoad);
hook.Add( "Execute", "InstantAction_Update", Execute);

_G.PrintConsoleMessage("Loadinged GameMode.InstantAction");