math.randomseed(os.time())

function onInit(id)
	posx = 300
	posy = 300
	r = math.random(0,360)/180.0*3.14
	dirx = math.sin(r)
	diry = math.cos(r)
	speed= math.random(0,5)+1
	MaxLen = 10000
	SetParticle(id,posx,posy,dirx,diry,speed,MaxLen)
end

function onFrameMove(id, frametime)
	posx,posy,dirx,diry,speed,MaxLen=GetParticle(id)
	goalx,goaly =GetGoalPos()


	if frametime% 10 == 0 then
		r = math.random(0,360)/180.0*3.14
		dirx = math.sin(r)
		diry = math.cos(r)
		speed= math.random(0,5)+1		
	end

	tdirx = dirx
	tdiry = diry

	px = goalx - posx
	py = goaly - posy
	pt = GetLength(px, py)
	len = pt/MaxLen	

	plx, ply = Normalize(px,py)
	tdirx = (dirx * (1.0 - len)) + (plx * len)
	tdiry = (diry * (1.0 - len)) + (ply * len)
	
	tdirx, tdiry = Normalize(tdirx, tdiry)

	posx = posx + (tdirx * speed)
	posy = posy + (tdiry * speed)
	
	
	SetParticle(id,posx,posy,dirx,diry,speed,MaxLen)
end







