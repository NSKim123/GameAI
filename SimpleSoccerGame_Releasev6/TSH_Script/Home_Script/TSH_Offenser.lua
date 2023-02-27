local module = {}

local Offenser = {}
Offenser.__index = Offenser

local function new(id)
  return setmetatable({AgentID=id,FSMrate={},Actrate={}}, Offenser)
end

function Offenser:Init()
    --Initiation:clear
    for i=0,100 do 
        self.Actrate[i]=-1
    end

    for i=0,6 do 
        self.FSMrate[i]=0
    end
end

function Offenser:Move()
    pos,getball=GetHomeAgentPos(self.AgentID)
    
    -- id selection
    if self.AgentID == OFFENSER1 then
        anotherID = OFFENSER2
    else
        anotherID = OFFENSER1
    end


    if getball == false then
        return 1.0
    end

    --MUST MODIFY
    --Find nearest player
    minlen=100000
    for i =1,4 do --1234 EnemyPlayer
        pos1,getball1=GetAwayAgentPos(i) -- EnemyPlayer
        t = pos1-pos
        len = t:getmag()
        if len<=minlen then -- find min
            minlen = len
        end
    end
    -- more than 200 just run
    if minlen>=200 then
        return 1.0
    else
        -- if close 200 run rate UP
        return minlen/200.0
    end
end

function Offenser:Pass(id)
    if self.AgentID==id then
        return 0.0
    end

    pos,getball=GetHomeAgentPos(self.AgentID)
    pos1,_=GetHomeAgentPos(id)
    posAwayGK,_=GetAwayAgentPos(0)


    -- Not Ball Cant Pass
    if getball == false then
        return 0.0
    end

    -- more close more pass <-- NO!!!
    t1 = pos1-pos
    t2 = posAwayGK-pos

    lenH = t1:getmag()
    lenA = t2:getmag()

    --re =1.0-(len/550.0)
    --re = len/550.0
    re = 0
    --[[
    if re<=0.0 then
        re =0.0
    end
    ]]
    return re
end

function Offenser:Shoot()
    ---?????????
    if self.AgentID==id then
        return 0.0
    end

    pos,getball=GetHomeAgentPos(self.AgentID)
    pos1,_=GetAwayAgentPos(0)
    if getball == false then
        return 0.0
    end

    re = 1
    --[[
    t = pos1-pos
    len = t:getmag()
    re =1.0-len/550.0
    if re<=0.0 then
        re =0.0
    end
    ]]
    return re
end
     

function Offenser:Defense()
    pos,_=GetHomeAgentPos(self.AgentID)
    pos1,getball=GetAwayAgentPos(self.AgentID)
    t = pos1-pos
    t:norm()
    AgentMove(self.AgentID,t)
    if getball==true then
        AgentTackle(self.AgentID)
    end
end

function Offenser:MakeFSMTable(id)
    count =0
    if self.FSMrate[id]==0 then
        return 0
    end
    while(1)
    do
        pos = math.random(0, 99)
        
        if self.Actrate[pos]==-1 then
            self.Actrate[pos] = id
            count =count+1
        end
        if count==self.FSMrate[id] then
            return 0
        end
        
    end
end

function Offenser:SelectAction()
    while(1)
    do
        pos = math.random(0, 99)
        
        if self.Actrate[pos] ~=-1 then
           return self.Actrate[pos]
        end
    end
end

function Offenser:onAction(id)
    if id==0 then
        pos,getball=GetHomeAgentPos(self.AgentID)
        if getball==true then
            pos1,_=GetAwayAgentPos(0)
            t = pos1-pos
            t:norm()
            AgentMove(self.AgentID,t)
        else
            pos1,_=GetBallPos()
            t = pos1-pos
            if t:getmag()<=100 then
                t:norm()
                AgentMove(self.AgentID,t)
            else
                pos1,_=GetAwayAgentPos(0)
                t = pos1-pos
                t:norm()
                AgentMove(self.AgentID,t)
            end
        end
       
    end
    if id==1 then
        AgentPass(self.AgentID,GOALKEEPER)
    end
    if id==2 then
        AgentPass(self.AgentID,DEFENDER1)
    end
    if id==3 then
        AgentPass(self.AgentID,DEFENDER2)
    end
    if id==4 then
        AgentPass(self.AgentID,OFFENSER1)
    end
    if id==5 then
        AgentPass(self.AgentID,OFFENSER2)
    end
    if id==6 then
        AgentShoot(self.AgentID)
    end
end

function Offenser:Attack()
    self.FSMrate[0] = math.floor(self:Move()*100.0)
    self.FSMrate[1] = math.floor(self:Pass(0)*100.0)
    self.FSMrate[2] = math.floor(self:Pass(1)*100.0)
    self.FSMrate[3] = math.floor(self:Pass(2)*100.0)
    self.FSMrate[4] = math.floor(self:Pass(3)*100.0)
    self.FSMrate[5] = math.floor(self:Pass(4)*100.0)
    self.FSMrate[6] = math.floor(self:Shoot()*100.0)
    tmax = 0
    for i=0,6 do
        tmax =tmax+self.FSMrate[i]
    end
    for i=0,6 do
        self.FSMrate[i] = math.floor(self.FSMrate[i]/tmax*100.0)
    end
    tmax = 0
    for i=0,6 do
        tmax =tmax+self.FSMrate[i]
    end
    for i=0,100 do 
        self.Actrate[i]=-1
    end
    for i=0,6 do
        self:MakeFSMTable(i)
    end
    action =self:SelectAction()
    self:onAction(action)
end

function Offenser:FrameMove()
    if WhoAttack()==HOME then
        self:Attack()
    else
        self:Defense()
    end
    

end


module.new = new
return setmetatable(module, {__call = function(_,...) return new(...) end})