x1=-2
x2=2
y1=16
y2=17
z1=4
z2=8
k=1
for x in xrange(x1,x2):
	for y in xrange(y1,y2):
		for z in xrange(z1,z2):
			print 'LightSource "shaders/shadowlight" '+str(k)+' "intensity" 10 "from" ['+str(x)+' '+str(y)+' '+str(z)+']'
			k=k+1
