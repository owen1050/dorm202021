import requests
iftttErrorURL = "http://maker.ifttt.com/trigger/dormError/with/key/Bf91G_MsjKUzsWqRs5N7n"
r = requests.post(iftttErrorURL, headers = {"Content-Type": "application/json"}, data = "{\"value1\":\"python\"}")
print(r.text)