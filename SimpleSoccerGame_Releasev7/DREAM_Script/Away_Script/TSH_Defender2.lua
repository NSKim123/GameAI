
local module = {}

local Defender = {}
Defender.__index = Defender

local function new(id)
  return setmetatable({AgentID=id}, Defender)
end

function Defender:Init()
end

function Defender:FrameMove()

    --When Game Start or Rematch
    posSelf,getballSelf = GetHomeAgentPos(self.AgentID)
    posGK,getBallGK = GetHomeAgentPos(GOALKEEPER)
    posA1,getBallA1 = GetHomeAgentPos(OFFENSER1)
    posA2,getBallA2 = GetHomeAgentPos(OFFENSER2)

    d2_aim_x = 954
    d2_aim_y = 334

    gk_mid_x = 954
    gk_mid_y = 284

    d2_location = vector(d2_aim_x, d2_aim_y) - posSelf
    goal_Mid = vector(gk_mid_x, gk_mid_y) - posSelf

    d2_location:norm()
    goal_Mid:norm()


    --Shoot or Pass
    if getballSelf == true then
        --Shoot
        if posA1.x > ground_mid_x and posA2.x > ground_mid_x then
            --AgentShoot(self.AgentID)
            AgentShoot(DEFFENDER2)
        end
        
        --Pass to Further Player
        if posA1.x > posA2.x then
            AgentPass(self.AgentID,OFFENSER2)
        else
            AgentPass(self.AgentID,OFFENSER1)
        end
    end


    --When Game Start or Rematch move to GK
    if posSelf.x ~= posGK.x then

        if posGK.y < 334 then
            AgentMove(self.AgentID, d2_location)
            AgentTackle(self.AgentID)
        end

        if posGK.y > 334 then
            AgentMove(self.AgentID, goal_Mid)
            AgentTackle(self.AgentID)
        end
    end


    

end

module.new = new
return setmetatable(module, {__call = function(_,...) return new(...) end})