for i = 1, 9, 1 do
	for j = 1, 9, 1 do
		p= i * j
		printf(tostring(p))		
	end
	printf("\n")
end

function GetFunc(val1, val2)
	printf(tostring(val1+val2))
	return val1+ val2, val1*val2
end