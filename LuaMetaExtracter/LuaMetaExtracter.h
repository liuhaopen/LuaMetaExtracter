#ifndef __LUA_PARSER_H__
#define __LUA_PARSER_H__

#include "rapidjson/document.h"
#include <string>
using namespace std;

class LuaMetaExtracter
{
public:
	LuaMetaExtracter();
	rapidjson::Document* ParseFile(const char* file_path);
	rapidjson::Document* ParseString(const char* content);

	static void PrintDoc(rapidjson::Document* doc);
	static void SaveDocToFile(rapidjson::Document* doc, const char* file_path);

	//添加关注的函数名，默认是全部提取的，调用此函数后将只提取关注的函数名的调用
	void AddConcernCallFunc(const char* func_name);
};

#endif