// tree.c
// by Pete Myers
// OIT Portland, Fall 2008
//
// Assign3 Handout

#include "tree.h"
#include <stdlib.h>
#include "symtable.h"


//
// print tree
//

void printindent(FILE * f, int indent)
{
	for (; indent > 0; indent--)
		fprintf(f, "    ");
}

char * nodetypestring(NODE_TYPE t)
{
	switch (t)
	{
	case NT_BLOCK:
		return "BLOCK";
	case NT_NUMBER:
		return "NUMBER";
	case NT_COLOR_NAME:
		return "COLOR_NAME";
	case NT_FUNCTION:
		return "FUNCTION";
	case NT_TURTLE_CMD:
		return "TURTLE_CMD";
	case NT_IF:
		return "IF";
	case NT_IFELSE:
		return "IFELSE";
	case NT_OPERATOR:
		return "OPERATOR";
	case NT_REPEAT:
		return "REPEAT";
	case NT_DECLARATION:
		return "DECLARATION";
	case NT_VARIABLE:
		return "VARIABLE";
	case NT_ASSIGNMENT:
		return "ASSIGNMENT";
	default:
		return "UNKNOWN";
	}
}

char * turtlecmdtypestring(TURTLE_CMD cmd)
{
	switch (cmd)
	{
	case CMD_HOME:
		return "HOME";
	case CMD_FD:
		return "FD";
	case CMD_BK:
		return "BK";
	case CMD_RT:
		return "RT";
	case CMD_LT:
		return "LT";
	case CMD_SETC:
		return "SETC";
	case CMD_SETXY:
		return "SETXY";
	case CMD_PD:
		return "PD";
	case CMD_PU:
		return "PU";
	case CMD_SETH:
		return "SETH";
	case CMD_SETX:
		return "SETX";
	case CMD_SETY:
		return "SETY";
	case CMD_HT:
		return "HT";
	case CMD_ST:
		return "ST";
	default:
		return "UNKNOWN";
	}
}

char * colorstring(COLOR_TYPE c)
{
	switch (c)
	{
	case BLACK:
		return "BLACK";
	case WHITE:
		return "WHITE";
	case ORANGE:
		return "ORANGE";
	case YELLOW:
		return "YELLOW";
	case LIME:
		return "LIME";
	case CYAN:
		return "CYAN";
	case BLUE:
		return "BLUE";
	case MAGENTA:
		return "MAGENTA";
	case RED:
		return "RED";
	case BROWN:
		return "BROWN";
	case GREEN:
		return "GREEN";
	case TURQUOISE:
		return "TURQUOISE";
	case SKY:
		return "SKY";
	case VIOLET:
		return "VIOLET";
	case PINK:
		return "PINK";
	case GOLD:
		return "GOLD";
	case SILVER:
		return "SILVER";
	case BRONZE:
		return "BRONZE";
	default:
		return "UNKNOWN";
	}
}

char * functionstring(FUNCTION_TYPE f)
{
	switch (f)
	{
	case FT_COLOR:
		return "COLOR";
	case FT_XCOR:
		return "XCOR";
	case FT_YCOR:
		return "YCOR";
	case FT_RANDOM:
		return "RANDOM";
	case FT_HEADING:
		return "HEADING";
	default:
		return "UNKNOWN";
	}
}

char * operatorstring(OPERATOR_TYPE op)
{
	switch (op)
	{
	case OT_EQUALS:
		return "=";
	case OT_LESSTHAN:
		return "<";
	case OT_GREATERTHAN:
		return ">";
	case OT_PLUS:
		return "+";
	case OT_MINUS:
		return "-";
	case OT_TIMES:
		return "*";
	case OT_DIVIDE:
		return "/";
	default:
		return "UNKNOWN";
	}
}

char * typestring(VARIABLE_TYPE type)
{
	switch (type)
	{
	case VT_INT:
		return "INT";
	default:
		return "UNKNOWN";
	}
}


//
// C++ Syntax Tree Jan 2017
// 

TreeNode::TreeNode(NODE_TYPE type)
{
	this->type = type;
	this->parent = NULL;
	this->firstChild = NULL;
	this->secondChild = NULL;
	this->thirdChild = NULL;
}

TreeNode::~TreeNode()
{
	std::list<TreeNode*>::iterator iter;
	for (iter = children.begin(); iter != children.end(); iter++)
	{
		delete (*iter);
	}
}

void TreeNode::SetParent(TreeNode * parent)
{
	this->parent = parent;
}

TreeNode * TreeNode::GetParent()
{
	return parent;
}

void TreeNode::AddChild(TreeNode * child)
{
	children.push_back(child);
	child->parent = this;
	if (firstChild == NULL)
		firstChild = child;
	else if (secondChild == NULL)
		secondChild = child;
	else if (thirdChild == NULL)
		thirdChild = child;
}

std::list<TreeNode*> TreeNode::GetChildren()
{
	return children;
}

TreeNode * TreeNode::FirstChild()
{
	return firstChild;
}

TreeNode * TreeNode::SecondChild()
{
	return secondChild;
}

TreeNode * TreeNode::ThirdChild()
{
	return thirdChild;
}

void TreeNode::AdoptChildren(TreeNode * fromParent)
{
	std::list<TreeNode*>::iterator iter;
	for (iter = fromParent->children.begin(); iter != fromParent->children.end(); iter++)
	{
		AddChild(*iter);
	}
	fromParent->children.clear();
}

void TreeNode::PrintTree(FILE * out)
{
	// print this node and all it's children
	int indent = 0;
	PrintNode(out, indent);
}

void TreeNode::PrintNode(FILE * out, int indent)
{
	printindent(out, indent);
	fprintf(out, "%s ", nodetypestring(type));
	PrintNodeAttributes(out);
	fprintf(out, "\n");
	
	// print children
	indent++;
	std::list<TreeNode*>::iterator iter;
	for (iter = children.begin(); iter != children.end(); iter++)
	{
		(*iter)->PrintNode(out, indent);
	}
}


//
// Node type sub-classes
//

BlockTreeNode::BlockTreeNode() : TreeNode(NT_BLOCK)
{
}

NumberTreeNode::NumberTreeNode(int value) : TreeNode(NT_NUMBER)
{
	this->value = value;
}

void NumberTreeNode::PrintNodeAttributes(FILE * out)
{
	fprintf(out, "%d", value);
}

ColorNameTreeNode::ColorNameTreeNode(COLOR_TYPE color) : TreeNode(NT_COLOR_NAME)
{
	this->color = color;
}

void ColorNameTreeNode::PrintNodeAttributes(FILE * out)
{
	fprintf(out, "%s", colorstring(color));
}

FunctionTreeNode::FunctionTreeNode(FUNCTION_TYPE func) : TreeNode(NT_FUNCTION)
{
	this->func = func;
}

FunctionTreeNode::FunctionTreeNode(FUNCTION_TYPE func, TreeNode * param) : TreeNode(NT_FUNCTION)
{
	this->func = func;
	AddChild(param);
}

void FunctionTreeNode::PrintNodeAttributes(FILE * out)
{
	fprintf(out, "%s", functionstring(func));
}

TurtleCmdTreeNode::TurtleCmdTreeNode(TURTLE_CMD cmd) : TreeNode(NT_TURTLE_CMD)
{
	this->cmd = cmd;
}

TurtleCmdTreeNode::TurtleCmdTreeNode(TURTLE_CMD cmd, TreeNode * param) : TreeNode(NT_TURTLE_CMD)
{
	this->cmd = cmd;
	AddChild(param);
}

TurtleCmdTreeNode::TurtleCmdTreeNode(TURTLE_CMD cmd, TreeNode * param1, TreeNode * param2) : TreeNode(NT_TURTLE_CMD)
{
	this->cmd = cmd;
	AddChild(param1);
	AddChild(param2);
}

void TurtleCmdTreeNode::PrintNodeAttributes(FILE * out)
{
	fprintf(out, "%s", turtlecmdtypestring(cmd));
}

IfTreeNode::IfTreeNode(TreeNode * condition, BlockTreeNode * block) : TreeNode(NT_IF)
{
	AddChild(condition);
	AddChild(block);
}

IfElseTreeNode::IfElseTreeNode(TreeNode * condition, BlockTreeNode * trueBlock, BlockTreeNode * falseBlock) : TreeNode(NT_IFELSE)
{
	AddChild(condition);
	AddChild(trueBlock);
	AddChild(falseBlock);
}

OperatorTreeNode::OperatorTreeNode(OPERATOR_TYPE op, TreeNode * lhs, TreeNode * rhs) : TreeNode(NT_OPERATOR)
{
	this->op = op;
	AddChild(lhs);
	AddChild(rhs);
}

void OperatorTreeNode::PrintNodeAttributes(FILE * out)
{
	fprintf(out, "%s", operatorstring(op));
}

RepeatTreeNode::RepeatTreeNode(TreeNode * loopFor, BlockTreeNode * block) : TreeNode(NT_REPEAT)
{
	AddChild(loopFor);
	AddChild(block);
}

DeclarationTreeNode::DeclarationTreeNode(VARIABLE_TYPE vartype, VariableTreeNode * variable) : TreeNode(NT_DECLARATION)
{
	this->vartype = vartype;
	AddChild(variable);
}

void DeclarationTreeNode::PrintNodeAttributes(FILE * out)
{
	fprintf(out, "%s", typestring(vartype));
}

VariableTreeNode::VariableTreeNode(SymbolTable::Entry * symentry) : TreeNode(NT_VARIABLE)
{
	this->symentry = symentry;
}

void VariableTreeNode::PrintNodeAttributes(FILE * out)
{
	fprintf(out, "%s", symentry->lexeme.c_str());
}

AssignmentTreeNode::AssignmentTreeNode(VariableTreeNode * lhs, TreeNode * rhs) : TreeNode(NT_ASSIGNMENT)
{
	AddChild(lhs);
	AddChild(rhs);
}
