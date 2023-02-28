require("Tool/function")
local Offenser= require("JEC_Script/Offenser")
local Offenser2 = require("JEC_Script/Offenser2")
local Defender= require("JEC_Script/Defender")
local Goalkeeper=require("JEC_Script/Goalkeeper")

GAgent=nil
AAgent1= nil
AAgent2= nil
DAgent1= nil
DAgent2= nil

function onInit()
	GAgent=Goalkeeper()
	AAgent1= Offenser(OFFENSER1)
	AAgent2= Offenser2(OFFENSER2)
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
