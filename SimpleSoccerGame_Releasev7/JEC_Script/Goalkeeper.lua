local module = {}

local Goalkeeper = {}
Goalkeeper.__index = Goalkeeper

by = 0
bDiry = 0

local function new()
  return setmetatable({AgentID=0}, Goalkeeper)
end

function Goalkeeper:Init()
end

function Goalkeeper:RBE(x, EstmationX, beforeDir)
    p1 = x - EstmationX
    p2 = (p1 * p1)

    mean = (x + EstmationX) / 2.0
    p3 = x - mean
    p4 = EstmationX - mean
    p5 = (p3 * p3) + (p4 * p4)

    result = math.exp(-p2 / (p5 * p5))

    if p2 == 0 then
        result = 0
    end

    dir = p1 * result

    return dir * 0.7 + beforeDir * 0.3
end


function Goalkeeper:FrameMove()
    pos,getball=GetHomeAgentPos(0)
    bpos,bstate= GetBallPos()

    by = bpos.y

    value = self:RBE(bpos.y, by, bDiry)
    value = value * 1000
    bDiry = value
    by = by + bDiry

    bpos.x = 0
    bpos.y = by

    t = bpos - pos
    t:norm()
    
    AgentMove(0, t)
    

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


