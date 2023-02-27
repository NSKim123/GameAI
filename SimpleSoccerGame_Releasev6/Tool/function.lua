vector = require("Tool/vector")
math.randomseed(os.time())

GOALKEEPER =   0
DEFENDER1  =   1
DEFENDER2  =   2
OFFENSER1  =   3
OFFENSER2  =   4

HOME       =   0
AWAY       =   1

Team_ID    =   0

function onSetTeamID(id)
  Team_ID =id
end

----------------------------------------------------
function printf(...)
  for i, str in ipairs{...} do
    if type(str) ~= 'string'then
      Tprintf(tostring(str))
    else
      Tprintf(str)
    end
      Tprintf(" ")
  end
  Tprintf("\n")
  
end

function GetScore()
  return TGetScore(Team_ID)
end

function GetGameTime()
  return TGetGameTime()
end

function WhoAttack()
  return TWhoAttack(Team_ID)
end

function GetBallPos()
  local x,y,state=TGetBallPos()
  return vector(x,y),state
end

function AgentMove(AgentID,dir)
  Tmove(Team_ID,AgentID,dir.x,dir.y)
end

function AgentPass(fromAgentID,toAgentID)
  return Tpass(Team_ID,fromAgentID,toAgentID)
end

function AgentShoot(AgentID)
  return Tshoot(Team_ID,AgentID)
end

function AgentTackle(AgentID)
  return TTackle(Team_ID,AgentID)
end

function GetHomeAgentPos(AgentID)
  local  x,y,getball=TGetHomeAgentPos(Team_ID,AgentID)
  return vector(x,y),getball
end

function GetAwayAgentPos(AgentID)
  local  x,y,getball=TGetAwayAgentPos(Team_ID,AgentID)
  return vector(x,y),getball
end
----------------------------------------------------
