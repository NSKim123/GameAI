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
    
    ballpos,_=GetBallPos()
    
    
    for i=0,4 do            
        pos1,getball1=GetAwayAgentPos(i)                         
        if getball1 == true then
             break
        end
    end        
    
    

    if getball1==true then
        if Team_ID == 0 then
            goalX = 30
        else 
            goalX = 994
        end
        x = ((19 * pos1.x) + goalX) / 20
        y = (pos1.y + 284) / 2 
            
        l = vector(x,y)
        dir = l - pos
        dir:norm()
        AgentMove(self.AgentID, dir)     
        AgentTackle(self.AgentID)
    else
        dir = ballpos - pos
        dir:norm()
        AgentMove(self.AgentID, dir)
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