
local module = {}

local Defender = {}
Defender.__index = Defender

local function new(id)
  return setmetatable({AgentID=id}, Defender)
end

function Defender:Init()
end

function Defender:FrameMove()
    --=================== MODIFY ===================
    --When Game Start or Rematch
    posSelf,getballSelf = GetHomeAgentPos(self.AgentID)
    posGK,getBallGK = GetHomeAgentPos(GOALKEEPER)
    posA1,getBallA1 = GetHomeAgentPos(OFFENSER1)
    posA2,getBallA2 = GetHomeAgentPos(OFFENSER2)

    d1_aim_x = 80;
    d1_aim_y = 234;

    gk_mid_x = 80;
    gk_mid_y = 284;

    ground_mid_x = 512;

    d1_location = vector(d1_aim_x, d1_aim_y) - posSelf
    goal_Mid = vector(gk_mid_x, gk_mid_y) - posSelf

    d1_location:norm()
    goal_Mid:norm()

    --Shoot or Pass
    if getballSelf == true then
        --Shoot
        if posA1.x < ground_mid_x or posA2.x < ground_mid_x then
            --AgentShoot(self.AgentID)
            AgentShoot(DEFFENDER1)
        end
        
        --Pass to Further Player
        if posA1.x < posA2.x then
            AgentPass(self.AgentID,OFFENSER2)
        else
            AgentPass(self.AgentID,OFFENSER1)
        end
    end

    
    --When Game Start or Rematch move to GK
    if posSelf.x ~= posGK.x then
        AgentMove(self.AgentID, d1_location)

        if posGK.y > 234 then
            AgentMove(self.AgentID, d1_location)
        end
    
        if posGK.y < 234 then
            AgentMove(self.AgentID, goal_Mid)
        end
    end
end

module.new = new
return setmetatable(module, {__call = function(_,...) return new(...) end})