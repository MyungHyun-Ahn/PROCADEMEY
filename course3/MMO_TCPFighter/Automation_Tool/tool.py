import os
import jinja2

PacketList = dict()
PacketArgList = dict()


def PacketListFileParse():
    path = os.path.dirname(os.path.realpath(__file__))
    f = open(path + "/Template/PacketList.txt", 'r')

    PacketDir = str()
    PacketName = str()

    lines = f.readlines()
    for line in lines:
        line = line.strip()
        if not line:
            continue

        # Stub
        if line == "# Server to Client":
            PacketDir = "SC"
            continue
        
        # Proxy
        if line == "# Client to Server":
            PacketDir = "CS"
            continue

        if line[0] != '-':
            p = line.split(' ')
            PacketName = PacketDir + p[0]
            PacketList[PacketName] = int(p[1])
            PacketArgList[PacketName] = dict()
        else: # argList
            key, value = [l.strip() for l in line[1:].split(':')]
            PacketArgList[PacketName][key] = int(value)

    f.close()

def Main():
    PacketListFileParse()
    


if (__name__ == "__main__"):
    Main()