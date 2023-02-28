local module = {}

local Goalkeeper = {}
Goalkeeper.__index = Goalkeeper

local function new()
  return setmetatable({AgentID=0}, Goalkeeper)
end

function Goalkeeper:Init()
end

function Goalkeeper:FrameMove()
    pos,getball=GetHomeAgentPos(GOALKEEPER)
    bpos,bstate= GetBallPos()
    bpos.x=0
    pos.x=0
    t= (bpos-pos)
    len = t:getmag()
    t:norm()

    posOff1,_ = GetAwayAgentPos(OFFENSER1)
    posOff2,_ = GetAwayAgentPos(OFFENSER2)

    posOff = (posOff2-posOff1)/2


    if len > 1000.0 then
        t2 = posOff - pos
        t2:norm()
        AgentMove(GOALKEEPER,t2)    
    else
        AgentMove(GOALKEEPER,t)
    end
    
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


