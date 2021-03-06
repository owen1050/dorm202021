from http.server import BaseHTTPRequestHandler, HTTPServer
import time, threading, requests

states = {"remoteServerOn" : 1, "lightsOn" :0, "lightsOff" : 0, "mainOn" : 0, "mainOff" : 0 ,"hallOn" : 0, "hallOff" : 0, "mainOnHallOff":0, "mainOffHallOn" :0, "projectorOn" :0,"projectorOff" :0,"volumeUp2" :0,"volumeDown2" :0,"volumeUp5" :0,"volumeDown5" :0,"auxOne" :0,"auxTwo" :0,"phono" :0,"mute" :0,"cd" :0,"openBlinds" :0,"closeBlinds" :0, "stopBlinds":0}
lastCheckin = time.time()
remoteServerError = False
remoteUrl = "http://100.35.205.75:23655"
lightUrl = "http://192.168.212.124/"
blindsUrl = "http://192.168.212.122/"
IRUrl = "http://192.168.212.125/"
iftttErrorURL = "https://maker.ifttt.com/trigger/dormError/with/key/Bf91G_MsjKUzsWqRs5N7n"
lastError = 20

class httpServer(BaseHTTPRequestHandler):
	global remoteServerError, lastCheckin, states

	def do_GET(self):
		self.send_response(200)
		self.send_header("content-type", "text/plain")
		self.end_headers()
		self.wfile.write("TheLocalServerIsUp".encode())

	def do_POST(self):
		self.send_response(200)
		self.send_header('Content-type','text/plain')
		self.end_headers()
		postReply = ""
		rebootV = False

		if self.headers["REBOOT"] is not None:
			postReply = "LocalServerRebooting"
			rebootV = True
			
		if postReply == "":
			postReply = "ERROR"

		self.wfile.write(str(postReply).encode())

		if(rebootV):
			pass
			#impliment reboot

def startStatusServer():
	httpd = HTTPServer(('', 23655), httpServer)
	print ("Starting http server on 23655")
	try:
		httpd.serve_forever()	 
	except:		 
		httpd.shutdown()		 
		print("Shutdown server") 

def maintainContact():
	remoteErrors = 0
	lightErrors = 0
	blindErrors = 0
	irErrors = 0

	while(True):
		time.sleep(4)
		try:
			r = requests.get(remoteUrl, timeout = 1)
			remoteErrors = 0
		except:
			print("REMOTE FAILED")
			remoteErrors = remoteErrors + 1

		try:
			r = requests.get(lightUrl, timeout = 1)
			lightErrors = 0
		except:
			lightErrors = lightErrors + 1
			print("LIGHTS FAILED")

		try:
			r = requests.get(blindsUrl, timeout = 1)
			blindErrors = 0
		except:
			blindErrors = blindErrors + 1
			print("blinds FAILED")

		try:
			r = requests.get(IRUrl, timeout = 1)
			irErrors = 0
		except:
			irErrors = irErrors + 1
			print("IR FAILED")
		
		if(lightErrors > 4):
			iftttError("LightsHaveFailled5Times")
		if(remoteErrors > 4):
			iftttError("RemoteCanNotBeContacted5Times")
		if(blindErrors > 4):
			iftttError("BlindsHaveFailled5Times")
		if(irErrors > 4):
			iftttError("IRHaveFailled5Times")

def iftttError(s):
	global lastError
	if(time.time() - lastError > 600):
		print("ifttt sent:"+s)
		r = requests.post(iftttErrorURL, headers = {"Content-Type": "application/json"}, data = "{\"value1\":\""+s+"\"}")
		lastError = time.time()
def getRemoteVars():
	prevVar = ""
	lt = 0
	while(True):
		try:
			r = requests.post(remoteUrl, headers = {"GETALL" :"null"})
			
			inString = r.text
			if(prevVar == inString):
				if(int(time.time())%3 == 0 and str(int(time.time())) != lt):
					lt = str(int(time.time()))
					print("No Vars Changed:"+ lt)

				time.sleep(0.05)
			else:
				prevVar = inString
				splitString = inString.split(";")
				for i in splitString:
					if(i.find("=") > 0):
						states[i.split('=')[0]] = i.split('=')[1]
				log = "New Data:" + str(time.time()) + ":"+ str(states)
				print(log)
				#impliment some txt logging

				lightArduinoPoss = ["lightsOn","lightsOff","mainOn", "mainOff", "hallOn", "hallOff", "mainOnHallOff", "mainOffHallOn"]
				for par in lightArduinoPoss:
					if(states[par] == "1"):
						try:
							r = requests.get(lightUrl + par)
							r = requests.post(remoteUrl, headers = {par :"0"})
						except Exception as e:
							print(e)

				blindArduinoPoss = ["openBlinds", "closeBlinds", "stopBlinds"]
				for par in blindArduinoPoss:
					if(states[par] == "1"):
						try:
							r = requests.get(blindsUrl + par)
							r = requests.post(remoteUrl, headers = {par :"0"})
						except Exception as e:
							print(e)

				irArduinoPoss = ["projectorOn" ,"projectorOff" ,"volumeUp2" ,"volumeDown2" ,"volumeUp5" ,"volumeDown5" ,"auxOne" ,"auxTwo" ,"phono","mute" ,"cd"]
				for par in irArduinoPoss:
					if(states[par] == "1"):
						try:
							r = requests.get(IRUrl + par)
							r = requests.post(remoteUrl, headers = {par :"0"})
						except Exception as e:
							print(e)

				time.sleep(0.05)

		except Exception as e:
			print(str(e)[:50])
			time.sleep(1)
		
statusServer = threading.Thread(target = startStatusServer)
varUpdate = threading.Thread(target = getRemoteVars)
maintainRemote = threading.Thread(target = maintainContact)

statusServer.start()
varUpdate.start()
maintainRemote.start()