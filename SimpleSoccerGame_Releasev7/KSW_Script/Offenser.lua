local module = {}

local Offenser = {}
Offenser.__index = Offenser

local function new(id)
  return setmetatable({AgentID=id,FSMrate={},Actrate={}}, Offenser)
end

function Offenser:Init()
    for i=0,100 do 
        self.Actrate[i]=-1
    end
    for i=0,6 do 
        self.FSMrate[i]=0
    end
end

function Offenser:Move()
    pos,getball=GetHomeAgentPos(self.AgentID)

    if getball ==false then
        return 1.0
    end

    minlen=100000
    for i =1,4 do
        pos1,getball1=GetAwayAgentPos(i)
        t = pos1-pos
        len = t:getmag()
        if len<=minlen then
            minlen = len
        end
    end
    if minlen>=300 then
        return 1.0
    else
        return minlen/200.0
    end
end

function Offenser:Pass(id)
    if self.AgentID==id then
        return 0.0
    end
    pos,getball=GetHomeAgentPos(self.AgentID)
    pos1,_=GetHomeAgentPos(id)
    if getball == false then
        return 0.0
    end

    posGK,_ = GetAwayAgentPos(0)
    t3 = posGK - pos
    shootLen = t3:getmag()

    if shootLen <= 1550 then
        return 0.0
    end 

    t = pos1-pos
    len = t:getmag()
    re =1.0-(len/550.0)
    if re<=0.0 then
        re =0.0
    end
    return re
end

function Offenser:Shoot()
    if self.AgentID==id then
        return 0.0
    end
    pos,getball=GetHomeAgentPos(self.AgentID)
    pos1,_=GetAwayAgentPos(0)
    if getball == false then
        return 0.0
    end
    t = pos1-pos
    len = t:getmag()
    re =1.0-len/1550.0
    if re<=0.0 then
        re =0.0
    end
    return re
end
     

function Offenser:Defense()
    pos,_=GetHomeAgentPos(self.AgentID)
    pos1,getball=GetAwayAgentPos(self.AgentID)
    posDefender = GetHomeAgentPos(self.AgentID-2)
    t2  = posDefender - pos1
    defLen = t2:getmag()
    if defLen > 50.0 then
        t = pos1-pos
        t:norm()
        AgentMove(self.AgentID,t)
    else
        posGK,_ = GetAwayAgentPos(0)
        t3 = pos - pos1 
        t4 = pos - posGK
        if t3:getmag() < 800.0 then
            if t4:getmag() < 500.0 then
                t3:norm()
                t3 = t3*0.75
                AgentMove(self.AgentID,t3)
            end
        end
    end

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