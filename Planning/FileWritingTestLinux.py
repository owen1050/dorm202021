import pathlib

currentPath = str(pathlib.Path().absolute())
pathAbove = currentPath[0:currentPath.rfind("/")+1]
logsPath = pathAbove+"Logs/Logs.txt"
f = open(logsPath, "r")
print(f.read())
f.close()
