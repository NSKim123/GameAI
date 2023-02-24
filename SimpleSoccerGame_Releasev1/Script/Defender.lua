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
        for i=0,4 do
            pos1,getball1=GetAwayAgentPos(i)
            if getball1 == true then
                 break
            end
        end
    end

           
    if self.AgentID == 1 then         
        if Team_ID == 0 then
            goalX = 30
        else 
            goalX = 994
        end
        x = ((2 * pos1.x) + goalX) / 3
        y = (pos1.y + 284) / 2 + (math.sin(GetGameTime()*50) * (200 / 3)) 
        
        l = vector(x,y)
        dir = l - pos
        dir:norm()
        AgentMove(self.AgentID, dir)
    end
    

    if self.AgentID == 2 then         
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