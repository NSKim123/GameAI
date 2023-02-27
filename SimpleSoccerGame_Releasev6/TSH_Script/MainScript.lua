require("Tool/function")

local Offenser
local Defender1
local Defender2
local Goalkeeper


GAgent=nil
AAgent1= nil
AAgent2= nil
DAgent1= nil
DAgent2= nil

function onInit()
	if Team_ID==HOME then 
	Offenser= require("TSH_Script/Home_Script/TSH_Offenser")
	Defender1= require("TSH_Script/Home_Script/TSH_Defender1")
	Defender2= require("TSH_Script/Home_Script/TSH_Defender2")
	Goalkeeper=require("TSH_Script/Home_Script/TSH_Goalkeeper")
else
	Offenser= require("TSH_Script/Away_Script/TSH_Offenser")
	Defender1= require("TSH_Script/Away_Script/TSH_Defender1")
	Defender2= require("TSH_Script/Away_Script/TSH_Defender2")
	Goalkeeper=require("TSH_Script/Away_Script/TSH_Goalkeeper")
end
printf(Team_ID,HOME)

	GAgent=Goalkeeper()
	AAgent1= Offenser(OFFENSER1)
	AAgent2= Offenser(OFFENSER2)
	DAgent1= Defender1(DEFENDER1)
	DAgent2= Defender2(DEFENDER2)

	GAgent:Init()
	AAgent1:Init()
	AAgent2:Init()
	DAgent1:Init()
	DAgent2:Init()

end

function onFrameMove()
	printf(GetGameTime())
	GAgent:FrameMove()
	AAgent1:FrameMove()
	AAgent2:FrameMove()
	DAgent1:FrameMove()
	DAgent2:FrameMove()
end
