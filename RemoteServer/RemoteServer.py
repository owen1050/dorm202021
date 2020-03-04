from http.server import BaseHTTPRequestHandler, HTTPServer
import time, threading

states = {"remoteServerOn" : 1, "lightsOn" :0, "lightsOff" : 0, "mainOn" : 0, "mainOff" : 0 ,"hallOn" : 0, "hallOff" : 0, "mainOnHallOff":0, "mainOffHallOn" :0}
lastCheckin = time.time()
remoteServerError = False

class httpServer(BaseHTTPRequestHandler):
    global remoteServerError, lastCheckin, states

    def do_GET(self):
        global lastCheckin
        self.send_response(200)
        self.send_header("content-type", "text/plain")
        self.end_headers()
        self.wfile.write("TheRemoteServerIsUp".encode())
        lastCheckin = time.time()

        
    def do_POST(self):
        self.send_response(200)
        self.send_header('Content-type','text/plain')
        self.end_headers()
        postReply = ""
        
        if self.headers["RESET"] is not None:
            postReply = "ResetAllVarsTo0"
            for x in states:
                states[x] = 0

        if self.headers["REBOOT"] is not None:
            pass
            #impliment computer reboot

        if self.headers["GET"] is not None:
            postReply = states[self.headers["GET"]]

        if self.headers["GETALL"] is not None:
            for x in states:
                postReply = postReply + str(x) + "=" + str(states[x]) + ";"

        for x in states:
            if self.headers[x] is not None:
                postReply = "Changed variable " + x + " to " + self.headers[x]
                states[x] = self.headers[x]
        if postReply == "":
            postReply = "ERROR"
        self.wfile.write(str(postReply).encode())

def run():
    httpd = HTTPServer(('', 23653), httpServer)
    print ("Starting http server on 23653")
    try:         
        httpd.serve_forever()     
    except:         
        httpd.shutdown()         
        print("Shutdown server") 

def checkStatus():
    global remoteServerError
    global lastCheckin
    while remoteServerError==False:
        time.sleep(1)
        if(time.time() - lastCheckin >10):
            print("LocalServerIsDown")
            lastCheckin = time.time()
            #remoteServerError = True
            #impliment IFTTT Text

checker = threading.Thread(target = checkStatus)
checker.start()
run()
