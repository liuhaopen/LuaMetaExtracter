extern "C"
{
#include "lua.h"
#include <lualib.h>  
#include <lauxlib.h>  
#include "lvm.h"
}
#include "LuaMetaExtracter.h"
#include "LuaParserExport.h"
#include <rapidjson/prettywriter.h>
#include "rapidjson/stringbuffer.h"
#include <rapidjson/writer.h>
#include <iostream>  
#include <fstream> 

LuaMetaExtracter::LuaMetaExtracter()
{
}

rapidjson::Document * LuaMetaExtracter::ParseFile(const char* file_path)
{
	lua_State *L = lua_open();
	luaL_openlibs(L);
	lparser_init();
	luaL_loadfile(L, file_path);
	lua_close(L);
	return static_cast<rapidjson::Document*>(lparser_get_document());
}

rapidjson::Document * LuaMetaExtracter::ParseString(const char* content)
{
	lua_State *L = lua_open();
	luaL_openlibs(L);
	lparser_init();
	luaL_loadstring(L, content);
	lua_close(L);
	return static_cast<rapidjson::Document*>(lparser_get_document());
}

void LuaMetaExtracter::PrintDoc(rapidjson::Document * doc)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc->Accept(writer);
	printf("LuaMetaExtracter::PrintDoc : %s\n", buffer.GetString());
}

void LuaMetaExtracter::SaveDocToFile(rapidjson::Document * doc, const char * file_path)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	doc->Accept(writer);
	std::ofstream out(file_path);
	if (out.is_open())
	{
		out << buffer.GetString();
		out.close();
	}
}

void LuaMetaExtracter::AddConcernCallFunc(const char * func_name)
{
	lparser_add_concern_func(func_name);
}
