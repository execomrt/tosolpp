/*==============================================================================

 $Id$

 tosol.cpp
 tosol++ is an extension of Sol, a tool to integrate C/CPP code with Lua

 ==============================================================================*/

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <array>
#include <list>
#include <map>
#include <vector>

#define TOSOL_VERSION "0.1.0"

#define String std::string

class SignatureDef
{
public:
	void CreateFromString(String& str, bool isCtor);
	std::vector < std::tuple<String, String>> parameters;	
	String Construct(int numParameter);
	String Callers(int numParameters);
	String Signature();
	bool hasDefaultParameter;
	bool isUnary;
	bool isConst;
	int GetNumExplicitParameters() const;

};
class MethodDef
{
public:
	bool isConst;
	bool isStatic;
	bool isOverload;
	String returnType;
	String methodName;
	std::vector<SignatureDef> overloads;

 	

};

class ConstructorDef
{
public:
	bool isOverload;
	SignatureDef overload;
};



class VariableDef
{
public:
	bool isConst;
	bool isStatic;
	String varType;
	String varName;
};

class ClassDef
{
public:
	String name;
	String aliasName;
	String moduleName;
	std::list<VariableDef> variables;
	std::list<MethodDef> methods;
	std::list<ConstructorDef> ctors;
	std::list<String> inherits;
};



class SourceDef
{
public:
	std::list<ClassDef*> classes;
	void Parse(char * value);
	std::vector<String> lines;
	String namespaceName;
	String moduleName;
	String GenerateCode(String name);
};
