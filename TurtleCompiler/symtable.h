// symtable.h
// pete myers
// OIT Fall 2008

#ifndef _SYMTABLE_H_
#define _SYMTABLE_H_


//
// type constants used for representing types of variables
//

typedef enum
{
	VT_INT = 0
} VARIABLE_TYPE;


//
// C++ Symbol Table Jan 2017
// Singleton
//

#include <string>
#include <map>
#include <list>

class SymbolTable
{
public:
	class Entry
	{
	public:
		std::string lexeme;
		int token;
		VARIABLE_TYPE type;
		int address;

		Entry(const std::string & lexeme, int token);
		~Entry();
	};

	static SymbolTable * GetInstance();

	Entry * Insert(const std::string & lexeme, int token);
	Entry * Lookup(const std::string & lexeme);

	std::list<Entry*> GetAllEntries();

private:
	SymbolTable();
	~SymbolTable();
	static SymbolTable * theInstance;
	std::map<std::string, Entry*> entries;
};

#endif

