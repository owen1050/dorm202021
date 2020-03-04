from http.server import BaseHTTPRequestHandler, HTTPServer
import time, threading, requests

states = {"remoteServerOn" : 1, "lightsOn" :0, "lightsOff" : 0, "mainOn" : 0, "mainOff" : 0 ,"hallOn" : 0, "hallOff" : 0, "mainOnHallOff":0, "mainOffHallOn" :0}
lastCheckin = time.time()
remoteServerError = False
remoteUrl = "http://100.35.205.75:23653"
lightUrl = "http://192.168.212.122/"

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
	while(True):
		time.sleep(5)
		try:
			r = requests.get(remoteUrl)
			
		except:
			pass
			#call ifttt error for remote
		try:
			r = requests.get(lightUrl)
			
		except:
			pass
			#call ifttt error for lights


def getRemoteVars():
	prevVar = ""
	lt = 0
	while(True):
		try:
			try:
				r = requests.post(remoteUrl, headers = {"GETALL" :"null"})
			except:
				pass
				#remote failed
			inString = r.text
			if(prevVar == inString):
				if(int(time.time())%10 == 0 and str(int(time.time())) != lt):
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
							#make sure lights dont fail
				time.sleep(0.05)

		except Exception as e:
			print(e)
			#just to ensure no failure
		
statusServer = threading.Thread(target = startStatusServer)
varUpdate = threading.Thread(target = getRemoteVars)
maintainRemote = threading.Thread(target = maintainContact)

statusServer.start()
varUpdate.start()
maintainRemote.start()