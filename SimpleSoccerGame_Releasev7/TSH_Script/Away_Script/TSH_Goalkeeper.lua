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
    t= (bpos-pos):norm()

    --=================== MODIFY ===================
    posA1,getBallA1 = GetHomeAgentPos(OFFENSER1)
    posA2,getBallA2 = GetHomeAgentPos(OFFENSER2)

    
    AgentMove(GOALKEEPER,t)

    -- Pass to Offenser Further
    if getball==true then
        --
        --AgentShoot(0, GOALKEEPER)
        if posA1.x < posA2.x then
            --=================== MODIFY ===================
            AgentPass(GOALKEEPER,OFFENSER1)
        else
            --=================== MODIFY ===================
            AgentPass(GOALKEEPER,OFFENSER2)
        end
    end
   
end

--class화 시키기위한거
module.new = new
return setmetatable(module, {__call = function(_,...) return new(...) end})


