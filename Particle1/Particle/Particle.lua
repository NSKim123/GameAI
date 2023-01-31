math.randomseed(os.time())

function onInit(id)
	posx = 0
	posy = 0
	r = math.random(0,360) / 180.0 * 3.14
	dirx =math.sin(r)
	diry =math.cos(r)
	speed = math.random(0,5) +1
	life= 1
	SetParticle(id, posx, posy, dirx, diry, speed, life)
end

function onFrameMove(id, frametime)
	posx, posy, dirx, diry, speed, life = GetParticle(id)
	goalx, goaly = GetGoalPos()

	if life == 0 then
		posx = 0
		posy = 0
		r= math. random(0,360) / 180.0*3.14
		dirx = math.sin(r)
		diry = math.cos(r)
		speed = math.random(0,5) +1
		life = 1
	end
	
	if frametime % 50 ==0 then
		r=  math. random(0,360) / 180.0*3.14
		dirx = math.sin(r)
		diry = math.cos(r)
		speed = math.random(0,5) +1
	end

	posx = posx + (dirx * speed)
	posy = posy + (diry * speed)

	px = posx - goalx
	py = posy - goaly
	pt = GetLength(px, py)

	if pt <=10 then
		SetGoal(1)
	end

	if posx >= 0 and posx < 600 and posy >=0 and posy < 600 then
		if GetCollision(posx, posy) == 1 then
			life = 0
		end
	else
		life = 0
	end

	SetParticle(id, posx, posy, dirx, diry, speed, life)
end

	