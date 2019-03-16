// symtable.c
// pete myers
// OIT Fall 2008

#include "symtable.h"


//
// C++ Symbol Table Jan 2017
// Singleton
//

// The one instance
SymbolTable * SymbolTable::theInstance = NULL;

SymbolTable * SymbolTable::GetInstance()
{
	if (theInstance == NULL)
	{
		theInstance = new SymbolTable();
	}

	return theInstance;
}

SymbolTable::Entry * SymbolTable::Insert(const std::string & lexeme, int token)
{
	Entry * entry = Lookup(lexeme);
	if (entry == NULL)
	{
		entry = new Entry(lexeme, token);
		entries[lexeme] = entry;
	}

	return entry;
}

SymbolTable::Entry * SymbolTable::Lookup(const std::string & lexeme)
{
	std::map<std::string, Entry*>::iterator iter = entries.find(lexeme);
	if (iter != entries.end())
		return iter->second;

	return NULL;
}

std::list<SymbolTable::Entry*> SymbolTable::GetAllEntries()
{
	std::list<Entry*> all;
	std::map<std::string, Entry*>::iterator iter;
	for (iter = entries.begin(); iter != entries.end(); iter++)
	{
		all.push_back(iter->second);
	}
	return all;
}

SymbolTable::SymbolTable()
{
}

SymbolTable::~SymbolTable()
{
	std::map<std::string, Entry*>::iterator iter;
	for (iter = entries.begin(); iter != entries.end(); iter++)
	{
		if (iter->second != NULL)
		{
			delete iter->second;
			iter->second = NULL;
		}
	}
}

SymbolTable::Entry::Entry(const std::string & lexeme, int token)
{
	this->lexeme = lexeme;
	this->token = token;
	this->type = (VARIABLE_TYPE)-1;
	this->address = -1;
}

SymbolTable::Entry::~Entry()
{
}
