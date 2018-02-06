// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * [[[FILE NAME]]]
 *
 * [[[BREIF DESCRIPTION]]]
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include "tinydir/tinydir.h"
#include <set>

using std::string;

#define ST_UNUSED(X) (void)(X)

typedef uint8_t   u8;
typedef uint16_t  u16;
typedef uint32_t  u32;
typedef uint64_t  u64;
typedef int8_t    s8;
typedef int16_t   s16;
typedef int32_t   s32;
typedef int64_t   s64;
typedef float     f32;
typedef double    f64;

enum class TypeKind
{
  Void,
  POD,
  Struct,
  Function,
  ConstString,
  String,
  Enum
};

class TypeBase
{
  public:
    bool isPublic, isNative;
    virtual ~TypeBase() { }
    virtual std::string getTypeName() const = 0;
    virtual TypeKind    getType() const = 0;
};

typedef std::shared_ptr<TypeBase> Type;

class VoidType : public TypeBase
{
public:
  VoidType()  { isPublic = true; isNative = true; }
  ~VoidType() override {}
  std::string getTypeName() const override { return "void"; }
  TypeKind    getType() const override { return TypeKind::Void; }
};

class CStrType : public TypeBase
{
public:
  CStrType()  { isPublic = true; isNative = true; }
  ~CStrType() override {}
  std::string getTypeName() const override { return "stCStr"; }
  TypeKind    getType() const override { return TypeKind::ConstString; }
};

class StrType : public TypeBase
{
public:
  StrType()  { isPublic = true; isNative = true; }
  ~StrType() override {}
  std::string getTypeName() const override { return "stStr"; }
  TypeKind    getType() const override { return TypeKind::String; }
};

class U8Type : public TypeBase
{
public:
  U8Type()  { isPublic = true; isNative = true; }
  ~U8Type() override {}
  std::string getTypeName() const override { return "u8"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class U16Type : public TypeBase
{
public:
  U16Type()  { isPublic = true; isNative = true; }
  ~U16Type() override {}
  std::string getTypeName() const override { return "u16"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class U32Type : public TypeBase
{
public:
  U32Type()  { isPublic = true; isNative = true; }
  ~U32Type() override {}
  std::string getTypeName() const override { return "u32"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class U64Type : public TypeBase
{
public:
  U64Type()  { isPublic = true; isNative = true; }
  ~U64Type() override {}
  std::string getTypeName() const override { return "u64"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class S8Type : public TypeBase
{
public:
  S8Type()  { isPublic = true; isNative = true; }
  ~S8Type() override {}
  std::string getTypeName() const override { return "s8"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class S16Type : public TypeBase
{
public:
  S16Type()  { isPublic = true; isNative = true; }
  ~S16Type() override {}
  std::string getTypeName() const override { return "s16"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class S32Type : public TypeBase
{
public:
  S32Type()  { isPublic = true; isNative = true; }
  ~S32Type() override {}
  std::string getTypeName() const override { return "s32"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class S64Type : public TypeBase
{
public:
  S64Type()  { isPublic = true; isNative = true; }
  ~S64Type() override {}
  std::string getTypeName() const override { return "s64"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class F32Type : public TypeBase
{
public:
  F32Type()  { isPublic = true; isNative = true; }
  ~F32Type() override {}
  std::string getTypeName() const override { return "f32"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class F64Type : public TypeBase
{
public:
  F64Type()  { isPublic = true; isNative = true; }
  ~F64Type() override {}
  std::string getTypeName() const override { return "f64"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class BoolType : public TypeBase
{
public:
  BoolType()  { isPublic = true; isNative = false; }
  ~BoolType() override {}
  std::string getTypeName() const override { return "stBool"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class EnumValueType : public TypeBase
{
public:
  EnumValueType()  { isPublic = true; isNative = true; }
  ~EnumValueType() override {}
  std::string getTypeName() const override { return "stEnum32"; }
  TypeKind    getType() const override { return TypeKind::POD; }
};

class StructType : public TypeBase
{
public:
  
  std::string       name;
  std::vector<Type> types;

  StructType(const string& name_) : name(name_)
  {
    isPublic = false;
    isNative = false;
  }

  ~StructType() override {
    types.clear();
  }
  std::string getTypeName() const override { return name; }
  TypeKind    getType() const override { return TypeKind::Struct; }
};

class EnumType : public TypeBase
{
public:

  string                      named;
  std::multimap<int, string>  valueNames;

  EnumType(const string& name_) : named(name_)
  {
    isPublic = false;
    isNative = false;
  }

  ~EnumType() override {
  }

  std::string getTypeName() const override { return named; }
  TypeKind    getType() const override     { return TypeKind::Enum; }
};

class FunctionType : public TypeBase
{
  public:

    std::string       name;
    std::vector<Type> arguments;
    Type              response;

    FunctionType(const string& name_) : name(name_) 
    {
      isPublic = false;
      isNative = false;
    }

    ~FunctionType() override
    {
      arguments.clear();
    }

    std::string getTypeName() const override { return name; }
    TypeKind    getType() const override { return TypeKind::Struct; }
};

class ArgumentBase
{
  public:
    string name;
    Type   type;
    int    slot;
    bool   isPointer;
    ArgumentBase(const string& name_, Type type_, int slot_, bool isPointer_)
      : name(name_), type(type_), slot(slot_), isPointer(isPointer_)
      {
      }
    ~ArgumentBase() {}
};

typedef std::shared_ptr<ArgumentBase> Argument;

class Function : public FunctionType
{
public:

  Function(const std::string& name) : FunctionType(name) { isPublic = false; isNative = false; }
  ~Function() {}

  bool hasProperty(const std::string& str) const
  {
    return properties.find(str) != properties.end();
  }

  std::string getProperty(const std::string& str)
  {
    return properties[str];
  }

  string                      named;
  std::map<string, string>    properties;
  std::vector<Argument>       arguments;
  Argument                    response;
};



class Namespace
{
public:
  Namespace()
  {
    AddType(new VoidType());
    AddType(new CStrType());
    AddType(new StrType());
    AddType(new U8Type());
    AddType(new U16Type());
    AddType(new U32Type());
    AddType(new U64Type());
    AddType(new S8Type());
    AddType(new S16Type());
    AddType(new S32Type());
    AddType(new S64Type());
    AddType(new F32Type());
    AddType(new F64Type());
    AddType(new BoolType());
    AddType(new EnumValueType());
  }

  ~Namespace() {}

  bool GetType(const string& name_, Type& outType)
  {
    for(Type type : types)
    {
      if (type->getTypeName() == name_)
      {
        outType = type;
        return true;
      }
    }
    return false;
  }

  void AddType(TypeBase* type)
  {
    types.push_back(Type(type));
  }

  void AddFunction(Function* function)
  {
    functions.push_back(Type(function));
  }

  std::string        name;
  std::string        namePrefix, macroPrefix;
  std::vector<Type>  types;
  std::vector<Type>  functions;
};

// trim from start
static inline std::string &ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
    std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(),
    std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
  return ltrim(rtrim(s));
}
static inline bool startsWith(const std::string& input, const std::string& match)
{
  return input.size() >= match.size() && std::equal(match.begin(), match.end(), input.begin());
}

static inline size_t findFirstOf(const std::string& input, char k, size_t offset = 0)
{
  return input.find_first_of(k, offset);
}

static inline size_t findLastOf(const std::string& input, char k, size_t offset = std::string::npos)
{
  return input.find_last_of(k, offset);
}

static inline size_t skipWhiteSpace(const std::string& input, size_t it = 0)
{
  while(it < input.length() && std::isspace(input[it]))
    it++;
  return it;
}

static inline bool validToken(char k)
{
  return k == '_' || isalnum(k);
}

std::vector<std::string> explode(std::string const & s, char delim)
{
  std::vector<std::string> result;
  std::istringstream iss(s);

  for (std::string token; std::getline(iss, token, delim); )
  {
    result.push_back(std::move(token));
  }

  return result;
}

static inline size_t readToken(const std::string& input, size_t it, char* token, size_t tokenCapacity, size_t& tokenLength)
{
  memset(token, 0, tokenCapacity);
  tokenLength = 0;
  while(it < input.length() && tokenLength < tokenCapacity && validToken(input[it]))
  {
    token[tokenLength++] = input[it++];
  }
  return it;
}

 size_t readTypeToken(const std::string& input, size_t it, char* token, size_t tokenCapacity, size_t& tokenLength, bool* isPointer)
{
  memset(token, 0, tokenCapacity);
  tokenLength = 0;
  while(it < input.length() && tokenLength < tokenCapacity && validToken(input[it]))
  {
    token[tokenLength++] = input[it++];
  }

  if (tokenLength > 0)
  {
    it = skipWhiteSpace(input, it);
    (*isPointer) = (input[it] == '*');
    if ((*isPointer))
    {
      it++;
    }
  }

  return it;
}

static inline size_t readStringToken(const std::string& input, size_t it, char* token, size_t tokenCapacity, size_t& tokenLength)
{
  memset(token, 0, tokenCapacity);
  tokenLength = 0;
  while(it < input.length() && tokenLength < tokenCapacity)
  {
    char k = input[it++];
    if (k == '"')
    {
      //it++; // consume "
      break;
    }
    token[tokenLength++] = k;
  }
  return it;
}


static inline bool charMatch(const string& doc, size_t it, char k)
{
  return it < doc.length() && doc[it] == k;
}

static inline bool alphaMatch(const string& doc, size_t it)
{
  return it < doc.length() && isalnum(doc[it]);
}

static inline void makeLowerCase(char* text, size_t length)
{
  for(u32 i=0;i < length;i++)
    text[i] = (char) tolower(text[i]);
}

static inline void makeUpperCase(char* text, size_t length)
{
  for(u32 i=0;i < length;i++)
    text[i] = (char) toupper(text[i]);
}

static inline std::string makeLowerCaseStr(const std::string& str)
{
  std::string s = str;
  for (auto & c: s)
  {
    c = (char) tolower(c);
  }
  return s;
}

static inline std::string makeLowerCaseStr(const std::string& str, std::stringstream& ss)
{
  std::string s = str;
  for (auto & c: s)
  {
    ss << (char) tolower(c);
  }
  return s;
}

static inline std::string makeUpperCaseStr(const std::string& str)
{
  std::string s = str;
  for (auto & c: s)
  {
    c = (char) toupper(c);
  }
  return s;
}
static inline bool expectToken(const string& doc, const char* match, size_t& it)
{
  size_t it0 = it;
  size_t matchLength = strlen(match);
  for(size_t i=0;i < matchLength;i++)
  {
    if (it0 + i >= doc.length())
      return false;

    if (doc[it0 + i] != match[i])
    {
      return false;
    }
  }

  it += matchLength;
  return true;
}

static inline std::string strConcat(const string& a, const string& b)
{
  std::stringstream ss;
  ss << a << b;
  return ss.str();
}

void printError(const char* text, const string& doc)
{
  printf("Error: %s\n%s\n", text, doc.c_str());
  printf("** Fatal Exit.\n");
  exit(0);
}

void printWarning(const char* text, const string& doc)
{
  ST_UNUSED(text);
  ST_UNUSED(doc);
  //printf("Warning: %s\n%s\n", text, doc.c_str());
}

void readDocProperties(const string& doc, std::map<string, string>& args, const string& macroPrefix)
{
  ST_UNUSED(macroPrefix);

  size_t begin = findFirstOf(doc, '(');
  size_t end   = findLastOf(doc, ')');

  size_t it = begin + 1;

  char name[64];
  size_t nameLength;
  char value[512];
  size_t valueLength;

  while(it != end)
  {
    name[0] = 0;
    value[0] = 0;

    it = skipWhiteSpace(doc, it);
    if (it == end)
      break;

    it = readToken(doc, it, name, 64, nameLength);
    it = skipWhiteSpace(doc, it);

    if (charMatch(doc, it, ',') || charMatch(doc, it, ')'))
    {
      value[0] = '\0';
    }
    else if (charMatch(doc, it, '='))
    {
      it++;
      it = skipWhiteSpace(doc, it);

      if (alphaMatch(doc, it))
      {
        it = readToken(doc, it, value, 512, valueLength);
      }
      else if (charMatch(doc, it, '\"'))
      {
        it++;
        it = skipWhiteSpace(doc, it);
        it = readStringToken(doc, it, value, 512, valueLength);
      }
      else
      {
        printError("Unknown character for value", doc);
      }
    }
    else
    {
      printError("Syntax Error", doc);
    }

    makeLowerCase(name, nameLength);
    args.insert(std::make_pair(std::string(name), std::string(value)));

    it = skipWhiteSpace(doc, it);

    if (charMatch(doc, it, ')'))
    {
      if (it < end)
        printError("Extra )", doc);
      break;
    }

    if (charMatch(doc, it, ','))
    {
      it++;
      continue;
    }

    printError("Unknown character", doc);
  }
}

void readFunction(Namespace& ns, bool isPrivate, const std::string& doc, const std::string& sig, const string& macroPrefix)
{
  ST_UNUSED(isPrivate);
  ST_UNUSED(sig);
  Function* function = new Function("Unknown");
  function->isPublic = !isPrivate;
  readDocProperties(doc, function->properties, macroPrefix);
  
  // <ST_PUBLIC|if !isPrivate> Type name(Type Arg<,Type Arg ...>);
  size_t it = skipWhiteSpace(sig, 0);
  if (!isPrivate)
  {
    if (expectToken(sig, strConcat(macroPrefix, "PUBLIC").c_str(), it) == false)
    {
      printError("Missing PUBLIC on a PUBLIC_FUNCTION", sig);
    }
    it = skipWhiteSpace(sig, it);
  }

  bool typeIsPointer = false;
  char typeName[64]; size_t typeNameLength = 0;
  char argName[64];  size_t argNameLength  = 0;

  it = readTypeToken(sig, it, typeName, 64, typeNameLength, &typeIsPointer);
  
  // printf("> %s\n", typeName);
  Type type;
  if (!ns.GetType(typeName, type))
  {
    printError("Unknown Return Type", sig);
  }
  
  ArgumentBase* response = new ArgumentBase("response", type, 0, typeIsPointer);
  function->response = Argument(response);

  it = skipWhiteSpace(sig, it);
  it = readToken(sig, it, typeName, 64, typeNameLength);

  function->named = std::string(typeName);

  // printf(": %s\n", typeName);

  it = skipWhiteSpace(sig, it);
  if (charMatch(sig, it, '(') == false)
  {
    printError("Unknown token after function name, or missing (", sig);
  }

  it++;
  u32 slot = 0;

  while(it < sig.length())
  {
    it = skipWhiteSpace(sig, it);

    if (charMatch(sig, it, ')'))
      break;

    bool typeIsPointer2;

    it = readTypeToken(sig, it, typeName, 64, typeNameLength, &typeIsPointer);
    if (typeNameLength == 0)
    {
      printError("Argument Type Syntax Error", sig);
    }

    it = skipWhiteSpace(sig, it);

    it = readTypeToken(sig, it, argName, 64, argNameLength, &typeIsPointer2);
 
    if (ns.GetType(typeName, type) == false)
    {
      if (!isPrivate)
      {
        printf(">> %s\n", typeName);
        printError("Unknown type on Public Function", sig);
      }
    }

    ArgumentBase* argument = new ArgumentBase(argName, type, slot, typeIsPointer);
    function->arguments.push_back(Argument(argument));

    it = skipWhiteSpace(sig, it);

    if (charMatch(sig, it, ')'))
    {
      if (!(it + 1< sig.length()))
      {
        std::cout << argName << std::endl << type << std::endl << slot << std::endl<< typeIsPointer << std::endl;
        printError("Extra )", sig);
      }
      break;
    }

    if (charMatch(sig, it, ','))
    {
      it++;
      continue;
    }

    printError("Unknown character", sig);

  }

  ns.AddFunction(function);
}

void readEnum(Namespace& ns, bool isPrivate, const std::string& doc, const std::string& sig, const std::string& macroPrefix)
{
  ST_UNUSED(isPrivate);
  ST_UNUSED(sig);

  std::map<string, string> properties;
  readDocProperties(doc, properties, macroPrefix);

  if (properties.find("for") == properties.end())
  {
    printError("Missing 'for' on Enum", sig);
  }

  std::string forV = properties.find("for")->second;

  Type enumTV;
  EnumType* enumV;
  
  if (ns.GetType(forV, enumTV))
  {
    enumV = static_cast<EnumType*>(enumTV.get());
  }
  else
  {
    enumV = new EnumType(forV);
    enumV->isPublic = !isPrivate;
    ns.AddType(enumV);
  }

  if (properties.find("named") == properties.end())
  {
    printError("Missing 'Named' on Enum", sig);
  }

  std::string named = properties.find("named")->second;

  if (properties.find("value") == properties.end())
  {
    printError("Missing 'Named' on Enum", sig);
  }

  std::string value = properties.find("value")->second;

  enumV->valueNames.insert(std::make_pair(atoi(value.c_str()), named));
}

enum AutoDocType
{
  None,
  PublicFunction,
  PrivateFunction,
  PublicStruct,
  PrivateStruct,
  PublicEnum,
  PrivateEnum,
  Initializer,
  Shutdown
};

std::string exporterPrefix = "STEXP_";

void processFile(Namespace& ns, const std::string& path, const char* prefix)
{

  std::string macroPrefix = strConcat(prefix, "_");

  std::ifstream infile(path);
  
  std::string docLine, signatureLine;
  while (std::getline(infile, docLine))
  {
    if (docLine.empty())
      continue;
    trim(docLine);

    if (docLine.length() < 3)
      continue;

    if (startsWith(docLine, macroPrefix) == false)
    {
      if (startsWith(docLine, exporterPrefix) == false)
      { 
        continue;
      }

      macroPrefix = exporterPrefix;
    }

    AutoDocType docType = AutoDocType::None;

    if (startsWith(docLine, strConcat(macroPrefix, "PRIVATE_FUNCTION")))
    {
      docType = AutoDocType::PrivateFunction;
    }
    else if (startsWith(docLine, strConcat(macroPrefix, "PUBLIC_FUNCTION")))
    {
      docType = AutoDocType::PublicFunction;
    }
    else if (startsWith(docLine, strConcat(macroPrefix, "PRIVATE_ENUM")))
    {
      docType = AutoDocType::PrivateEnum;
    }
    else if (startsWith(docLine, strConcat(macroPrefix, "PUBLIC_ENUM")))
    {
      docType = AutoDocType::PublicEnum;
    }

    bool isPrivate = true;

    if (docType == AutoDocType::None)
      continue;

    std::getline(infile, signatureLine);

    switch(docType)
    {
      case AutoDocType::PublicFunction:
        isPrivate = false;
      case AutoDocType::PrivateFunction:
      {
        readFunction(ns, isPrivate, docLine, signatureLine, macroPrefix);
      }
      break;
      case AutoDocType::PublicStruct:
        isPrivate = false;
      case AutoDocType::PrivateStruct:
      {
        // TODO
      }
      break;
      case AutoDocType::PublicEnum:
      isPrivate = false;
      case AutoDocType::PrivateEnum:
      {
        readEnum(ns, isPrivate, docLine, signatureLine, macroPrefix);
      }
      break;
      case AutoDocType::Initializer:
      break;
      case AutoDocType::Shutdown:
      break;
    }
  }

}

void WriteFormatCSharpName(const std::string& name, std::stringstream& ss, const std::string& prefix)
{
  if (startsWith(name, prefix))
  {
    ss << name.substr(prefix.length());
  }
  else
  {
    ss << name;
  }
}

void WriteCSharpTypeName(Type type, bool isPointer, std::stringstream& ss)
{
  if (type->getType() == TypeKind::ConstString)
  {
    ss << "StringBuilder";
  }
  else if (type->getType() == TypeKind::String)
  {
    ss << "StringBuilder";
  }
  else if (isPointer)
  {
    ss << "IntPtr";
  }
  else if (type->getType() == TypeKind::POD)
  {
    if (type->getTypeName() == "u8")
      ss << "byte";
    else if (type->getTypeName() == "u16")
      ss << "ushort";
    else if (type->getTypeName() == "u32")
      ss << "uint";
    else if (type->getTypeName() == "u64")
      ss << "ulong";
    else if (type->getTypeName() == "s8")
      ss << "sbyte";
    else if (type->getTypeName() == "s16")
      ss << "short";
    else if (type->getTypeName() == "s32")
      ss << "int";
    else if (type->getTypeName() == "s64")
      ss << "long";
    else if (type->getTypeName() == "f32")
      ss << "float";
    else if (type->getTypeName() == "f64")
      ss << "double";
    else if (type->getTypeName() == "stBool")
      ss << "bool";
    else if (type->getTypeName() == "stByte")
      ss << "byte";
    else if (type->getTypeName() == "stEnum32")
      ss << "int";
    else
      ss << "ERROR";
  }
  else if (type->getType() == TypeKind::Void)
  {
    ss << "void";
  }
  else if (type->getType() == TypeKind::Struct)
  {
    ss << type->getTypeName();
  }
  else if(type->getType() == TypeKind::Function)
  {
    ss << type->getTypeName();
  }

}

void WriteCSharpEnum(const std::string& s, std::stringstream& ss)
{
  for(u32 i=0;i < s.length();i++)
  {
    char k = s[i];
    if (k != ' ')
      ss << k;
  }
}

void WriteCsEnumStyleClass(EnumType* env, std::stringstream& ss)
{
  ss << "\tpublic static class " << env->named << std::endl;
  ss << "\t{" << std::endl;
  for(auto nv : env->valueNames)
  {
    ss << "\t\tpublic const int ";
    WriteCSharpEnum(nv.second, ss);
    ss << " = (" << nv.first << ");" << std::endl;
  }
  ss << "\t}" << std::endl;
  ss << std::endl;
}

void WriteCsCategoryStyleClass(const Namespace& ns, const string& category, std::stringstream& ss)
{
  ss << "\tpublic static class " << category << std::endl;
  ss << "\t{" << std::endl;
  for(auto fnt : ns.functions)
  {
    Function* fn = static_cast<Function*>(fnt.get());

    if (fn->isPublic == false)
      continue;


    if (!(fn->hasProperty("category") && fn->getProperty("category") == category))
      continue;

    ss << std::endl;
    ss << "\t\t#if UNITY_EDITOR" << std::endl;
    ss << "\t\t[DllImport(\"" << ns.name << "\", CallingConvention = CallingConvention.Cdecl, EntryPoint = \"";
    ss << fn->named;
    ss << "\"), SuppressUnmanagedCodeSecurity]";
    ss << std::endl;

    ss << "\t\tpublic static extern ";

    WriteCSharpTypeName(fn->response->type, fn->response->isPointer, ss);

    ss << ' ';

    WriteFormatCSharpName(fn->named, ss, ns.namePrefix);

    ss << "(";

    for(u32 i=0;i < fn->arguments.size();i++)
    {
      Argument arg = fn->arguments[i];

      if (i > 0)
        ss << ", ";

      WriteCSharpTypeName(arg->type, arg->isPointer, ss);

      ss << ' ';
      ss << arg->name;
    }

    ss << ");" << std::endl;
    ss << "\t\t#else" << std::endl;
    ss << "\t\tpublic static ";

    WriteCSharpTypeName(fn->response->type, fn->response->isPointer, ss);

    ss << ' ';

    WriteFormatCSharpName(fn->named, ss, ns.namePrefix);
    ss << "(";

    for(u32 i=0;i < fn->arguments.size();i++)
    {
      Argument arg = fn->arguments[i];

      if (i > 0)
        ss << ", ";

      WriteCSharpTypeName(arg->type, arg->isPointer, ss);

      ss << ' ';
      ss << arg->name;
    }

    ss << ") {";
    if (fn->response->type->getType() != TypeKind::Void)
    {
      ss << " return default(";
      WriteCSharpTypeName(fn->response->type, fn->response->isPointer, ss);
      ss << "); ";
    }
    ss << "}" << std::endl;
    ss << "\t\t#endif" << std::endl;
  }

  ss << std::endl;
  ss << "\t}" << std::endl;

}

void WritePublicHeaderAsCSharp(const Namespace& ns, const char* outputName)
{
  std::stringstream ss;

  ss << "using System;"  << std::endl;
  ss << "using System.Runtime.CompilerServices;" << std::endl;
  ss << "using System.Runtime.InteropServices;" << std::endl;
  ss << "using System.Security;"  << std::endl;
  ss << "using System.Text;"  << std::endl << std::endl;
  
  ss << "namespace " << ns.name << std::endl;

  ss << "{" << std::endl;

  // Enums
  for(auto en : ns.types)
  {
    if (en->getType() != TypeKind::Enum)
      continue;
    WriteCsEnumStyleClass(static_cast<EnumType*>(en.get()), ss);
  }

  std::set<std::string> categories;
  for(auto fnt : ns.functions)
  {
    Function* fn = static_cast<Function*>(fnt.get());

    if (fn->isPublic == false)
      continue;


    if (!(fn->hasProperty("category")))
      continue;

    categories.insert(fn->getProperty("category"));
  }

  // Object style Namespaces
  for(auto cat : categories)
  {
    WriteCsCategoryStyleClass(ns, cat, ss);
  }
  
  ss << "}" << std::endl << std::endl;

  std::fstream file;
  file.open(strConcat(outputName, ".cs"), std::ios_base::out);
  std::string outStr = ss.str();
  file << outStr;
  file.close();
}

void WriteCEnum(const std::string& s, std::stringstream& ss)
{
  for(u32 i=0;i < s.length();i++)
  {
    char k = s[i];
    if (k == ' ')
      ss << '_';
    else
      ss << (char) toupper(k);
  }
}

void WriteCTypeName(Type type, bool isPointer, std::stringstream& ss)
{
  if (type->getType() == TypeKind::ConstString)
  {
    ss << "const char*";
    return;
  }
  else if (type->getType() == TypeKind::String)
  {
    ss << "char*";
    return;
  }
  else if (type->getType() == TypeKind::POD)
  {
    if (type->getTypeName() == "u8")
      ss << "uint8_t";
    else if (type->getTypeName() == "u16")
      ss << "uint16_t";
    else if (type->getTypeName() == "u32")
      ss << "uint32_t";
    else if (type->getTypeName() == "u64")
      ss << "uint64_t";
    else if (type->getTypeName() == "s8")
      ss << "int8_t";
    else if (type->getTypeName() == "s16")
      ss << "int16_t";
    else if (type->getTypeName() == "s32")
      ss << "int32_t";
    else if (type->getTypeName() == "s64")
      ss << "int64_t";
    else if (type->getTypeName() == "f32")
      ss << "float";
    else if (type->getTypeName() == "f64")
      ss << "double";
    else if (type->getTypeName() == "stBool")
      ss << "bool";
    else if (type->getTypeName() == "stByte")
      ss << "uint8_t";
    else if (type->getTypeName() == "stEnum32")
      ss << "int";
    else
      ss << "ERROR";
  }
  else if (type->getType() == TypeKind::Void)
  {
    ss << "void";
  }
  else if (type->getType() == TypeKind::Struct)
  {
    ss << type->getTypeName();
  }
  else if(type->getType() == TypeKind::Function)
  {
    ss << type->getTypeName();
  }

  if (isPointer)
    ss << '*';
}

void WritePublicHeaderAsC(const Namespace& ns, bool isStatic, const char* path)
{
  std::stringstream ss;

  std::string headerName =  makeUpperCaseStr(ns.name);

  ss << "#ifndef " << headerName << "_H" << std::endl;
  ss << "#define " << headerName << "_H" << std::endl << std::endl;

  ss << "#include <stdint.h>"  << std::endl;
  ss << "#include <stdbool.h>" << std::endl << std::endl;
  ss << "#ifdef __cplusplus"  << std::endl;
  ss << "  extern \"C\" {"  << std::endl;
  ss << "#endif" << std::endl << std::endl;

  if (isStatic)
  {
    ss << "  #define " << ns.macroPrefix << "_PUBLIC" << std::endl;
  }
  else
  {
    ss << "#if defined(_WIN32) || defined(_WIN64)" << std::endl;
    ss << "  #define " << ns.macroPrefix << "_PUBLIC  __declspec(dllimport)" << std::endl;
    ss << "#else" << std::endl;
    ss << "  #define " << ns.macroPrefix << "_PUBLIC  extern" << std::endl;
    ss << "#endif" << std::endl << std::endl;
  }

  // Enums
  for(auto en : ns.types)
  {
    if (en->getType() != TypeKind::Enum)
      continue;
    
    EnumType* env = static_cast<EnumType*>(en.get());

    for(auto nv : env->valueNames)
    {
      ss << "#define " << ns.macroPrefix << "_";
      WriteCEnum(env->named, ss);
      ss << "_";
      WriteCEnum(nv.second, ss);
      ss << " (" << nv.first << ")" << std::endl;
    }
    ss << std::endl;
  }

  // Functions
  for(auto fnt : ns.functions)
  {
    Function* fn = static_cast<Function*>(fnt.get());

    if (fn->isPublic == false)
      continue;

    ss << std::endl;
    ss << "/*" << std::endl;
    ss << " * Function: " << fn->named << std::endl;

    size_t l = fn->named.length() + 10;
    ss << " * ";
    for(size_t i=0;i < l;i++)
    {
      ss << '-';
    }
    ss << std::endl;

    if (fn->hasProperty("text"))
    {
      std::string text = fn->getProperty("text");
     
      auto lines = explode(text, '\n');

      ss << " *  " << std::endl;

      for(auto line : lines)
      {
        ss << " *  " << line << std::endl;
      }

      ss << " *  " << std::endl;
    }

    if (fn->arguments.size() > 0)
    {
      ss << " *  Parameters:" << std::endl;
    }

    for(u32 i=0;i < fn->arguments.size();i++)
    {
      Argument arg = fn->arguments[i];
      
      std::string argText;

      std::stringstream argPropTextName;
      argPropTextName << "_";
      makeLowerCaseStr(arg->name, argPropTextName);

      if (fn->hasProperty(argPropTextName.str()))
      {
        argText = fn->getProperty(argPropTextName.str());
      }

      ss << " *   " << (i+1) << ". ";

      if (startsWith(arg->name, "out_"))
      {
        ss << arg->name.substr(4);
        ss << " (Output)";
      }
      else
      {
        ss << arg->name;
      }
      
      if (argText.length() > 0)
      {
        ss << ": " << argText;
      }

      ss << std::endl;
    }

    if (fn->response->type->getType() != TypeKind::Void)
    {
      if (fn->hasProperty("_return"))
      {
        ss << " *  Return: " << std::endl;
        ss << " *   " << fn->getProperty("_return");
        ss << std::endl;
      }
    }
    ss << " */" << std::endl;




    ss << ns.macroPrefix << "_PUBLIC" << ' ';

    WriteCTypeName(fn->response->type, fn->response->isPointer, ss);

    ss << ' ';

    ss << fn->named;
    ss << "(";

    for(u32 i=0;i < fn->arguments.size();i++)
    {
      Argument arg = fn->arguments[i];

      if (i > 0)
      {
        ss << ", ";
      }

      WriteCTypeName(arg->type, arg->isPointer, ss);


      ss << ' ';

      ss << arg->name;

    }

    ss << ");";
    ss << std::endl;
  }



  ss << "#ifdef __cplusplus" << std::endl;
  ss << "}" << std::endl;
  ss << "#endif" << std::endl;


  ss << std::endl;
  ss << "#endif" << std::endl;

  // std::cout << ss.str() << std::endl;
  
  std::fstream file;
  file.open(path, std::ios_base::out);
  std::string outStr = ss.str();
  file << outStr;
  file.close();

}

void processLibrary(const char* dirPath, const char* output, const char* namePrefix, const char* macroPrefix, bool canStatic, bool canExp)
{
  Namespace ns;
  ns.name = output;
  ns.namePrefix = namePrefix;
  ns.macroPrefix = macroPrefix;

  tinydir_dir dir;
  tinydir_open(&dir, dirPath);

  while (dir.has_next)
  {
    tinydir_file file;
    tinydir_readfile(&dir, &file);

    std::string fileName(file.name);

    if (startsWith(fileName, namePrefix) || (canExp && ns.namePrefix != "st" && startsWith(fileName, "exp")))
    {
      processFile(ns, file.path, macroPrefix);
    }

    tinydir_next(&dir);
  }

  tinydir_close(&dir);

  WritePublicHeaderAsC(ns, false, strConcat(output, ".h").c_str());
  if (canStatic)
  {
    WritePublicHeaderAsC(ns, true,  strConcat(output, "Static.h").c_str());
  }

  WritePublicHeaderAsCSharp(ns, output);
}

int main(s32 argc, char* argv[])
{
  
  if (argc < 2)
  {
    std::cout << "Missing path" << std::endl;
    return 0;
  }

  processLibrary(argv[1], "SceneTrack", "st", "ST", true, false);
  processLibrary(argv[1], "SceneTrackFbx", "fbx", "STFBX", false, true);
  processLibrary(argv[1], "SceneTrackMidi", "midi", "STMIDI", false, true);
  processLibrary(argv[1], "SceneTrackVideo", "video", "STVIDEO", false, false);

}
