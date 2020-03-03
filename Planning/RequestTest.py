import requests
r = requests.get("http://192.168.212.122:23666")
print(r.text)