// tree.h
// by Pete Myers
// OIT Portland, Fall 2008

#ifndef _TREE_H_
#define _TREE_H_

#include <stdio.h>
#include "symtable.h"


typedef enum
{
	NT_BLOCK = 0,
	NT_NUMBER = 1,
	NT_COLOR_NAME = 2,
	NT_FUNCTION = 3,
	NT_TURTLE_CMD = 4,
	NT_IF = 5,
	NT_IFELSE = 6,
	NT_OPERATOR = 7,
	NT_REPEAT = 8,
	NT_DECLARATION = 9,
	NT_VARIABLE = 10,
	NT_ASSIGNMENT = 11
} NODE_TYPE;

typedef enum
{
	OT_EQUALS = 0,
	OT_LESSTHAN = 1,
	OT_GREATERTHAN = 2,
	OT_PLUS = 3,
	OT_MINUS = 4,
	OT_TIMES = 5,
	OT_DIVIDE = 6
} OPERATOR_TYPE;

typedef enum
{
	BLACK = 0x00,
	WHITE = 0x01,
	ORANGE = 0x02,
	YELLOW = 0x03,
	LIME = 0x04,
	CYAN = 0x05,
	BLUE = 0x06,
	MAGENTA = 0x07,
	RED = 0x08,
	BROWN = 0x09,
	GREEN = 0x0a,
	TURQUOISE = 0x0b,
	SKY = 0x0c,
	VIOLET = 0x0d,
	PINK = 0x0e,
	// new colors
	GOLD = 0x0f,
	SILVER = 0x10,
	BRONZE = 0x11

} COLOR_TYPE;

typedef enum
{
	FT_COLOR = 0,
	FT_XCOR = 1,
	FT_YCOR = 2,
	FT_RANDOM = 3,
	FT_HEADING = 4
} FUNCTION_TYPE;

typedef enum
{
	CMD_HOME = 0x00,
	CMD_FD = 0x01,
	CMD_BK = 0x02,
	CMD_RT = 0x03,
	CMD_LT = 0x04,
	CMD_SETC = 0x05,
	CMD_SETXY = 0x07,
	CMD_PD = 0x0b,
	CMD_PU = 0x0c,
	CMD_SETX = 0x0d,
	CMD_SETY = 0x0e,
	CMD_HT = 0x0f,
	CMD_ST = 0x10,
	CMD_SETH = 0x11
} TURTLE_CMD;


//
// C++ Syntax Tree Jan 2017
// 

#include <list>

// forward declarations
class TreeNode;
class BlockTreeNode;
class NumberTreeNode;
class ColorNameTreeNode;
class FunctionTreeNode;
class TurtleCmdTreeNode;
class IfTreeNode;
class IfElseTreeNode;
class OperatorTreeNode;
class RepeatTreeNode;
class DeclarationTreeNode;
class VariableTreeNode;
class AssignmentTreeNode;


// Node classes

class TreeNode		// abstract base class
{
private:
	NODE_TYPE type;
	TreeNode * parent;
	std::list<TreeNode*> children;
	TreeNode * firstChild;
	TreeNode * secondChild;
	TreeNode * thirdChild;

	void PrintNode(FILE * out, int indent);		// print this node, it's attributes and then it's children with indent

protected:
	TreeNode(NODE_TYPE type);
	virtual void PrintNodeAttributes(FILE * out) { }	// override to print attributes of concrete class

public:
	virtual ~TreeNode();

	NODE_TYPE NodeType() { return type; }

	void SetParent(TreeNode * parent);
	TreeNode * GetParent();

	void AddChild(TreeNode * child);
	std::list<TreeNode*> GetChildren();
	TreeNode * FirstChild();
	TreeNode * SecondChild();
	TreeNode * ThirdChild();
	void AdoptChildren(TreeNode * fromParent);

	void PrintTree(FILE * out);		// entry point for printing whole tree

	virtual int GenerateNode(unsigned char * program, int pc) { return pc; }	// override to generate code for this node
};

class BlockTreeNode : public TreeNode
{
public:
	BlockTreeNode();
};

class NumberTreeNode : public TreeNode
{
private:
	int value;
public:
	NumberTreeNode(int value);
	int Value() { return value; }
protected:
	virtual void PrintNodeAttributes(FILE * out);
};

class ColorNameTreeNode : public TreeNode
{
private:
	COLOR_TYPE color;
public:
	ColorNameTreeNode(COLOR_TYPE color);
	COLOR_TYPE Color() { return color; }
protected:
	virtual void PrintNodeAttributes(FILE * out);
};

class FunctionTreeNode : public TreeNode
{
private:
	FUNCTION_TYPE func;
public:
	FunctionTreeNode(FUNCTION_TYPE func);
	FunctionTreeNode(FUNCTION_TYPE func, TreeNode * param);
	FUNCTION_TYPE Function() { return func; }
protected:
	virtual void PrintNodeAttributes(FILE * out);
};

class TurtleCmdTreeNode : public TreeNode
{
private:
	TURTLE_CMD cmd;
public:
	TurtleCmdTreeNode(TURTLE_CMD cmd);
	TurtleCmdTreeNode(TURTLE_CMD cmd, TreeNode * param);
	TurtleCmdTreeNode(TURTLE_CMD cmd, TreeNode * param1, TreeNode * param2);
	TURTLE_CMD Command() { return cmd; }
protected:
	virtual void PrintNodeAttributes(FILE * out);
};

class IfTreeNode : public TreeNode
{
public:
	IfTreeNode(TreeNode * condition, BlockTreeNode * block);
};

class IfElseTreeNode : public TreeNode
{
public:
	IfElseTreeNode(TreeNode * condition, BlockTreeNode * trueBlock, BlockTreeNode * falseBlock);
};

class OperatorTreeNode : public TreeNode
{
private:
	OPERATOR_TYPE op;
public:
	OperatorTreeNode(OPERATOR_TYPE op, TreeNode * lhs, TreeNode * rhs);
	OPERATOR_TYPE Operator() { return op; }
protected:
	virtual void PrintNodeAttributes(FILE * out);
};

class RepeatTreeNode : public TreeNode
{
public:
	RepeatTreeNode(TreeNode * loopFor, BlockTreeNode * block);
};

class DeclarationTreeNode : public TreeNode
{
private:
	VARIABLE_TYPE vartype;
public:
	DeclarationTreeNode(VARIABLE_TYPE vartype, VariableTreeNode * variable);
protected:
	virtual void PrintNodeAttributes(FILE * out);
};

class VariableTreeNode : public TreeNode
{
private:
	SymbolTable::Entry * symentry;
public:
	VariableTreeNode(SymbolTable::Entry * symentry);
	int Address() { return symentry->address; }
protected:
	virtual void PrintNodeAttributes(FILE * out);
};

class AssignmentTreeNode : public TreeNode
{
public:
	AssignmentTreeNode(VariableTreeNode * lhs, TreeNode * rhs);
};


// factory
class TreeNodeFactory
{
public:
	virtual BlockTreeNode * CreateBlock() { return new BlockTreeNode(); }
	virtual NumberTreeNode * CreateNumber(int value) { return new NumberTreeNode(value); }
	virtual ColorNameTreeNode * CreateColorName(COLOR_TYPE color) { return new ColorNameTreeNode(color); }
	virtual FunctionTreeNode * CreateFunction(FUNCTION_TYPE func) { return new FunctionTreeNode(func); }
	virtual FunctionTreeNode * CreateFunction(FUNCTION_TYPE func, TreeNode * param) { return new FunctionTreeNode(func, param); }
	virtual TurtleCmdTreeNode * CreateTurtleCmd(TURTLE_CMD cmd) { return new TurtleCmdTreeNode(cmd); }
	virtual TurtleCmdTreeNode * CreateTurtleCmd(TURTLE_CMD cmd, TreeNode * param) { return new TurtleCmdTreeNode(cmd, param); }
	virtual TurtleCmdTreeNode * CreateTurtleCmd(TURTLE_CMD cmd, TreeNode * param1, TreeNode * param2) { return new TurtleCmdTreeNode(cmd, param1, param2); }
	virtual IfTreeNode * CreateIf(TreeNode * condition, BlockTreeNode * block) { return new IfTreeNode(condition, block); }
	virtual IfElseTreeNode * CreateIfElse(TreeNode * condition, BlockTreeNode * trueblock, BlockTreeNode * falseblock) { return new IfElseTreeNode(condition, trueblock, falseblock); }
	virtual OperatorTreeNode * CreateOperator(OPERATOR_TYPE op, TreeNode * lhs, TreeNode * rhs) { return new OperatorTreeNode(op, lhs, rhs); }
	virtual RepeatTreeNode * CreateRepeat(TreeNode * loopFor, BlockTreeNode * block) { return new RepeatTreeNode(loopFor, block); }
	virtual DeclarationTreeNode * CreateDeclaration(VARIABLE_TYPE vartype, VariableTreeNode * variable) { return new DeclarationTreeNode(vartype, variable); }
	virtual VariableTreeNode * CreateVariable(SymbolTable::Entry * symentry) { return new VariableTreeNode(symentry); }
	virtual AssignmentTreeNode * CreateAssignment(VariableTreeNode * lhs, TreeNode * rhs) { return new AssignmentTreeNode(lhs, rhs); }
};


#endif

