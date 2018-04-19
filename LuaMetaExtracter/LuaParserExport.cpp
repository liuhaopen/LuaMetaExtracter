#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <iostream>  
#include <fstream> 
#include "LuaParserExport.h"
#include <unordered_map>
#include <string>
#include <stack>

extern "C" int lparser_add_member(const char * name, const char* content, void* value);
extern "C" int lparser_start_func();
extern "C" int lparser_end_func();
extern "C" void* lparser_add_code(const char* type);
extern "C" int lparser_is_concern(const char* func_name);
extern "C" void* lparser_add_array_item(void* value, const char * array_name, const char* content);

static rapidjson::Document document;
static rapidjson::Value* cur_val = NULL;
static std::unordered_map<std::string, bool> concern_func_name;
static std::stack<rapidjson::Value*> value_stack;

void create_func_obj(rapidjson::Value& obj_value)
{
	rapidjson::Value funcs;
	funcs.SetArray();
	obj_value.AddMember("funcs", funcs, document.GetAllocator());
	rapidjson::Value codes;
	codes.SetArray();
	obj_value.AddMember("codes", codes, document.GetAllocator());
}

void * lparser_get_document()
{
	return &document;
}

int lparser_init()
{
	//printf("lparser_init\n");
	document.SetObject();
	document.RemoveAllMembers();
	document.AddMember("func_name", "outside", document.GetAllocator());
	create_func_obj(document);
	cur_val = &document;
	return 0;
}

int lparser_add_member(const char * name, const char * content, void* value)
{
	//printf("lparser_add_member() name:%s content:%s value:%d\n", name, content, value);
	if (value == NULL)
		value = cur_val;
	if (value != NULL)
	{
		rapidjson::Value key(name, document.GetAllocator());
		rapidjson::Value val(content, document.GetAllocator());
		((rapidjson::Value*)value)->AddMember(key, val, document.GetAllocator());
	}
	return 0;
}

int lparser_start_func()
{
	if (cur_val != NULL && cur_val->HasMember("funcs"))
	{
		rapidjson::Value& funcs = (*cur_val)["funcs"];
		rapidjson::Value func_obj;
		func_obj.SetObject();
		create_func_obj(func_obj);
		funcs.PushBack(func_obj, document.GetAllocator());
		value_stack.push(cur_val);
		cur_val = &funcs[funcs.Size() - 1];
	}
	return 0;
}

int lparser_end_func()
{
	cur_val = value_stack.top();
	value_stack.pop();
	return 0;
}

void* lparser_add_code(const char * type)
{
	if (cur_val != NULL && cur_val->HasMember("codes"))
	{
		rapidjson::Value new_code_val;
		new_code_val.SetObject();
		rapidjson::Value val(type, document.GetAllocator());
		new_code_val.AddMember("type", val, document.GetAllocator());
		rapidjson::Value arges;
		arges.SetArray();
		new_code_val.AddMember("arges", arges, document.GetAllocator());
		rapidjson::Value rets;
		rets.SetArray();
		new_code_val.AddMember("rets", rets, document.GetAllocator());
		rapidjson::Value& codes = (*cur_val)["codes"];
		codes.PushBack(new_code_val, document.GetAllocator());

		return &codes[codes.Size() - 1];
	}
	return NULL;
}

int lparser_is_concern(const char * func_name)
{
	if (concern_func_name.empty())
		return 1;
	return concern_func_name.find(func_name) != concern_func_name.end() ? 1 : 0;
}

void * lparser_add_array_item(void * value, const char * array_name, const char * content)
{
	rapidjson::Value* val = static_cast<rapidjson::Value*>(value);
	if (val != NULL && val->HasMember(array_name))
	{
		rapidjson::Value new_item;
		char buffer[255];
		int len = sprintf(buffer, "%s", content); // 动态创建的字符串。
		new_item.SetString(buffer, len, document.GetAllocator());
		memset(buffer, 0, sizeof(buffer));
		//new_item.SetString(StringRef(content));
		(*val)[array_name].PushBack(new_item, document.GetAllocator());
	}
	return NULL;
}

int lparser_print_all()
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);
	printf("lparser_print_all : \n%s\n", buffer.GetString());
	/*std::ofstream out("./test/out.json");
	if (out.is_open())
	{
		out << buffer.GetString();
		out.close();
	}*/
	return 0;
}

int lparser_add_concern_func(const char * func_name)
{
	concern_func_name[func_name] = true;
	return 0;
}
