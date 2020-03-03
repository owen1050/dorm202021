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

def startStatusServer():
    httpd = HTTPServer(('', 23655), httpServer)
    print ("Starting http server on 23655")
    try:
        httpd.serve_forever()     
    except:         
        httpd.shutdown()         
        print("Shutdown server") 

def run():
    while(True):
    	r = requests.post("http://127.0.0.1:23654", headers = {"GETALL" :"null"})
    	inString = r.text
    	splitString = inString.split(";")
    	for i in splitString:
    		if(i.find("=") > 0):
    			states[i.split('=')[0]] = i.split('=')[1]
    	print(states)
    	time.sleep(5)
        
statusServer = threading.Thread(target = startStatusServer)
statusServer.start()
run()