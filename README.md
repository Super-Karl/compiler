# Compiler

## namespace

|namespace|dir|usage|
|---|---|---|
|compiler|this Project|top namespace|
|compiler::controller|src/controller|control the whole project work|
|compiler::front::ast|src/front/ast|AST node define|

## AST继承关系

* Node
    * Expression
        * FunctionDefArg
        * FunctionDefArgList
        * FunctionDefine
        * FunctionCallArgList
        * FunctionCall
        * CommaExpression :unuse
        * NumberExpression
        * LogicExpression
        * UnaryExpression
        * CalcExpression
        * ArrayInitVal
        * Statement
            * Block
            * DeclareStatement
            * AssignStmt
            * IfStatement
            * WhileStatement
            * BreakStatemet
            * ContinueStatement
            * VoidStatement
            * ReturnStatement
    * FunctionDefArg
    * Identifier
        * ArrayIdentifier
    * Declare
        * VarDeclare
        * VarDeclareWithInit
        * ConstDeclare
        * ArrayDeclare
        * ConstArray
        * ArrayDeclareWithInit

## IR定义

|符号|意义|
|---|---|
|&|数组符号|
|%|局部变量|
|@|全局变量|

|操作|定义|形式|
|---|---|---|
|add|+|%arg1 = add %op1 %op2|
|sub|-|%arg1 = sub %op1 %op2|
|mul|*||
|div|/||
|mod|%||
|cmp| |%res1 = cmp %arg1 %arg2|
|jmp| |jmp label|
|jg|大于跳转|jg label|
|jge|大于等于跳转|jge label|
|je|等于跳转|je label|
|jne|不等于跳转|jne label|
|jl|小于|jl label|
|jle|小于等于跳转|jle label|
|=| |%arg1 = %arg2|
|and| | |
|or| | |
|not| | |
|xor| | |
|sal|%res1 = %op1 << %op2 |%res1 = sal %op1 %op2|
|sar|%res1 = %op1 >> %op2 |%res1 = sar %op1 %op2|
|store|%op1 = %op2装载到内存|store %op1 %op2|
|load|%op1 = %op2内存转载到寄存器|load %op1 %op2 |
|label| |label labelName |
|nop| | |

### testTool的使用
因为生成的是arm汇编,所以自动测试脚本只能在arm的docker或者arm的linux机器上运行

**使用**

1. 查看帮助
```shell
./test.sh -h
```

2. 测试所有样例
```shell
./test.sh -a
```

3. 测试单个样例
```shell
./test.sh -a file
```