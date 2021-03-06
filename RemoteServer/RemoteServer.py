from http.server import BaseHTTPRequestHandler, HTTPServer
import time, threading, requests, os

states = {"remoteServerOn" : 1, "lightsOn" :0, "lightsOff" : 0, "mainOn" : 0, "mainOff" : 0 ,"hallOn" : 0, "hallOff" : 0, "mainOnHallOff":0, "mainOffHallOn" :0, "projectorOn" :0,"projectorOff" :0,"volumeUp2" :0,"volumeDown2" :0,"volumeUp5" :0,"volumeDown5" :0,"auxOne" :0,"auxTwo" :0,"phono" :0,"mute" :0,"cd" :0,"openBlinds" :0,"closeBlinds" :0, "stopBlinds":0}
lastCheckin = time.time()
remoteServerError = False
lastError = 0
iftttErrorURL = "https://maker.ifttt.com/trigger/dormError/with/key/Bf91G_MsjKUzsWqRs5N7n"

class httpServer(BaseHTTPRequestHandler):
    global remoteServerError, lastCheckin, states

    def log_message(self, format, *args):
        return

    def do_GET(self):
        global lastCheckin
        self.send_response(200)
        self.send_header("content-type", "text/plain")
        self.end_headers()
        
        lastCheckin = time.time()
        print("Get Request at:" + str(time.time()))

        data = "nan"
        try:
            content_length = int(self.headers['Content-Length'])
            print(content_length)
            data = str(self.rfile.read(content_length))
            print(data)
        except:
            pass

        postReply = ""
        reboot = False

        if "RESET" in data:
            postReply = "ResetAllVarsTo0"
            print("Received POST for Reset at:" + str(time.time()))
            for x in states:
                states[x] = 0

        if "REBOOT" in data:
            print("Received POST for Reboot. Server Should be up in 3 minutes")
            postReply = "Rebooting server please wait 3 minutes"
            reboot = True

        for x in states:
            if x in data:
                postReply = "Changed variable " + x + " to 1"
                print("received post for " + postReply)
                states[x] = 1

        if postReply == "":
            postReply = "ERROR"

        if(reboot):
            passw = "adminadmin"
            com = "reboot"
            p = os.system("echo %s|sudo -S %s" %(passw, com))

        self.wfile.write("your Request has been processed".encode())
        
    def do_POST(self):
        self.send_response(200)
        self.send_header('Content-type','text/plain')
        self.end_headers()
        postReply = ""
        reboot = False

        if self.headers["RESET"] is not None:
            postReply = "ResetAllVarsTo0"
            print("Received POST for Reset at:" + str(time.time()))
            for x in states:
                states[x] = 0

        if self.headers["REBOOT"] is not None:
            print("Received POST for Reboot. Server Should be up in 3 minutes")
            postReply = "Rebooting server please wait 3 minutes"
            reboot = True

        if self.headers["GET"] is not None:
            postReply = states[self.headers["GET"]]
            print("Received POST for GET:"+ self.headers["GET"] + ":"+ postReply)

        if self.headers["GETALL"] is not None:
            print("Received POST for GETALL at:" + str(time.time()))
            for x in states:
                postReply = postReply + str(x) + "=" + str(states[x]) + ";"

        for x in states:
            if self.headers[x] is not None:
                postReply = "Changed variable " + x + " to " + self.headers[x]
                print("received post for " + postReply)
                states[x] = self.headers[x]
        if postReply == "":
            postReply = "ERROR"
        self.wfile.write(str(postReply).encode())

        if(reboot):
            passw = "adminadmin"
            com = "reboot"
            p = os.system("echo %s|sudo -S %s" %(passw, com))

def run():
    httpd = HTTPServer(('', 23655), httpServer)
    print ("Starting http server on 23655")
    try:         
        httpd.serve_forever()     
    except:         
        httpd.shutdown()         
        print("Shutdown server") 

def iftttError(s):
    global lastError
    if(time.time() - lastError > 60):
        print("ifttt contacted:" + s)
        r = requests.post(iftttErrorURL, headers = {"Content-Type": "application/json"}, data = "{\"value1\":\""+s+"\"}")
        lastError = time.time()

def checkStatus():
    global remoteServerError
    global lastCheckin
    while True:
        time.sleep(1)
        if(time.time() - lastCheckin >40):
            print("LocalServerIsDown")
            lastCheckin = time.time()
            iftttError("Local server could not be contacted")            

checker = threading.Thread(target = checkStatus)
checker.start()
run()
