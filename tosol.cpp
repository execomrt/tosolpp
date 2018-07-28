/*==============================================================================
 $Id$
 tosol.cpp
 tosol++ is an extension of Sol, a tool to integrate C/CPP code with Lua
 Uses the same format(PKG) than tolua++ (https://github.com/LuaDist/toluapp)
 Sol2 https://github.com/ThePhD/sol2
 
 This project : https://github.com/execomrt/tosolpp
 ==============================================================================*/

#include "tosol.h"


std::map<String, String> fields;
String trim(const String& str,
	const String& whitespace = " \t")
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == String::npos)
		return ""; // no content
	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;
	return str.substr(strBegin, strRange);
}
std::vector<String> &split(const String &s, char delim, std::vector<String> &elems) {
	std::stringstream ss(s);
	String item;
	while (std::getline(ss, item, delim)) {
		if (item.length() > 0) {
			elems.push_back(trim(item));
		}
	}
	return elems;
}
std::vector<String> split(const String &s, char delim) {
	std::vector<String> elems;
	split(s, delim, elems);
	return elems;
}
String GetParameterType(String& param)
{
	int index = 0;
	String ret;
	auto s = split(param, ' ');
	for (auto it : s)
	{
		index++;
		bool isLast = index == s.size() - 1;
		if (ret.length() > 0) {
			ret = ret + " ";
		}
		ret = ret + it;
		if (isLast) {
			return ret;
		}
	}
	return ret;
}
String PointerToSharedPtr(String& typeName)
{
	if (fields.find("x") != fields.end()) {
		if (typeName.find("*") != -1 && typeName.find("char") == -1)  // don't process 'char*'
		{
			return "std::shared_ptr<" + typeName.substr(0, typeName.length() - 1) + ">";
		}
	}
	return typeName;
}
int SignatureDef::GetNumExplicitParameters() const
{
	int ret = 0;
	for (auto it : parameters) {
		auto value = std::get<1>(it);
		if (value == "") {
			ret = ret + 1;
		}
	}
	return ret;
}
void SignatureDef::CreateFromString(String & paramBlock, bool isCtor)
{
	
	hasDefaultParameter = false;
	auto parameterList = split(paramBlock.substr(1, paramBlock.length() - 2), ',');
	for (auto p : parameterList) {
		auto i = p.find('=');
		if (i != -1) {
			hasDefaultParameter = true;
			// c++ 17
			std::tuple<String, String> pair = { trim(p.substr(0, i)),
				trim(p.substr(i + 1)) };
			parameters.push_back(pair);
		}
		else {
			auto closure = p.find_last_of(")");
			if (closure != -1) {
				p = p.substr(0, closure);
			}
			auto v = split(p, ' ');
			if (v.size() >= 2 && isCtor) {
				if (v.size() == 3) {
					std::tuple<String, String> pair = { v[0] + " " + v[1], v[2] };
					parameters.push_back(pair);
				}
				else
				{
					std::tuple<String, String> pair = { v[0], v[1] };
					parameters.push_back(pair);
				}
			}
			else {
				std::tuple<String, String> pair = { p, "" };
				parameters.push_back(pair);
			}
		}
	}
}
String SignatureDef::Construct(int numParameter)
{
	int index = 0;
	String ret = "(";
	for (auto it : parameters) {
		bool isLast = numParameter == 0 ;
		auto param = std::get<0>(it);
		auto value = std::get<1>(it);
		auto typeName = PointerToSharedPtr(GetParameterType(param));
		index++;
		if (value.length() > 0) {
			if (isLast) {
				return ret + ")";
			}
			if (index > 0 && ret.length() > 1) {
				ret = ret + ", ";
			}
			ret = ret + typeName;
			numParameter--;
		}
		else {
			if (index > 0 && ret.length() > 1) {
				ret = ret + ", ";
			}
			if (typeName != ")") {
				ret = ret + typeName;
			}
		}
	}
	ret = ret + ")";
	return ret;
}

String SignatureDef::Signature()
{
	int index = 0;
	String ret = "(";
	for (auto it : parameters) {
		
		auto param = std::get<0>(it);
		auto value = std::get<1>(it);

		index++;
		if (value.length() > 0) {
		
			if (index > 0 && ret.length() > 1) {
				ret = ret + ", ";
			}
			ret = ret + param + " " + value;
		
		}
		else {
			if (index > 0 && ret.length() > 1) {
				ret = ret + ", ";
			}
			if (value != ")") {
				ret = ret + param + " " + value;
			}
		}
	}
	ret = ret + ")";
	if (ret == "( )")
		return "(void)";

	return ret;
}
String SignatureDef::Callers(int numParameter)
{
	int index = 0;
	String ret = "(";
	for (auto it : parameters) {
		bool isLast = numParameter == 0;
		auto param = std::get<0>(it);
		auto value = std::get<1>(it);
	
		index++;
		if (value.length() > 0) {
			if (isLast) {
				return ret + ")";
			}
			if (index > 0 && ret.length() > 1) {
				ret = ret + ", ";
			}
			ret = ret + value;
			numParameter--;
		}
		else {
			if (index > 0 && ret.length() > 1) {
				ret = ret + ", ";
			}
			if (value != ")") {
				ret = ret + value;
			}
		}
	}
	ret = ret + ")";
	return ret;
}

void SourceDef::Parse(char* value)
{
	char * v = strtok(value, "\n\r");
	while (v) {
		if (*v) {
			char * begin = v;
			while (isspace(*begin))
				begin++;
			if (*begin != '/')
			{
				while (*v) {
					if (*v == ';') {
						*v = 0;
					}
					if (*v == '//') {
						*v = 0;
					}
					v++;
				}
				if (*begin != '//') {
					char * op = strstr(begin, "operator");
					if (op) {
						char * d = op + 8;
						char * s = d;
						while (isspace(*s)) {
							s++;
						}
						while (*s) {
							*d = *s;
							d++;
							s++;
						}
						*d = 0;
					}
					lines.push_back(trim(begin));
				}
			}
		}
		v = strtok(nullptr, "\n\r");
	}
	ClassDef* newClass = nullptr;
	int state = 0;
	for (auto it : lines) {
		String paramBlock;
		String defBlock;
		String constBlock;
		auto paramBlockStart = it.find_last_of('(');
		auto paramBlockEnd = it.find_last_of(')');
		if (paramBlockStart != -1 && paramBlockEnd != -1)
		{
			paramBlock = it.substr(paramBlockStart, paramBlockEnd - paramBlockStart + 1  );
			defBlock = it.substr(0, paramBlockStart);
			constBlock = trim(it.substr(paramBlockEnd+1));
		}
		else {
			defBlock = it;
		}

		auto elemt = split(defBlock, ' ');
		if (elemt.size() > 0) {
			if (state == 1) {
				String methodName = elemt[elemt.size() - 1];
				if (paramBlock.length() > 0 && newClass != nullptr) {
					if (methodName == newClass->name)
					{
						ConstructorDef newCtor;
						SignatureDef sig;						
						newCtor.overload.CreateFromString(paramBlock, true);						
						newClass->ctors.push_back(newCtor);
					}
					else
					{
						MethodDef newMethod;
						newMethod.isConst = false;
						newMethod.methodName = methodName;
						if (elemt.size() >= 2) {
							if (elemt[0] == "const" &&
								elemt[1] == "static")
							{
								auto tmp = elemt[0]; elemt[0] = elemt[1]; elemt[1] = tmp;
							}
						}
						
						if (elemt[0] == "static") {
							newMethod.isStatic = true;							
							newMethod.returnType = elemt[1];
						}
						else {
							newMethod.isStatic = false;
							newMethod.returnType = elemt[0];
							if (newMethod.returnType == "const") {
								newMethod.returnType = newMethod.returnType + " " + elemt[1];
							}
						}
						bool isOverload = false;
						SignatureDef sig;
						sig.isConst = constBlock == "const";						
						sig.CreateFromString(paramBlock, false);
						bool isUnary = false;
						
						if (newMethod.methodName == "operator-") {
							if (sig.parameters.size() == 1 && std::get<0>(sig.parameters[0]) == "")
							{
								isUnary = true;
							}
						}
					

						if (!isUnary) {
							for (auto& met : newClass->methods) {

							
								if (met.methodName == newMethod.methodName &&
									met.returnType == newMethod.returnType && !met.overloads[0].isUnary) {
									met.isOverload = true;
									sig.isUnary = false;
									met.overloads.push_back(sig);
									isOverload = true;

								}
							}

						}
						else
						{
							isUnary = isUnary;
						}
						if (!isOverload)
						{
							if (sig.hasDefaultParameter) {
								newMethod.isOverload = true;
							}
							sig.isUnary = isUnary;
							newMethod.overloads.push_back(sig);
							newClass->methods.push_back(newMethod);
						}
					}
				}
				else if (newClass != nullptr) {
					if (elemt[0] == "public" || elemt[0] == "protected" || elemt[0] == "private")
					{
					}
					else if (elemt.size() >= 2)
					{
						bool isStatic = false;
						bool isConst = false;
						String varType, varBlock;
						int varBlockIndex = 0;

						if (elemt.size() >= 2) {
							if (elemt[0] == "const" &&
								elemt[1] == "static")
							{
								auto tmp = elemt[0]; elemt[0] = elemt[1]; elemt[1] = tmp;
							}
						}

						if (elemt[0] == "static") {
							isStatic = true;
							if (elemt[1] == "const") {
								isConst = true;
								varType = elemt[2];
								varBlockIndex = 3;
							}
							else
							{
								varType = elemt[1];
								varBlockIndex = 2;
							}
						}
						else {
							isStatic = false;
							if (elemt[1] == "const") {
								isConst = true;
								varType = elemt[1];
								varBlockIndex = 2;
							}
							else
							{
								varType = elemt[0];
								varBlockIndex = 1;
							}
						}
						for (int i = varBlockIndex; i < elemt.size(); i++) {
							varBlock = elemt[i];
							auto vars = split(varBlock, ',');
							for (auto varNames : vars) {
								VariableDef newVar;
								newVar.isConst = isConst;
								newVar.isStatic = isStatic;
								newVar.varType = varType;
								newVar.varName = varNames;
								newClass->variables.push_back(newVar);
							}
						}
					}
				}
			}
			{
				if (elemt[0] == "{") {
					state = 1;
				}
				if (elemt[0] == "$using") {
					if (elemt[1] == "namespace") {
						namespaceName = elemt[2];
					}
				}
				if (elemt[0] == "module") {
					moduleName = elemt[1];
				}
				else if (elemt[0] == "class" || elemt[0] == "struct") {
					newClass = new ClassDef();
					newClass->name = elemt[1];
					if (elemt.size() >= 2) {
						bool hasInherits = false;
						for (int k = 2; k < elemt.size(); k++) {							
							if (elemt[k] == "{") {
								state = 1;
							}
							else if (hasInherits) {
								newClass->inherits.push_back(elemt[k]);
							}
							else if (elemt[k] == "public") {
								hasInherits = true;
							}
							if (elemt[k] == "@") {
								newClass->aliasName = elemt[k + 1];
							}
						}
					}
				}
				if (elemt[0] == "}") {
					if (newClass) {						
						classes.push_back(newClass);
					}
					newClass = nullptr;
					state = 0;
				}
			}
		}
	}
}
String SourceDef::GenerateCode(String name)
{
	String ret;
	ret = ret + "/*\n";
	ret = ret + "** Lua binding: " +name +"\n";
	ret = ret + "** Generated automatically by tosol++" + TOSOL_VERSION + " \n";
	ret = ret + "*/\n";
	if (namespaceName.length() > 0) {
		ret = ret + "using namespace " + namespaceName + ";\n";
	}
	
	for (auto c : classes) {
		int index = 0;
		for (auto m : c->ctors) {
			String getter = "tolua_" + name + "_" + c->name + "_" + std::to_string(index);
			
			ret = ret + "inline static " + c->name + " " + getter + m.overload.Signature() + "\n";
			ret = ret + "{\n";
			ret = ret + " return " + c->name + m.overload.Callers((int) m.overload.parameters.size()) + ";\n";
			ret = ret + "}\n";
			index++;
		}

		/*
		for (auto m : c->variables) {

			String getter = "tolua_" + name + "_" + c->name + "_" + m.varName + "_get";
			String setter = "tolua_" + name + "_" + c->name + "_" + m.varName + "_set";

			if (m.isStatic) {
				ret = ret + "inline static " +  m.varType + " " +  getter + "(void)" + "\n";
				ret = ret + "{\n return " + c->name + "::" + m.varName + "; \n}";;
				

				if (!m.isConst) {
					ret = ret + "inline static void " + setter + "("+m.varType+" value)" + "\n";
					ret = ret + "{\n " + c->name + "::" + m.varName + " = value; \n}";
				}
					
			}
	
		}
		*/
	}
	ret = ret + "\n";
	auto optUseSolLua = fields.find("s");
	if (optUseSolLua == fields.end()) {
		ret = ret + "int tolua_" + name + "_open(lua_State* L)\n";
	}
	else {
		ret = ret + "int tolua_" + name + "_open(sol::state & tosol_S)\n";
	}
	ret = ret + "{\n";
	if (optUseSolLua == fields.end()) {
		 ret = ret + " sol::state_view tosol_S(L);\n";
	}
	String module = "tosol_S";
	if (moduleName.length() > 0) {
		ret = ret + " sol::table _" + moduleName + " tosol_S.create_named_table(\"" + moduleName + "\"); \n";
		module = "_" +moduleName;
	}
	for (auto c : classes) {
		ret = ret + " "+ module +".new_usertype<" + ( c->name) + ">(\"" + (c->aliasName.length() ? c->aliasName : c->name) + "\",";
		ret = ret + " \n";
		int index = 0;
		/************************************************************************************
			INHERITS
		*/


		if (c->inherits.size() > 0) {
			ret = ret + " \tsol::base_classes, \n";
			for (auto ih : c->inherits)
			{
				if (index) {
					ret = ret + ",\n";
				}
				index++;
				ret = ret + " \t\tsol::bases<" + ih + ">";				
			}
			ret = ret + "()";
		}
		/************************************************************************************
			CTORS
		*/

		if (c->ctors.size() > 0) {
			ret = ret + " \tsol::constructors<\n";
			for (auto ctor : c->ctors)
			{
				if (index) {
					ret = ret + ",\n";
				}
				index++;
				ret = ret + " \t\t" + c->name + ctor.overload.Signature();				
			}
			ret = ret + ">()";
		}

		/************************************************************************************
			METHODS
		*/
		int numMethod = 0;
		for (auto m : c->methods) {
			
			String metaFunction = "\"" + m.methodName + "\"";
			if (m.methodName == "operator+") {
				metaFunction = "sol::meta_function::addition";
			}
			else if (m.methodName == "operator^") {
				metaFunction = "sol::meta_function::bitwise_xor";
			}
			else if (m.methodName == "operator%") {
				metaFunction = "sol::meta_function::modulo";
			}
			else if (m.methodName == "operator-") {
				if (m.overloads[0].isUnary) {
					metaFunction = "sol::meta_function::unary_minus";
				}
				else {
					metaFunction = "sol::meta_function::subtraction";
				}
			}
			else if (m.methodName == "operator*") {
				metaFunction = "sol::meta_function::multiplication";
			}
			else if (m.methodName == "operator/") {
				metaFunction = "sol::meta_function::division";
			}
			else if (m.methodName == "operator==") {
				metaFunction = "sol::meta_function::equal_to";
			}			
			else if (m.methodName == "operator<") {
				metaFunction = "sol::meta_function::less_than";
			}
			else if (m.methodName == "operator>") {
				metaFunction = "sol::meta_function::greater_than";
			}
			else if (m.methodName == "operator[]") {
				metaFunction = "sol::meta_function::index";
			}
			else if (m.methodName == "operator()") {
				metaFunction = "sol::meta_function::index";
			}
			// Not supported by lua
			if (
				m.methodName == "operator()")
			{
				fprintf(stdout, "Warning : not supported\n");
			}
			else
			if (m.methodName == "operator+=" ||
				m.methodName == "operator-=" ||
				m.methodName == "operator/=" ||
				m.methodName == "operator*=")
			{
				fprintf(stdout, "Warning : addition, substraction etc... operators are not supported by Lua.\n");
			}
			else
			{
				
				numMethod++;
				if (m.isOverload)  {

					if (index) {
						ret = ret + ",\n";
					}
					index++;

					String overloads = ""; // &" + c->name + "::" + m.methodName
					ret = ret + " \t" + metaFunction + ", sol::overload(\n";
					int overloadIndex = 0;
					for (auto r : m.overloads) {
						if (overloadIndex) {
							ret = ret + ",\n";
						}
					
						overloadIndex++;
						if (r.hasDefaultParameter) {
							int i = r.parameters.size() - r.GetNumExplicitParameters();
							String proto = PointerToSharedPtr(m.returnType) + r.Construct(i);
							if (r.isConst) {
								proto = proto + " const";
							}							
							ret = ret + "\t\tsol::resolve<" + proto + ">(&" + c->name + "::" + m.methodName + ")";
								
						}
						else
						{
							String proto = PointerToSharedPtr(m.returnType) + r.Construct((int) r.parameters.size());
							if (r.isConst) {
								proto = proto + " const";
							}
							ret = ret + "\t\tsol::resolve<" + proto + ">(&" + c->name + "::" + m.methodName + ")";
							
						}
					}
					ret = ret + "\n\t)";

					
				}
				else {
					if (true)
					{
						if (index) {
							ret = ret + ",\n";
						}
						index++;
						auto r = m.overloads[0];
						String proto = PointerToSharedPtr(m.returnType) + r.Construct((int)r.parameters.size());
						if (r.isConst) {
							proto = proto + " const";
						}
						ret = ret + " \t" + metaFunction+",";
						ret = ret + "sol::resolve<" + proto + ">(&" + c->name + "::" + m.methodName + ")";
					}
					else
					{
						ret = ret + " \t" + metaFunction + ", &" + c->name + "::" + m.methodName;
					}					
				}
			}
		}
	
	
		/************************************************************************************
			VARIABLES
		*/
			
		for (auto m : c->variables) {
			if (index) {
				ret = ret + ",\n";
			}
			index++;	

			/*
			String getter = "tolua_" + name + "_" + c->name + "_" + m.varName + "_get";
			String setter = "tolua_" + name + "_" + c->name + "_" + m.varName + "_set";
			*/
			String lambda_getter = "[]() { return " + c->name + "::" + m.varName + "; }";
			String lambda_setter = "[](" + m.varType + " value) { " + c->name + "::" + m.varName + " = value; }";

			if (m.isStatic) {

				if (m.isConst)
					ret = ret + " \t\"" + m.varName + "\", sol::property(" + lambda_getter + ")";
				else
					ret = ret + " \t\"" + m.varName + "\", sol::property(" + lambda_getter + ", " + lambda_setter + " )";
				

				/*
				if (m.isConst)
					ret = ret + " \t\"" + m.varName + "\", sol::property(&" + getter + ")";
				else
					ret = ret + " \t\"" + m.varName + "\", sol::property(&" + getter + ", &" + setter + " )";
				*/
			}
			else {
				ret = ret + " \t\"" + m.varName + "\", &" + c->name + "::" + m.varName;
			}
		}
		
			
		
		/* END */
		ret = ret + "\n);";
		
		ret = ret + "\n";
		ret = ret + "\n";

		/************************************************************************************
			DEFAULT CTORS
		*/

		if (c->ctors.size() > 0) {
			ret = ret + " tosol_S[\"" + (c->aliasName.length() ? c->aliasName : c->name) + "\"] =\n";
			ret = ret + "\tsol::overload(\n";
			index = 0;
			for (auto m : c->ctors) {

				String fun = "tolua_" + name + "_" + c->name + "_" + std::to_string(index);
				String proto = c->name + m.overload.Signature();
				if (index > 0) {
					ret = ret + ", \n";
				}
				ret = ret + "\t\tsol::resolve<" + proto + " >(&" + fun + ")";
				index++;
			}
		}
		if (c->ctors.size() > 0) {
			ret = ret + ");";
		}

		ret = ret + "\n";
	}
	ret = ret + " return 0;\n}\n";
	return ret;
}
static void version(void)
{
	fprintf(stderr, "%s (written by execom)\n", TOSOL_VERSION);
}
static void setfield(int table, char* f, char* v)
{
	fields[f] = v;
}
static void add_extra(char* value) {
}
static void help(void)
{
	fprintf(stdout, "\n"
		"usage: tosol++ [options] input_file\n"
		"\n"
		"Command line options are:\n"
		"  -v       : print version information.\n"
		"  -o  file : set output file; default is stdout.\n"
		// "  -H  file : create include file.\n"
		"  -n  name : set package name; default is input file root name.\n"
		"  -s       : Use sol::lua instead of Lua_State and sol::state_view.\n"
		"  -x       : Convert all pointers to std::shared_ptr\n"
		"  -h       : print this message.\n"
		"Should the input file be omitted, stdin is assumed;\n"
		"in that case, the package name must be explicitly set.\n\n"
	);
}
static void error(char* o)
{
	fprintf(stdout, "tosol: unknown option '%s'\n", o);
	help();
	exit(1);
}
int tosol(String filename)
{
	SourceDef sourceDef;
	FILE * f = fopen(filename.c_str(), "rt");
	if (!f) {
		fprintf(stdout, "Couldn't open %s", filename.c_str());
		return -1;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  //same as rewind(f);
	char *string = (char *)calloc(1, fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);
	sourceDef.Parse(string);
	String outputName = filename;
	auto o = fields.find("o");
	if (o == fields.end()) {
		outputName = "";
	}
	else {
		outputName = o->second;
	}
	String packageName;
	auto n = fields.find("n");
	if (n != fields.end()) {
		packageName = n->second;
	}
	else
	{
		auto pos = filename.rfind('.');
		filename.replace(pos, filename.length() - pos, "");
		pos = filename.rfind('\\');
		if (pos > 0) {
			packageName = filename.substr(pos+1);
		}
		else {
			packageName = filename;
		}
	}
	String ret = sourceDef.GenerateCode(packageName);
	if (outputName == "") {
		fprintf(stdout, ret.c_str());
	}
	else
	{
		f = fopen(outputName.c_str(), "wt");
		if (f != nullptr) {
			fwrite(ret.c_str(), ret.length() + 1, 1, f);
			fclose(f);
		}
	}
	return 0;
}
int main(int argc, char* argv[])
{
	int i;
	int t = 0;
	for (i = 1; i < argc; ++i)
	{
		if (*argv[i] == '-')
		{
			switch (argv[i][1])
			{
			case 'v': version(); return 0;
			case 'h': help(); return 0;
			case 'p': setfield(t, "p", ""); break;
			case 'P': setfield(t, "P", ""); break;
			case 'o': setfield(t, "o", argv[++i]); break;
			case 'n': setfield(t, "n", argv[++i]); break;
			case 'H': setfield(t, "H", argv[++i]); break;
			case 's': setfield(t, "s", ""); break;
			case '1': setfield(t, "1", ""); break;
			case 'L': setfield(t, "L", argv[++i]); break;
			case 'D': setfield(t, "D", ""); break;
			case 'W': setfield(t, "W", ""); break;
			case 'C': setfield(t, "C", ""); break;
			case 'E': add_extra(argv[++i]); break;
			case 't': setfield(t, "t", ""); break;
			case 'q': setfield(t, "q", ""); break;
			default: error(argv[i]); break;
			}
		}
		else
		{
			setfield(t, "f", argv[i]);
		}
	}
	if (argc == 1) {
		help();
	}
	else {
		tosol(argv[argc - 1]);
	}
	return 0;
}
