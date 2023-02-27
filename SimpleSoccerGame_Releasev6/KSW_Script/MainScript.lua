require("Tool/function")
local Offenser= require("KSW_Script/Offenser")
local Defender= require("KSW_Script/Defender")
local Goalkeeper=require("KSW_Script/Goalkeeper")

GAgent=nil
AAgent1= nil
AAgent2= nil
DAgent1= nil
DAgent2= nil

function onInit()
	GAgent=Goalkeeper()
	AAgent1= Offenser(OFFENSER1)
	AAgent2= Offenser(OFFENSER2)
	DAgent1= Defender(DEFENDER1)
	DAgent2= Defender(DEFENDER2)

	GAgent:Init()
	AAgent1:Init()
	AAgent2:Init()
	DAgent1:Init()
	DAgent2:Init()

end

function onFrameMove()
	GAgent:FrameMove()
	AAgent1:FrameMove()
	AAgent2:FrameMove()
	DAgent1:FrameMove()
	DAgent2:FrameMove()
end
