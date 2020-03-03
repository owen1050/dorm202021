from http.server import BaseHTTPRequestHandler, HTTPServer
import time, threading, requests

states = {"remoteServerOn" : 1, "lightsOn" :0, "lightsOff" : 0}
lastCheckin = time.time()
remoteServerError = False

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
    	r = requests.post("http://127.0.0.1:23654", headers = {"GETALL" :"null"})
    	inString = r.text
    	splitString = inString.split(";")
    	for i in splitString:
    		if(i.find("=") > 0):
    			states[i.split('=')[0]] = i.split('=')[1]
    	print(states)

    	if(states["lightsOn"] == "1"):
    		r = requests.get("http://192.169.212.122/lightsOn")
    		
    	time.sleep(1)
        
statusServer = threading.Thread(target = startStatusServer)
remoteMaintain = threading.Thread(target = maintainContactWithRemote)
statusServer.start()
remoteMaintain.start()