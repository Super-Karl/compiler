# gen c file from ir

import sys
res = ""
firstFunction = 0
variable = {}
def putint(l):
    if l[0] in VariableInfo:
        item = VariableInfo[l[0]]
    else:
        item = int(l[0])
    puts.append(item)

def putch(l):
    if l[0] in VariableInfo:
        item = VariableInfo[l[0]]
    else:
        item = int(l[0])
    puts.append(chr(item - ord('0')))


def getint(l):
    num = 0
    while True:
        c = data.read(1)
        if c != '\s':
            num = int(c)
            break
    while True:
        c = data.read(1)
        if c in "\s\t\n":
            break
        else:
            num = num*10+int(c)
    return sum


def getch(l):
    while True:
        c = data.read(1)
        if c == '\s':
            continue
        elif '-' in l:
            retirm
        else:
            return c

def CtypeName(i,ctr = True):
    if '%' in i or '@' in i:
        return "Var"+i[1:]
    elif ctr:
        if '-' in i :
            return f"({i})"
        else:
            return i
    else:
        return i

SpecCallMap = {
    "putch": putch,
    "putint": putint,
    "getch": getch,
    "getint": getint
}
# arithmic instructions


def add(l):  
    global res  
    print(f"int {CtypeName(l[0])}={CtypeName(l[1])}+{CtypeName(l[2])};")
    res = CtypeName(l[0])



def sub(l):
    global res 
    print(f"int {CtypeName(l[0])}={CtypeName(l[1])}-{CtypeName(l[2])};")
    res = CtypeName(l[0])


def mul(l):
    global res 
    print(f"int {CtypeName(l[0])}={CtypeName(l[1])}*{CtypeName(l[2])};")
    res = CtypeName(l[0])


def div(l):
    global res 
    print(f"int {CtypeName(l[0])}={CtypeName(l[1])}/{CtypeName(l[2])};")
    res = CtypeName(l[0])


def mod(l):
    global res 
    print(f"int {CtypeName(l[0])}={CtypeName(l[1])}%{CtypeName(l[2])};")
    res = CtypeName(l[0])


def assign(l):
    global res 
    print(f"int {CtypeName(l[0])}={CtypeName(l[1])};")
    res = CtypeName(l[0])

# call and return


def call(List):
    global res 
    name = List[0][1:]
    if "&" in List:
        args = List[1:-2]
        print(f"int {CtypeName(List[-1][:-1])} =",end=' ')
        res = CtypeName(List[-1][:-1])
        print(f"{name}(",end=' ')
        if len(args) != 0:
            for i in args[:-1]:
                print(CtypeName(i),end=',')
            print(f"{CtypeName(args[-1])});\n")
        else:
            print(");")
    else:
        args = List[1:]
        print(f"{name}(",end=' ')
        for i in args:
            print(CtypeName(i),end=' ')
        print(");\n")
        


def ret(l):
    if len(l)==0 or l[0] == '':
        print(f"return 0;")
    else:
        print(f"return {CtypeName(l[0])};")

# jump instructions


def Jmp(l):
    label = l[0][1:-1]
    print (f"goto Label_{label};")


def Jge(l):
    label = l[0][1:-1]
    print(f"if ({res} >= 0) goto Label_{label};")


def Jg(l):
    label = l[0][1:-1]
    print(f"if ({res} > 0) goto Label_{label};")


def Jle(l):
    label = l[0][1:-1]
    print(f"if ({res} <= 0) goto Label_{label};")


def Jl(l):
    label = l[0][1:-1]
    print(f"if ({res} < 0) goto Label_{label};")


def Jne(l):
    label = l[0][1:-1]
    print(f"if ({res} != 0) goto Label_{label};")


def Jeq(l):
    label = l[0][1:-1]
    print(f"if ({res} == 0) goto Label_{label};")
# logic instructions


def vnot(op):
    return not op


def nop(i):
    """nop"""
    pass


def load(l):
    """load()"""
    print(f"int {CtypeName(l[0])}={CtypeName(l[2][:-1])}[{CtypeName(l[1],ctr=False)}];")

def store(l):
    """store()"""
    print(f"{CtypeName(l[0])}[{CtypeName(l[1],ctr=False)}]={CtypeName(l[2][:-1])};")
    pass


def alloca(l):
    """alloca()"""
    print(f"int {CtypeName(l[0][:-1],ctr=False)}[{CtypeName(l[2][1:-1],ctr=False)}];")
    


opMap = {
    "Add": add,
    "Sub": sub,
    "Mul": mul,
    "Div": div,
    "Mod": mod,
    "Call": call,
    "Jmp": Jmp,
    "Jg": Jg,
    "Jge": Jge,
    "Jl": Jl,
    "Jle": Jle,
    "Cmp": sub,
    "Jne": Jne,
    "Jeq": Jeq,
    "Not": vnot,
    "Mov":assign,
    "Alloca":alloca,
    "Ret":ret,
    "Nop":nop,
    "Load": load,
    "Store":store
}
def excute(line):
    global res,firstFunction
    if line[0] == '\t':
        line = line[1:]
    inst = line.split(' ')
    print('//',inst)
    if '@' == line[0]:
        if firstFunction :
            print('}')
        else:
            firstFunction = 1
        if len(inst) == 2:
            print(f"int {inst[0][1:-1]}()",end='{\n')
        else:
            print(f"int {inst[0][1:-1]}(",end="")
            for i in inst[1:-2] :
                if "&" in i:
                    print(f"int {i[1:]} []",end=",")
                else:
                    print(f"int {i}",end=',')
            i = inst[-2]
            if "&" in i:
                print(f"int {i[1:]} []",end="){\n")
            else:
                print(f"int {i}",end="){\n")
        return
    if '$' in line:
        return
    if ':' in line:
        print(f"Label_{inst[0][1:-1]};")
        return
    opMap[inst[0]](inst[1:])

if __name__ == "__main__":
    args = sys.argv
    
    if '-h' in args:
        print("""
        -h: help infomation
        -a/-auto: automatically read from the ir file with the name 'gen.ir' and read data from 'test.in'
        -tty : input the ir file from standard input 
        """)
    if '-auto' in args or '-a' in args:
        file = open("gen.ir", 'r')
        data = open("test.data", "r")
        IR = file.readlines()

    if '-tty' in args:
        tmpstr = input()
        try:
            while True:
                IR.append(tmpstr)
                tmpstr = input()
        except KeyboardInterrupt:
            pass
    print("""
#include <stdio.h>
void putint(int c){
    printf("%d",c);
}
void putch(int c){
    printf("%c",c);
}
int getint(){
    int tmp;
    scanf("%d",&tmp);
    return tmp;
}
int getch(){
    char tmp;
    scanf("%c",&tmp);
    return tmp;
}    
    """)
    for line in IR:
        excute(line)
    print('}')