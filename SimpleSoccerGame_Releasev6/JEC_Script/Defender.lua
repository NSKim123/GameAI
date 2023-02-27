local module = {}

local Defender = {}
Defender.__index = Defender

local function new(id)
  return setmetatable({AgentID=id}, Defender)
end

function Defender:Init()
end

function Defender:FrameMove()
    pos,getball=GetHomeAgentPos(self.AgentID)
    if WhoAttack()==AWAY then
        pos1,getball1=GetAwayAgentPos(self.AgentID+2)
        t = pos1-pos
        t:norm()
        AgentMove(self.AgentID,t)
        if getball1==true then
            AgentTackle(self.AgentID)
        end
    end
    if getball==true then
        passid = math.random(1)
        if passid==0 then 
            AgentPass(self.AgentID,OFFENSER1)
        else
            AgentPass(self.AgentID,OFFENSER2)
        end
    end
end

module.new = new
return setmetatable(module, {__call = function(_,...) return new(...) end})