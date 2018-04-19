#ifndef __LUA_PARSER_EXPORT_H__
#define __LUA_PARSER_EXPORT_H__

//导出给c++使用的接口，c使用的在cpp文件导出就行
extern void* lparser_get_document();
extern int lparser_init();
extern int lparser_print_all();
//添加关注的函数名，不添加的话就不会生成在json里的
extern int lparser_add_concern_func(const char* func_name);

#endif