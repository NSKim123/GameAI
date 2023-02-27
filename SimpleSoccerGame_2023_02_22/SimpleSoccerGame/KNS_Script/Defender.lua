local module = {}

local Defender = {}
Defender.__index = Defender

local function new(id)
  return setmetatable({AgentID=id}, Defender)
end

function Defender:Init()
end

function Defender:FrameMove()
    if Team_ID == 0 then
        pos,getball=GetHomeAgentPos(self.AgentID)
    else 
        pos,getball=GetAwayAgentPos(self.AgentID)
    end 
    
    
    if WhoAttack()==AWAY then
        for i=0,4 do
            if Team_ID == 0 then
                pos1,getball1=GetAwayAgentPos(i)
            else 
                pos1,getball1=GetHomeAgentPos(i)
            end             
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
        x = ((9 * pos1.x) + goalX) / 10
        y = (pos1.y + 284) / 2 + (math.sin(GetGameTime()*3.14 / 2) * 20) 
        
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