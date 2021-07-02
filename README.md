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
