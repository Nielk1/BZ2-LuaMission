-- Battlezone Classic Training Mission 5, written by General BlackDragon.

-- Variables Not saved. Constants that never change.
local NumDrones = 21;
local NumTargets = 6;
local MaxSovShips = 10;
local NumBase = 16;
--local ForgivenessTime = 30;

-- Offensive ships, 1/4th chance Flanker, 1/5th chance Tank, 1/6th chance LTnk, Hraz or Rckt, 1/10th chance APC. No walkers, since this mission occurs before the NSDF campaign starts.
local OffensiveShips = {"vfigh","vfigh","vfigh","vfigh","vfigh","vtank","vtank","vtank","vtank","vltnk","vltnk","vltnk","vhraz","vhraz","vhraz","vrckt","vrckt","vrckt","vapc","vapc"}

local M = {
-- Bools
UpdateObjectives = false,
Warn1 = false,
Warn2 = false,
--Warn3 = false,
-- Floats
Warn1Time = 999999,
--Warn2Time = 0,
--Warn3Time = 0,
Nav3Time = 0,
-- Handles
Player = nil,
Recy = nil,
Turret1 = nil,
Turret2 = nil,
Turret3 = nil,
Nav1 = nil,
Nav2 = nil,
Nav3 = nil,
Nav4 = nil,
NavBase = nil,
Tank = { },
Drone = { },
Target = { },
TargetTimer = { },
SovShips = { },
Powerups = { },
Base = { },
-- Ints
ElapsedGameTime = 0,
Difficulty = 0,
StopScript = 0,
MissionState = 0,
Aud = { },
Nav1Hits = 0,
Nav2Hits = 0,
Nav3Hits = 0,
Nav4ShipsKilled = 0,
Nav4ShipsLost = 0,
SovietTime = 0,
SovietWave = 0,

endme = 0
}

--local ScoreData = {"Nav1Hits", "Nav2Hits", "Nav3Hits", "Nav4ShipsKilled", "Nav4ShipsLost"}




--function InitialSetup()
--	M.Difficulty = IFace_GetInteger("options.play.difficulty");
--end

function Save()
    return 
		M
end

function Load(...)	
    if select('#', ...) > 0 then
		M
		= ...
    end
end

function Start()

    PrintConsoleMessage("Training Mission 5 Lua created by General BlackDragon");
	
	M.Difficulty = IFace_GetInteger("options.play.difficulty");
	M.StopScript = GetVarItemInt("network.session.ivar119");
	
	AllyTeams(1, 3);
	AllyTeams(4, 3);

	M.Tank[1] = BuildAngleObject("avtank", 1, "spawn_tank1", "tank1", 0, -1, true, false, 0, false);
	M.Tank[2] = BuildAngleObject("avtank", 1, "spawn_tank2", "tank2", 0, -1, true, false, 0, false);
	M.Tank[3] = BuildAngleObject("avtank", 1, "spawn_tank3", "tank3", 0, -1, true, false, 0, false);
	
    M.Base[1] = BuildAngleObject("abhang", 3, "spawn_abhang1", "hang1");
    M.Base[2] = BuildAngleObject("abhang", 3, "spawn_abhang2", "hang2");
    M.Base[3] = BuildAngleObject("abhang", 3, "spawn_abhang3", "hang3");
    M.Base[4] = BuildAngleObject("abhqcp", 3, "spawn_abhqcp", "hqcp");
    M.Base[5] = BuildAngleObject("absupp", 3, "spawn_absupp1", "supp1", 90);
    M.Base[6] = BuildAngleObject("absupp", 3, "spawn_absupp2", "supp2", 90);
    M.Base[7] = BuildAngleObject("abcafe", 3, "spawn_abcafe", "cafe", 270);
    M.Base[8] = BuildAngleObject("abspow", 3, "spawn_abspow1", "spow1");
    M.Base[9] = BuildAngleObject("abspow", 3, "spawn_abspow2", "spow2");
    M.Base[10] = BuildAngleObject("abtowe", 3, "spawn_abtowe1", "towe1");
    M.Base[11] = BuildAngleObject("abtowe", 3, "spawn_abtowe2", "towe2", 270);
    M.Base[12] = BuildAngleObject("abtowe", 3, "spawn_abtowe3", "towe3");
    M.Base[13] = BuildAngleObject("abtowe", 3, "spawn_abtowe4", "towe4", 90);
    M.Base[14] = BuildAngleObject("abcomm", 3, "spawn_abcomm", "comm");
	M.Base[15] = BuildAngleObject("abbarr", 3, "spawn_abbarr1", "barr1");
    M.Base[16] = BuildAngleObject("abbarr", 3, "spawn_abbarr2", "barr2");
    
    M.Recy = BuildAngleObject("avrect5", 3, "spawn_recy", "montana", 90); -- Special one capable of building Test and Drone, just for fun.
    Goto(M.Recy, GetHandle("recy_geyser"));
    
    M.Turret1 = BuildAngleObject("avturr", 3, "spawn_avturr1", "turr1", 90);
	Defend(M.Turret1, 1);
    M.Turret2 = BuildAngleObject("avturr", 3, "spawn_avturr2", "turr2");
	Defend(M.Turret2, 1);
    M.Turret3 = BuildAngleObject("avturr", 3, "spawn_avturr3", "turr3");
	Defend(M.Turret3, 1);
	
	M.NavBase = BuildAngleObject("apcamr", 1, "nav_base", "nav_base");
	M.Nav1 = BuildAngleObject("apcamr", 1, "nav1", "nav1");
	M.Nav2 = BuildAngleObject("apcamr", 1, "nav2", "nav2");
	M.Nav3 = BuildAngleObject("apcamr", 1, "nav3", "nav3");
	M.Nav4 = BuildAngleObject("apcamr", 1, "nav4", "nav4");
	SetObjectiveName(M.NavBase, "Petavius Base");
	SetObjectiveName(M.Nav1, "Nav Beacon 1");
	SetObjectiveName(M.Nav2, "Nav Beacon 2");
	SetObjectiveName(M.Nav3, "Nav Beacon 3");
	SetObjectiveName(M.Nav4, "Nav Beacon 4");
	SetMaxHealth(M.Nav1, 0);
	SetMaxHealth(M.Nav2, 0);
	SetMaxHealth(M.Nav3, 0);
	SetMaxHealth(M.Nav4, 0);	
	
	for i = 1, NumDrones do
		local PathName = "test_drone" .. i;
		if i == 3 then
			M.Drone[i] = BuildAngleObject("avtest", 3, PathName, PathName);
			SetMaxHealth(M.Drone[i], 0);
		else
			M.Drone[i] = BuildAngleObject("avdron", 3, PathName, PathName);
		end
		SetIndependence(M.Drone[i], 0);
		GiveWeapon(M.Drone[i], "gBZCatstab");
		GiveWeapon(M.Drone[i], "gBZCatstab");
		SetMaxAmmo(M.Drone[i], 960);
		SetCurAmmo(M.Drone[i], 960);
		if M.Difficulty < 2 then
			SetWeaponMask(M.Drone[i], 1);
		end
	end
	
	for i = 1, NumTargets do
		local PathName = "target" .. i;
		M.Target[i] = BuildAngleObject("nparr2", 0, PathName, PathName);
		SetObjectiveName(M.Target[i], "Target");
		M.TargetTimer[i] = 99999;
	end
	
	if M.StopScript == 0 then
	
		M.UpdateObjectives = true;
		MissionAudioMessage("tran0501.wav"); -- Welcome to the base Commander.
		MissionAudioMessage("tran0502.wav"); -- Get in a tank.
		
		Patrol(M.Drone[1], "patrol1", 1);
		Patrol(M.Drone[2], "patrol2", 1);
		
		M.MissionState = 0; -- Start the mission.
		
		M.SovietTime = 20 + GetRandomInt(20);
		print("Magic Spawner is: ", M.SovietTime);
		
	end

end


function AddObject(h)

	local Team = GetTeamNum(h);

	if M.StopScript == 0 then
	
		if Team ~= 1 then
			SetSkill(h, M.Difficulty+1);
		else
			SetSkill(h, 3);
		end

	    if IsRecycler(h) then 
			SetObjectiveName(h, "Montana");
		end
	
	end

end

function DeleteObject(h)

	if M.MissionState == 5 then -- Nav 1 test.
	
		for i = 1, NumTargets do 
			if h == M.Target[i] then
				M.TargetTimer[i] = ((3 + GetRandomInt(4)) * GetTPS());
				M.Nav1Hits = M.Nav1Hits + 1;
			end
		end
	
	elseif M.MissionState > 10 and M.Nav1Hits ~= M.SovietTime then -- Nav 4 test has begun.
	
		if (h == M.Tank[1] or h == M.Tank[2] or h == M.Tank[3]) then -- Player died. -- and h == M.Player then 
			M.Nav4ShipsLost = M.Nav4ShipsLost + 1;
			
			local TanksLeft = 0;
			for i = 1, 3 do
				if IsValid(M.Tank[i]) then 
					TanksLeft = TanksLeft + 1;
				end
			end	
			
			if TanksLeft == 2 then
				MissionAudioMessage("tran0535.wav"); -- Good you, if you'd like to keep trying, get in another tank.
			elseif TanksLeft == 1 then
				MissionAudioMessage("tran0536.wav"); -- You've still got 1 tank left, sir.
			end
		end
	
		for i = 1, NumDrones do 
			
			if h == M.Drone[i] then
				M.Nav4ShipsKilled = M.Nav4ShipsKilled + 1;
			end		
		end
	
	end

end

function Update()

	M.ElapsedGameTime = M.ElapsedGameTime + 1;
	
	if M.StopScript == 0 then
	
		M.Player = GetPlayerHandle();
		
	-- Nope, Dues Ex Ceteri doesn't like. :(
		-- Try to keep powerups from sliding down a hill.
	--	for i = 1, 7 do
	--		if IsValid(M.Powerups[i]) then 
	--			local Velocity = GetVelocity(M.Powerups[i]);
	--			Velocity.x = 0;
	--			Velocity.z = 0;
	--			SetVelocity(M.Powerups[i], Velocity);
	--		end
	--	end
		-- Every 30 seconds, try to respawn powerups.
		if GetTime() > 0 and (GetTime() % 30 == 0) then
			for i = 1, 7 do
				if (M.Powerups[i] == nil) or (not IsValid(M.Powerups[i])) then
					local PathName = "spawn_apammo" .. i;
					
					if i < 5 then 
						M.Powerups[i] = BuildAngleObject("apBZCammo", 0, PathName, PathName);
					elseif i == 5 then
						PathName = "spawn_aprepa" .. i-4;
						M.Powerups[i] = BuildAngleObject("apBZCrepa", 0, PathName, PathName);
					elseif i > 5 then
						PathName = "spawn_apsstb" .. i-5;
						M.Powerups[i] = BuildAngleObject("apBZCsstb", 0, PathName, PathName);
					end
				end
			end
		end
		
		-- Objective Display. Does Objectivey stuff.
		if M.UpdateObjectives then
			ClearObjectives();
			
			if M.MissionState > 13 then 
			
				if M.Nav1Hits == M.SovietTime then --M.Difficulty == 2 then 
				
					if M.MissionState > 14 then 
						AddObjective("tran0510.otf", "GREEN");
					else
						AddObjective("tran0510.otf", "WHITE");
					end
					
				else
					if M.MissionState > 20 then 
						AddObjective("tran0509.otf", "GREEN");
					else
						AddObjective("tran0509.otf", "WHITE");
						AddObjective("tran0511.otf", "YELLOW");
					end
				end
				
			elseif M.MissionState > 10 then 
				
				if M.MissionState > 12 then 
					AddObjective("tran0508.otf", "GREEN");
				else
					AddObjective("tran0508.otf", "WHITE");
					
					local String = "You dodged " .. M.Nav3Hits .. " out of 20 Shots!";
					AddObjective(String, "GREEN");
				end
			
			elseif M.MissionState > 7 then 
			
				if M.MissionState > 9 then 
					AddObjective("tran0507.otf", "WHITE");
				elseif M.MissionState > 8 then 
					AddObjective("tran0506.otf", "GREEN");
				else
					AddObjective("tran0506.otf", "WHITE");
				end
			
			elseif M.MissionState > 6 then 
			
				if M.MissionState > 7 then 
					AddObjective("tran0505.otf", "GREEN");
				else
					AddObjective("tran0505.otf", "WHITE");
				end
			
			elseif M.MissionState > 5 then 
			
				if M.MissionState > 5 then 
					AddObjective("tran0504.otf", "GREEN");
				else
					AddObjective("tran0504.otf", "WHITE");
				end
				
			elseif M.MissionState > 4 then 
			
				if M.MissionState > 5 then 
					AddObjective("tran0503.otf", "GREEN");
					local String = "You hit " .. M.Nav1Hits .. " Targets!";
					AddObjective(String, "GREEN");
				else
					AddObjective("tran0503.otf", "WHITE");
				end
			
			elseif M.MissionState > 2 then 
			
				if M.MissionState > 3 then 
					AddObjective("tran0502.otf", "GREEN");
				else
					AddObjective("tran0502.otf", "WHITE");
				end
			
			
			else
				
				if M.MissionState > 1 then
					AddObjective("tran0501.otf", "GREEN");
				else
					AddObjective("tran0501.otf", "WHITE");	
				end
				
			end
			
			M.UpdateObjectives = false;
		end
		
		-- Main mission code, State machine.
		if M.MissionState == 0 then 
		
			-- Player is in a ship now, let's brief him on Thanks.
			--if not IsPerson(M.Player) then
			if M.Player == M.Tank[1] or M.Player == M.Tank[2] or M.Player == M.Tank[3] then
				M.UpdateObjectives = true;
				M.MissionState = 1;
				M.Warn1Time = GetTime() + 45; -- include VO length in timer.
				M.Aud[1] = AudioMessage("tran0546.wav"); -- Press the 'i' key on the Base Nav for a briefing on weapon linking, otherwise head to Nav Beacon 1.
				
				-- Test me.
				--M.MissionState = 100; -- Win me!
				
			end
			
		elseif M.MissionState == 1 then 
		
			-- Long winded boring and nonsensical explanation done.
			if IsAudioMessageDone(M.Aud[1]) then
				-- If you blew up the base nav, or go away from base, or timer is up, you opted out of the briefing. 
				if not IsValid(M.NavBase) or (GetDistance(M.Player, M.NavBase) > 200) or (GetTime() > M.Warn1Time) then 
					M.UpdateObjectives = true;
					M.Warn1Time = GetTime() + 45;
					SetObjectiveOn(M.Nav1);
					M.MissionState = 3;
				end
			
				if IsInfo("apcamr") then 
					M.Aud[1] = MissionAudioMessage("tran0503.wav"); -- Long winded explanation of Tanks weapons and Weapon Linking.
					M.Aud[2] = MissionAudioMessage("tran0504.wav");
					M.Aud[3] = MissionAudioMessage("tran0505.wav");
					M.Aud[4] = MissionAudioMessage("tran0506.wav");
					M.Aud[5] = MissionAudioMessage("tran0507.wav");
					M.Aud[6] = MissionAudioMessage("tran0508.wav");
					M.Aud[7] = MissionAudioMessage("tran0509.wav"); -- This wav signals that we're finished.
					M.MissionState = 2;
				end	
			end
			
-- Player is ordered to go to Nav Beacon 1.
		elseif M.MissionState == 2 then 
		
			-- If you blew up the base nav, or go away from base, or timer is up, you opted out of the briefing. 
			if not IsValid(M.NavBase) or (GetDistance(M.Player, M.NavBase) > 200) or (GetTime() > M.Warn1Time) then 
				M.UpdateObjectives = true;
				M.Warn1Time = GetTime() + 45;
				SetObjectiveOn(M.Nav1);
				M.MissionState = 3;
				
				-- Stop current Briefing if it's in-progress.
				for i = 1, 7 do
					if IsAudioMessagePlaying(M.Aud[i]) then
						StopAudioMessage(M.Aud[i]);
					end
				end
			end
		
			-- Long winded boring and nonsensical explanation done.
			if IsAudioMessageDone(M.Aud[7]) then
				M.UpdateObjectives = true;
				M.Warn1Time = GetTime() + 45;
				MissionAudioMessage("tran0510.wav"); -- Go to the Target Range at Nav Beacon 1.
				SetObjectiveOn(M.Nav1);
				M.MissionState = 3;
			end
			
		elseif M.MissionState == 3 then 
		
			-- The player is at Nav Beacon 1.
			if GetDistance(M.Player, "nav1") < 50 then
				M.Aud[1] = MissionAudioMessage("tran0514.wav"); -- Explain to sit at nav and shoot the targets.
				--M.Warn1Time = 0; -- turn off warning.
				M.MissionState = 4;
				M.Warn1 = false;
				M.Warn2 = false;
				M.UpdateObjectives = true;
			end
			
			-- End Mission conditions.
		
			-- If the player doesn't go to Nav 1 in time.
			if (GetTime() > M.Warn1Time) and not M.Warn1 then
				M.Warn1 = true;
				M.Warn1Time = GetTime() + 45;
				MissionAudioMessage("tran0511.wav"); -- Go to the nav, commander!
			end
			
			if M.Warn1 and (GetTime() > M.Warn1Time) and not M.Warn2 then
				M.Warn2 = true;
				M.Warn1Time = GetTime() + 45;
				MissionAudioMessage("tran0512.wav"); -- We don't have time for this!
			end
			
			if M.Warn2 and (GetTime() > M.Warn1Time) and not M.MissionOver then
				FailMission(GetTime() + 10, "tran05l1.des");
				MissionAudioMessage("tran0513.wav"); -- Go waste someone else's time, sir!
				M.MissionState = 99;
			end
-- Player is At Nav Beacon 1. 
		elseif M.MissionState == 4 then 
		
			if IsAudioMessageDone(M.Aud[1]) then 
				M.UpdateObjectives = true;
				StartCockpitTimer(60);
				M.MissionState = 5;
				M.Nav1Hits = 0;
				
				for i = 1, NumTargets do
					if not IsAround(M.Target[i]) then 
						local PathName = "target" .. i;
						M.Target[i] = BuildAngleObject("nparr2", 4, PathName, PathName);
					end
					
					SetTeamNum(M.Target[i], 4);
					SetObjectiveOn(M.Target[i]);
					SetMaxHealth(M.Target[i], 70);
					SetCurHealth(M.Target[i], 70);
					SetObjectiveName(M.Target[i], "Target");
					M.TargetTimer[i] = GetTime() + 60;
				end
				
			end
-- Training Exercise 1, target and shoot the targets.		
		elseif M.MissionState == 5 then 
		
			-- Respawn the targets as needed.
			for i = 1, NumTargets do
				if not IsAround(M.Target[i]) then 
					M.TargetTimer[i] = M.TargetTimer[i] - 1;
					
					if M.TargetTimer[i] <= 0 then
						local PathName = "target" .. i;
						M.Target[i] = BuildAngleObject("nparr2", 4, PathName, PathName);
						SetObjectiveName(M.Target[i], "Target");
						SetMaxHealth(M.Target[i], 70);
						SetCurHealth(M.Target[i], 70);
						SetObjectiveOn(M.Target[i]);
						M.TargetTimer[i] = GetTime() + 60;
					end
				end
			end
			
			-- If time is up, let's see how you scored.
			if GetCockpitTimer() == 0 then 
				if M.Nav1Hits == 0 then 
					MissionAudioMessage("tran0517.wav"); -- Are you serious?
					M.Aud[1] = AudioMessage("tran0516.wav"); -- You'll have to do better then that, Commander. Try again.
					M.MissionState = 4; -- Try again.
				elseif M.Nav1Hits < 20 then
					M.Aud[1] = MissionAudioMessage("tran0516.wav"); -- You'll have to do better then that, Commander. Try again.
					M.MissionState = 4; -- Try again.
				else
					if M.Nav1Hits >= 40 then
						M.Aud[1] = MissionAudioMessage("tran0545.wav"); -- Great shooting sir! Now we'll try moving targets, Go to Nav 2.
					else
						M.Aud[1] = MissionAudioMessage("tran0515.wav"); -- Now we'll try moving targets, Go to Nav 2.
					end
					M.MissionState = 6; -- You pass.
					M.UpdateObjectives = true;
					HideCockpitTimer();
					SetObjectiveOff(M.Nav1);
					SetObjectiveOn(M.Nav2);
					M.Warn1Time = GetTime() + 45;
					M.Warn1 = false;
					M.Warn2 = false;
					
					for i = 1, NumTargets do
						SetTeamNum(M.Target[i], 0);
						SetObjectiveOff(M.Target[i]);
						SetMaxHealth(M.Target[i], 0);
					end
					
				end
			end
			
			-- If you get far away from the nav, get yelled at.
			if GetDistance(M.Player, "nav1") > 50 then
				if not M.Warn1 then 
					M.Warn1 = true;
					M.Warn1Time = GetTime() + 15;
					MissionAudioMessage("tran0518.wav"); -- Stay with the nav beacon!
				else
					if not M.Warn2 then 
						if (GetTime() > M.Warn1Time) then
							M.Warn2 = true;
							M.Warn1Time = GetTime() + 15;
							MissionAudioMessage("tran0518.wav"); -- Stay with the nav beacon!
						end
					else
						if (GetTime() > M.Warn1Time) then
							FailMission(GetTime() + 5, "tran05l2.des");
							M.MissionState = 99;
						end
					end
				end
			end
-- Player is ordered to go to Nav Beacon 2.			
		elseif M.MissionState == 6 then 
		
			-- Your at nav 2, congrats...
			if GetDistance(M.Player, "nav2") < 50 then
				if not IsValid(M.Drone[1]) and not IsValid(M.Drone[2]) then
					M.Aud[1] = MissionAudioMessage("tran0542.wav"); -- Hmm, We seem to be missing some target drones. Oh well... Head to Nav Beacon 3.
					M.Warn1Time = GetTime() + 45;
					SetObjectiveOff(M.Nav2);
					SetObjectiveOn(M.Nav3);
					M.MissionState = 8;
					M.Warn1 = false;
					M.Warn2 = false;
				else
					M.Aud[1] = MissionAudioMessage("tran0519.wav"); -- Use the T key to Target the drones
					SetTeamNum(M.Drone[1], 5);
					SetTeamNum(M.Drone[2], 5);
					Ally(1, 5);
					M.MissionState = 7;
				end
				M.UpdateObjectives = true;
			end
			
			-- If the player doesn't go to Nav 2 in time.
			if (GetTime() > M.Warn1Time) and not M.Warn1 then
				M.Warn1 = true;
				M.Warn1Time = GetTime() + 45;
				MissionAudioMessage("tran0511.wav"); -- Go to the Nav Beacon!
			end
			
			if M.Warn1 and (GetTime() > M.Warn1Time) and not M.Warn2 then
				M.Warn2 = true;
				M.Warn1Time = GetTime() + 45;
				MissionAudioMessage("tran0522.wav"); -- We dont have all day.
			end
			
			if M.Warn2 and (GetTime() > M.Warn1Time) and not M.MissionOver then
				FailMission(GetTime() + 10, "tran05l1.des");
				MissionAudioMessage("tran0523.wav"); -- Come back when you're ready!
				M.MissionState = 99;
			end
			
-- Player is at Nav Beacon 2. 		
		elseif M.MissionState == 7 then 
		
			-- If both drones are dead...
			if not IsAround(M.Drone[1]) and not IsAround(M.Drone[2]) and IsAudioMessageDone(M.Aud[1]) then
				M.Aud[1] = MissionAudioMessage("tran0520.wav"); -- Good job, now we'll preactice Evasive menuvers. Head to Nav 3.
				M.Warn1Time = GetTime() + 45;
				M.MissionState = 8;
				SetObjectiveOff(M.Nav2);
				SetObjectiveOn(M.Nav3);
				SetPerceivedTeam(M.Player, 1);
				M.Warn1 = false;
				M.Warn2 = false;
				M.UpdateObjectives = true;
			end
			
-- Player is ordered to go to Nav Beacon 3.		
		elseif M.MissionState == 8 then 
		
			if GetDistance(M.Player, "nav3") < 50 then
				M.Aud[1] = MissionAudioMessage("tran0524.wav"); -- Okay, in this exercise you'll practice evading enemy fire. Try not to get hit.
				M.MissionState = 9;
				M.UpdateObjectives = true;
			end
		
			-- If the player doesn't go to Nav 3 in time.
			if (GetTime() > M.Warn1Time) and not M.Warn1 then
				M.Warn1 = true;
				M.Warn1Time = GetTime() + 45;
				MissionAudioMessage("tran0511.wav"); -- Go to the target range!
			end
			
			if M.Warn1 and (GetTime() > M.Warn1Time) and not M.Warn2 then
				M.Warn2 = true;
				M.Warn1Time = GetTime() + 45;
				MissionAudioMessage("tran0522.wav"); -- We dont have all day.
			end
			
			if M.Warn2 and (GetTime() > M.Warn1Time) and not M.MissionOver then
				FailMission(GetTime() + 10, "tran05l1.des");
				MissionAudioMessage("tran0523.wav"); -- Come back when you're ready!
				M.MissionState = 99;
			end
-- Player is at Nav Beacon 3.		
		elseif M.MissionState == 9 then 
		
			if IsAudioMessageDone(M.Aud[1]) then 
				SetTeamNum(M.Drone[3], 4);
				SetSkill(M.Drone[3], 3); -- Maximum Skill.
				GiveWeapon(M.Drone[3], "gBZCdatstab");
				GiveWeapon(M.Drone[3], "gBZCdatstab");
				SetWeaponMask(M.Drone[3], 1); -- Only shoot 1 cannon.
				SetObjectiveOn(M.Drone[3]);
				SetObjectiveOff(M.Nav3);
			--	SetMaxHealth(M.Nav3, 1);
			--	Damage(M.Nav3, GetMaxHealth(M.Nav3) + 9000); -- It's over 9000!
				--Attack(M.Drone[3], M.Player);
				Defend(M.Drone[3], 1);
				SetMaxAmmo(M.Drone[3], 240); -- 20 single shots.
				SetCurAmmo(M.Drone[3], 240);
				M.MissionState = 10;
				M.Nav3Hits = 0;
				M.Nav3Time = 0;
				M.UpdateObjectives = true;
			end
-- Begin the exercise 3, dodge enemy fire.

		elseif M.MissionState == 10 then 
		
			-- It'll come down eventually....
			if GetPosition(M.Nav3).y < 350 then
				local Velocity = GetVelocity(M.Nav3);
				Velocity.y = 25;
				SetVelocity(M.Nav3, Velocity);
			end
			
			-- If the drone is out of ammo, or your tank's health is at 25%, stop test.
			if GetCurAmmo(M.Drone[3]) < 12 then

				if M.Nav3Time == 0 then 
					M.Nav3Time = GetTime() + 2; -- Give time for the inbound ordnance to finish.
				end
				
				if GetTime() > M.Nav3Time then 
				
					M.Nav3Hits = 20 - M.Nav3Hits; -- Re-calculate how many you dodged vs how many hit you.

					if M.Nav3Hits > 10 then -- You dodged half the shots.
						if M.Nav3Hits == 20 then
							MissionAudioMessage("tran0526.wav"); -- Very good, dodged all shots.
						else
							MissionAudioMessage("tran0525.wav"); -- Good Work.
						end
						Stop(M.Drone[3], 1); -- Order drone to stop shooting.
						SetTeamNum(M.Drone[3], 3);
						SetObjectiveOff(M.Drone[3]);
						M.MissionState = 11;
						M.Warn1Time = GetTime() + 45;
						MissionAudioMessage("tran0528.wav"); -- Now we'll test you on what you've learned, Head to Nav Beacon 4.
					--	M.Nav3 = BuildAngleObject("apcamr", 1, "nav3", "nav3", 0, -1, false, false, 300);
					--	SetMaxHealth(M.Nav3, 0);
					--	SetObjectiveOff(M.Nav3);
						SetObjectiveOn(M.Nav4);
						M.UpdateObjectives = true;
					else
						M.Aud[1] = MissionAudioMessage("tran0527.wav"); -- If this was real, you'd be dead!
						M.MissionState = 9; -- Try again!
					end
				end
			end
		
			-- If you get far away from the Drone, get yelled at.
			if GetDistance(M.Player, M.Drone[3]) > 200 then -- was "nav3", 50
				if not M.Warn1 then 
					M.Warn1 = true;
					M.Warn1Time = GetTime() + 15;
					MissionAudioMessage("tran0544.wav"); -- Stay with the Target Drone!
				else
					if not M.Warn2 then 
						if (GetTime() > M.Warn1Time) then
							M.Warn2 = true;
							M.Warn1Time = GetTime() + 15;
							MissionAudioMessage("tran0544.wav"); -- Stay with the Target Drone!
						end
					else
						if (GetTime() > M.Warn1Time) then
							FailMission(GetTime() + 5, "tran05l2.des");
							M.MissionState = 99;
						end
					end
				end
			end
		end
		
-- Non Hard Mode, Continue training as normal.
		if M.Nav1Hits ~= M.SovietTime then --if M.Difficulty < 2 then 
	-- Player is ordered to go to Nav Beacon 4.
			if M.MissionState == 11 then 
			
				M.MissionState = 12; -- Added to bump mission objectives.
				
			elseif M.MissionState == 12 then 
			
				M.MissionState = 13; -- Added to bump mission objectives.
				
			elseif M.MissionState == 13 then 
			
				if GetDistance(M.Player, "nav4") < 50 then
					M.Aud[1] = MissionAudioMessage("tran0529.wav"); -- Okay, first 1 drone Skill level 1.
					SetObjectiveOff(M.Nav4);
					M.UpdateObjectives = true;
					SetTeamNum(M.Drone[4], 4);
					SetSkill(M.Drone[4], 1);
					--Attack(M.Drone[4], M.Player);
					SetObjectiveOn(M.Drone[4]);
					M.MissionState = 14;
				end
				
				-- If the player doesn't go to Nav 1 in time.
				if (GetTime() > M.Warn1Time) and not M.Warn1 then
					M.Warn1 = true;
					M.Warn1Time = GetTime() + 45;
					MissionAudioMessage("tran0511.wav"); -- Go to the nav, commander!
				end
				
				if M.Warn1 and (GetTime() > M.Warn1Time) and not M.Warn2 then
					M.Warn2 = true;
					M.Warn1Time = GetTime() + 45;
					MissionAudioMessage("tran0512.wav"); -- We don't have time for this!
				end
				
				if M.Warn2 and (GetTime() > M.Warn1Time) and not M.MissionOver then
					FailMission(GetTime() + 10, "tran05l1.des");
					MissionAudioMessage("tran0513.wav"); -- Go waste someone else's time, sir!
					M.MissionState = 99;
				end

	-- 1 Drone Skill 1 Attack		
			elseif M.MissionState == 14 then 
			
				if not IsAround(M.Drone[4]) then
					M.Aud[1] = MissionAudioMessage("tran0530.wav"); -- Okay, first 1 drone Skill level 2.
					SetTeamNum(M.Drone[5], 4);
					SetSkill(M.Drone[5], 2);
					--Attack(M.Drone[5], M.Player);
					SetObjectiveOn(M.Drone[5]);
					M.MissionState = 15;
				end
	-- 1 Drone Skill 2 Attack		
			elseif M.MissionState == 15 then 
			
				if not IsAround(M.Drone[5]) then
					M.Aud[1] = MissionAudioMessage("tran0531.wav"); -- Okay, first 1 drone Skill level 3.
					SetTeamNum(M.Drone[6], 4);
					SetSkill(M.Drone[6], 3);
					--Attack(M.Drone[6], M.Player);
					SetObjectiveOn(M.Drone[6]);
					M.MissionState = 16;
				end
	-- 1 Drone Skill 3 Attack		
			elseif M.MissionState == 16 then 
			
				if not IsAround(M.Drone[6]) then
					M.Aud[1] = MissionAudioMessage("tran0532.wav"); -- Okay, Now 2 on 1, same settings as before, first Low, then Medium, then High. Good luck sir.
					for i = 7, 8 do
						SetTeamNum(M.Drone[i], 4);
						SetSkill(M.Drone[i], 1);
						--Attack(M.Drone[i], M.Player);
						SetObjectiveOn(M.Drone[i]);
					end
					M.MissionState = 17;
				end
	-- 2 Drones	Skill 1 Attack
			elseif M.MissionState == 17 then 
			
				if not IsAround(M.Drone[7]) and not IsAround(M.Drone[8]) then
					for i = 9, 10 do
						SetTeamNum(M.Drone[i], 4);
						SetSkill(M.Drone[i], 2);
						--Attack(M.Drone[i], M.Player);
						SetObjectiveOn(M.Drone[i]);
					end
					M.MissionState = 18;
				end
	-- 2 Drones Skill 2 Attack		
			elseif M.MissionState == 18 then 
			
				if not IsAround(M.Drone[9]) and not IsAround(M.Drone[10]) then
					for i = 11, 12 do
						SetTeamNum(M.Drone[i], 4);
						SetSkill(M.Drone[i], 3);
						--Attack(M.Drone[i], M.Player);
						SetObjectiveOn(M.Drone[i]);
					end
					M.MissionState = 19;
				end
	-- 2 Drones Skill 3 Attack.
			elseif M.MissionState == 19 then 
			
				if not IsAround(M.Drone[11]) and not IsAround(M.Drone[12]) then
					M.Aud[1] = MissionAudioMessage("tran0533.wav"); -- Okay, Now 2 on 1, same settings as before, first Low, then Medium, then High. Good luck sir.
					for i = 13, 15 do
						SetTeamNum(M.Drone[i], 4);
						SetSkill(M.Drone[i], 1);
						--Attack(M.Drone[i], M.Player);
						SetObjectiveOn(M.Drone[i]);
					end
					M.MissionState = 20;
				end
	-- 3 Drones Skill 1 Attack		
			elseif M.MissionState == 20 then 
			
				if not IsAround(M.Drone[13]) and not IsAround(M.Drone[14]) and not IsAround(M.Drone[15]) then
					for i = 16, 18 do
						SetTeamNum(M.Drone[i], 4);
						SetSkill(M.Drone[i], 2);
						--Attack(M.Drone[i], M.Player);
						SetObjectiveOn(M.Drone[i]);
					end
					M.MissionState = 21;
				end
	-- 3 Drones Skill 2 Attack		
			elseif M.MissionState == 21 then 
			
				if not IsAround(M.Drone[16]) and not IsAround(M.Drone[17]) and not IsAround(M.Drone[18]) then
					for i = 19, 21 do
						SetTeamNum(M.Drone[i], 4);
						SetSkill(M.Drone[i], 3);
						--Attack(M.Drone[i], M.Player);
						SetObjectiveOn(M.Drone[i]);
					end
					M.MissionState = 22;
				end
	-- 3 Drones Skill 3 Attack		
			elseif M.MissionState == 22 then 
			
				if not IsAround(M.Drone[19]) and not IsAround(M.Drone[20]) and not IsAround(M.Drone[21]) then 
					MissionAudioMessage("tran0534.wav"); -- Very good sir, excellent piloting skills.
					M.MissionState = 100; -- You win.
					M.UpdateObjectives = true;
				end
			
			end
			
			
	-- 	Watches for End Game stuff.
			if M.MissionState > 13 and M.MissionState < 23 then
			
				-- If the player left to get a new tank, order current drones to attack him.
				if GetDistance(M.Player, "nav4") < 300 then
					if not IsPerson(M.Player) then
						for i = 1, NumDrones do
							if GetTeamNum(M.Drone[i]) == 4 and GetCurrentCommand(M.Drone[i]) == 0 then
								Attack(M.Drone[i], M.Player, 1);
							end
						end
					end
				end
			
				if not IsValid(M.Tank[1]) and not IsValid(M.Tank[2]) and not IsValid(M.Tank[3]) then -- Out of tanks, training is over.
					MissionAudioMessage("tran0537.wav"); -- You pass this training exercise.
					M.MissionState = 100; -- You win.			
				end
				
				-- Require the player to kill XX tanks? Naw, just let them go until they're out of tanks.
				--if M.Nav4ShipsKilled > 10 then 
				
				--end
				
				-- The CCA didn't destroy it, who did?
				if not IsAround(M.Recy) then 
					MissionAudioMessage("tran0543.wav");
					FailMission(GetTime() + 10, "tran05l4.des");
					M.MissionState = 99;
				end
				
			end
			
			if M.MissionState == 100 then -- You won!
				-- Make fancy output to save stats to a file. -GBD
				local Debrief = LoadFile("tran05w1.des");
				
				if Debrief ~= nil then 
					local VarCount = 1;
					local start = 0;
					local last = 0;
					local SavedScore = { };
					local PlayerName = GetPlayerName(GetPlayerHandle());
					SavedScore[1], found = GetODFInt("tran05saved.odf", PlayerName, "Test1", 0);
					SavedScore[2], found = GetODFInt("tran05saved.odf", PlayerName, "Test2", 0);
					SavedScore[3], found = GetODFInt("tran05saved.odf", PlayerName, "Test3", 0);
					SavedScore[4], found = GetODFInt("tran05saved.odf", PlayerName, "Test4", 0);
					SavedScore[5], found = GetODFInt("tran05saved.odf", PlayerName, "Test5", 0);

					Debrief = Debrief:format(M.Nav1Hits, M.Nav2Hits, M.Nav3Hits, M.Nav4ShipsKilled, M.Nav4ShipsLost, SavedScore[1], SavedScore[2], SavedScore[3], SavedScore[4], SavedScore[5]);
					
					-- If Scores are better then Saved Scores, write out SavedScores to tran05saved.odf. Keep the best of each test saved.						
					if (M.Nav1Hits > SavedScore[1]) then
						SavedScore[1] = M.Nav1Hits;
					end
					if(M.Nav2Hits > SavedScore[2]) then
						SavedScore[2] = M.Nav2Hits;
					end
					if (M.Nav3Hits > SavedScore[3]) then
						SavedScore[3] = M.Nav3Hits;
					end
					if (M.Nav4ShipsKilled > SavedScore[4]) then 
						SavedScore[4] = M.Nav4ShipsKilled;
						SavedScore[5] = M.Nav4ShipsLost;
					end
					
					local NewString = "// These are your Saved Scores for the Training 5 mission. \n[" .. PlayerName .. "] \nTest1 = " .. SavedScore[1] .. " // Targets Hit\nTest2 = " .. SavedScore[2] .. " // Shots hit Drones\nTest3 = " .. SavedScore[3] .. " // Shots Dodged\nTest4 = " .. SavedScore[4] .." // Drones Destroyed\nTest5 = " .. SavedScore[5] .. " // Tanks Lost";
					
					-- Write the new Scores.
					WriteToFile("logs/tran05saved.odf", NewString, false);
					-- Write the new Debrief.
					WriteToFile("logs/lasttran5.des", Debrief, false);
				end
				
				SucceedMission(GetTime() + 10, "lasttran5.des");
				M.MissionState = 99;
			end
			
-- HARD MODE, skip Nav 4 training and protect the Recycler!
		else
		
			if M.MissionState == 11 then 
			
				MissionAudioMessage("tran0538.wav");
				M.Aud[1] = MissionAudioMessage("tran0539.wav");
				M.MissionState = 12;
			
			elseif M.MissionState == 12 then 
				
				if IsAudioMessageDone(M.Aud[1]) then 
					SetObjectiveOff(M.Nav4);
					SetTeamNum(M.Recy, 1);
					Defend(M.Turret1, 0);
					Defend(M.Turret2, 0);
					Defend(M.Turret3, 0);
					SetTeamNum(M.Turret1, 1);
					SetTeamNum(M.Turret2, 1);
					SetTeamNum(M.Turret3, 1);
					M.MissionState = 13;
					
					-- Build pilots to get into the remaining tanks if they're around.
					for i = 1, 3 do
						if IsValid(M.Tank[i]) then
							local testme = M.Tank[i]; -- Temp handle for IsAlive Check.
							local SpawnPath = nil;
							
							if not IsAlive(testme) then
							
								if IsValid(M.Base[15]) then
									SpawnPath = "spawn_abbarr1";
								elseif IsValid(M.Base[16]) then 
									SpawnPath = "spawn_abbarr2";
								end
								
								local Pilot = BuildAngleObject("aspilo", 1, SpawnPath, nil, 0, 20);
								GetIn(Pilot, M.Tank[i], 1);
							end
						end
					end
					
					for i = 1, NumBase do
						SetTeamNum(M.Base[i], 1);
					end
					
					for i = 1, MaxSovShips do
						M.SovShips[i] = nil; -- Create this.
					end
				end
				
			elseif M.MissionState == 13 then 
				
				SetScrap(1, 20);
				M.MissionState = 14;
				M.UpdateObjectives = true;
				
			elseif M.MissionState == 14 then 
			
				M.Recy = GetRecyclerHandle(1); -- This allows us to undeploy our Recycler, though i'm not sure why you'd want to. 
			
				-- If the Wave # is less then max waves (10).
				if M.SovietWave < 11 then
					local SpawnShips = true;
					-- Loop over all the ships, find out if any of them are still alive.
					for i = 1, MaxSovShips do
				
						-- All the sov ship's in this wave are dead. Spawn a new one.
						if (IsAlive2(M.SovShips[i]) and GetTeamNum(M.SovShips[i]) == 2) then
							SpawnShips = false;
						end
					end
					
					if SpawnShips then 
						local SpawnCount = 3 + GetRandomInt(7);
						M.SovietWave = M.SovietWave + 1;
						for i = 1, SpawnCount do 
							M.SovShips[i] = BuildAngleObject(GetRandomOffensiveShipODF('s'), 2, "cca_spawn");
							Attack(M.SovShips[i], M.Recy, 0);
						end
						
						-- Support the player if we can.
						for i = M.SovietWave, M.SovietWave + 1 do
							local DroneNumber = M.SovietWave * 2 + 1;
							if i > M.SovietWave then
								DroneNumber = M.SovietWave * 2 + 2;
							end
							
							print("Sending Drone: ", DroneNumber);
							if (DroneNumber ~= 3) and (M.Drone[DroneNumber] ~= nil) and IsAlive(M.Drone[DroneNumber]) then 
								Goto(M.Drone[DroneNumber], GetPositionNear(GetPosition("nav1"), 30, 80), 0);
								--Patrol(M.Drone[DroneNumber], "fodder1", 0); -- Send some of the test drones to help out.
								SetIndependence(M.Drone[DroneNumber], 1); -- Restore rational thought.
							end
						end
						
					end
				
				else -- All waves are dead.
				
					MissionAudioMessage("tran0540.wav");
					SucceedMission(GetTime() + 10, "tran05w2.des");
					M.UpdateObjectives = true;
					M.MissionState = 99;
				
				end
				
				if not IsAround(M.Recy) then 
					MissionAudioMessage("tran0541.wav");
					FailMission(GetTime() + 10, "tran05l3.des");
					M.MissionState = 99;				
				end
				
			
			end
		
		
		end
		
	end    
end


function PreOrdnanceHit(shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF)

	if M.MissionState == 7 then 
	
		-- If the player shoots a drone, make sure he's Targeted it.
		for i = 1, 2 do
			if (shooterHandle == M.Player) and (victimHandle == M.Drone[i]) then
				if GetUserTarget() == victimHandle then 
					M.Nav2Hits = M.Nav2Hits + 1;
				else -- You didn't TARGET the enemy!
					if GetTime() > M.Warn1Time then
						MissionAudioMessage("tran0521.wav"); -- You need to Target the drones, remember to use the T key.
						M.Warn1Time = GetTime() + 10;
					end
				end				
			end
		end
	
	elseif M.MissionState == 10 then 
	
		if shooterHandle == M.Drone[3] and victimHandle == M.Player then -- You got hit. 
			M.Nav3Hits = M.Nav3Hits + 1;
		end
		
	end
	
end


function GetRandomOffensiveShipODF(Race)
-- Gets a random offensive ODF of the specified race.
	return Race .. OffensiveShips[math.random(# OffensiveShips)];
end