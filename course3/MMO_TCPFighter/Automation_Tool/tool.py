import os
from jinja2 import Environment, FileSystemLoader

CSList = list()
SCList = list()

class Function:
    def __init__(self, Name, Code):
        self.name = Name
        self.code = Code
        self.argList = list()

    def pushArg(self, Name, Type):
        self.argList.append(Arg(Name, Type))

class Arg:
    def __init__(self, Name, Type):
        self.name = Name
        self.type = Type

path = str()

def PacketListFileParse():
    f = open("E:\Procademy\PROCADEMEY\course3\MMO_TCPFighter\Automation_Tool\PacketList.txt", 'r')

    PacketDir = str()
    index = 0

    lines = f.readlines()
    for line in lines:
        line = line.strip()
        if not line:
            continue

        # Proxy
        if line == "# Server to Client":
            PacketDir = "SC"
            continue
        
        # Stub
        if line == "# Client to Server":
            PacketDir = "CS"
            continue

        if line[0] != '-':
            p = line.split(' ')
            packetName = PacketDir + p[0]
            packetCode = int(p[1])
            Function(packetName, packetCode)
            if PacketDir == "SC":
                SCList.append(Function(packetName, packetCode))
                index = len(SCList) - 1
            elif PacketDir == "CS":
                CSList.append(Function(packetName, packetCode))
                index = len(CSList) - 1

        else: # argList
            name, type = [l.strip() for l in line[1:].split(':')]

            if PacketDir == "SC":
                SCList[index].pushArg(name, type)
            elif PacketDir == "CS":
                CSList[index].pushArg(name, type)

    f.close()

def JinJaTemplate():
    file_loader = FileSystemLoader("E:\\Procademy\\PROCADEMEY\\course3\\MMO_TCPFighter\\Automation_Tool\\templates")
    env = Environment(loader=file_loader)
    # 파일명
    template1 = env.get_template("DefinePacket.h")
    template2 = env.get_template("GenPacket.h")
    template3 = env.get_template("GenPacket.cpp")
    template4 = env.get_template("ProcessPacket.h")

    result1 = template1.render(scList=SCList, csList=CSList)
    f1 = open('E:\\Procademy\\PROCADEMEY\\course3\\MMO_TCPFighter\\Automation_Tool\\result\\DefinePacket.h', 'w+')
    f1.write(result1)
    f1.close()
    print(result1)

    result2 = template2.render(scList=SCList, csList=CSList)
    f2 = open('E:\\Procademy\\PROCADEMEY\\course3\\MMO_TCPFighter\\Automation_Tool\\result\\GenPacket.h', 'w+')
    f2.write(result2)
    f2.close()
    print(result1)
    
    result3 = template3.render(scList=SCList, csList=CSList)
    f3 = open('E:\\Procademy\\PROCADEMEY\\course3\\MMO_TCPFighter\\Automation_Tool\\result\\GenPacket.cpp', 'w+')
    f3.write(result3)
    f3.close()
    print(result3)

    result4 = template4.render(scList=SCList, csList=CSList)
    f4 = open('E:\\Procademy\\PROCADEMEY\\course3\\MMO_TCPFighter\\Automation_Tool\\result\\ProcessPacket.h', 'w+')
    f4.write(result4)
    f4.close()
    print(result4)

def Main():
    PacketListFileParse()
    JinJaTemplate()
    


if (__name__ == "__main__"):
    Main()
    # os.system("pause")