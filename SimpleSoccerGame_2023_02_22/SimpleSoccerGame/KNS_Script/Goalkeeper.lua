local module = {}

local Goalkeeper = {}
Goalkeeper.__index = Goalkeeper

local function new()
  return setmetatable({AgentID=0}, Goalkeeper)
end

function Goalkeeper:Init()
end

function Goalkeeper:FrameMove()
    if Team_ID == 0 then
        pos,getball=GetHomeAgentPos(GOALKEEPER)
    else 
        pos,getball=GetAwayAgentPos(GOALKEEPER)
    end 
    
    bpos,bstate= GetBallPos()
    bpos.x=0
    pos.x=0
    t= (bpos-pos):norm()
    AgentMove(GOALKEEPER,t)
    if getball==true then
        passid = math.random(1)
        if passid==0 then 
            AgentPass(GOALKEEPER,DEFENDER1)
        else
            AgentPass(GOALKEEPER,DEFENDER2)
        end
    end
   
end

module.new = new
return setmetatable(module, {__call = function(_,...) return new(...) end})


