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
    * CommaExpression
    * NumberExpression
    * LogicExpression
    * UnaryExpression
    * CalcExpression
    * Statement
      * Block
      * DeclareStatement
      * IfStatement
      * WhileStatement
      * BreakStatemet
      * ContinueStatement
      * VoidStatement
      * ReturnStatement
  * FunctionDefArg
  * Identifier
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
|%|局部变量|
|@|全局变量|

|操作|定义|形式|
|---|---|---|
|add|+|%arg1 = %op1 + %op2|
|sub|-|%arg1 = %op1 - %op2|
|mul|*||
|div|/||
|mod|%||
|cmp| |%res1 = cmp %arg1 %arg2|
|jmp| |jmp label|
|jg||jg label|
