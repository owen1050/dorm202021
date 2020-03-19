states = {"remoteServerOn" : 1, "lightsOn" :0, "lightsOff" : 0, "mainOn" : 0}
states2 = {"remoteServerOff" : 1, "lights2" :0, "lights2" : 0, "main2" : 0}
states3 = states2.update(states)
for i in states:
	print(i)
	if "li" in i:
		print("GGG")