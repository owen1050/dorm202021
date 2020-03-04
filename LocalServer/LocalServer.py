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

        if self.headers["REBOOT"] is not None:
        	postReply = "LocalServerRebooting"
            #impliment computer reboot
        if postReply == "":
            postReply = "ERROR"
        self.wfile.write(str(postReply).encode())

def startStatusServer():
    httpd = HTTPServer(('', 23655), httpServer)
    print ("Starting http server on 23655")
    try:
        httpd.serve_forever()     
    except:         
        httpd.shutdown()         
        print("Shutdown server") 

def maintainContactWithRemote():
    while(True):
    	r = requests.post(remoteUrl, headers = {"GETALL" :"null"})

    	inString = r.text
    	splitString = inString.split(";")
    	for i in splitString:
    		if(i.find("=") > 0):
    			states[i.split('=')[0]] = i.split('=')[1]
    	print(states)

    	if(states["lightsOn"] == "1"):
    		r = requests.get(lightUrl + "lightsOn")
    		r = requests.post(remoteUrl, headers = {"lightsOn" :"0"})
    	if(states["lightsOff"] == "1"):
    		r = requests.get(lightUrl + "lightsOff")
    		r = requests.post(remoteUrl, headers = {"lightsOff" :"0"})
    	if(states["mainOn"] == "1"):
    		r = requests.get(lightUrl + "mainOn")
    		r = requests.post(remoteUrl, headers = {"mainOn" :"0"})
    	if(states["lightsOn"] == "1"):
    		r = requests.get(lightUrl + "lightsOn")
    		r = requests.post(remoteUrl, headers = {"lightsOn" :"0"})
    	if(states["lightsOn"] == "1"):
    		r = requests.get(lightUrl + "lightsOn")
    		r = requests.post(remoteUrl, headers = {"lightsOn" :"0"})
    	if(states["lightsOn"] == "1"):
    		r = requests.get(lightUrl + "lightsOn")
    		r = requests.post(remoteUrl, headers = {"lightsOn" :"0"})
    		
    	time.sleep(0.05)
        
statusServer = threading.Thread(target = startStatusServer)
remoteMaintain = threading.Thread(target = maintainContactWithRemote)
statusServer.start()
remoteMaintain.start()