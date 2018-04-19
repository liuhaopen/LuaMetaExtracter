#include "LuaMetaExtracter.h"

/*
LuaMetaExtracter类说明
功能：提取lua代码的元信息并转化为json格式，目前只支持函数定义和函数调用(提取出调用的函数名，参数列表，返回值列表等信息)，如传入lua代码：
function func1()
	local result_val = self:func2("arge")
end
将生成json代码：
{
  "func_name": "func1",
  "codes": [
	  {
		"type": "call_func",
		"arges": [
			"arge",
		],
		"rets": [
			"result_val",
		],
		  "call_name": "call_func1",
		  "call_full_name": "self:call_func1"
	  }
  ],
}
func_name就是定义的函数名，codes是该函数体里的表达式列表。
为了简洁起见上面只是一部分json代码而已，因为lua支持嵌套的函数定义，如：
function func1()
	local func2 = function (  )
		self:call_func3()
	end
end
所以每个函数体内又可以包含函数体，把整个lua文件当作一个函数（其实lua内部实现解析一个lua文件时也是把它当成一个闭包的）,那上面代码的json结构大概就是：
{
	"func_name": "outside",
	"codes": [函数调用信息列表，最外层没调用函数所以这里为空],
	"funcs": [
		{
			"func_name": "func1",
			"codes": [func1函数体里没调用函数，所以这里也为空]
			"funcs": [
				{
					"func_name": "func2",
					"funcs": [终于没定义新函数了，所以这里为空],
					"codes": [
						{
							"type": "call_func",
							"arges": [],
							"rets": [],
							"call_name": "call_func3",
							"call_full_name": "self:call_func3"
						}
					]
				}
			],
		}
	]
}
一个函数定义的信息有三字段：函数名(func_name)，函数体里执行的表达式列表(codes)，函数体里定义的函数列表(funcs)
表达式信息有五字段：类型(type)，调用的函数名(call_name)，调用的函数全名(call_full_name)，参数列表(arges)，返回值列表(rets)
*/
int main()
{
	LuaMetaExtracter extracter;

	//提取lua代码中的函数调用信息，具体见test_simple.lua
	rapidjson::Document* doc = extracter.ParseFile("./test/test_simple.lua");
	LuaMetaExtracter::PrintDoc(doc);
	LuaMetaExtracter::SaveDocToFile(doc, "./test/extract_result_simple.json");
	
	//测试多层函数嵌套定义的信息提取
	doc = extracter.ParseFile("./test/test_nested_func.lua");
	LuaMetaExtracter::PrintDoc(doc);
	LuaMetaExtracter::SaveDocToFile(doc, "./test/extract_result_nested_func.json");

	//完整测试
	doc = extracter.ParseFile("./test/test_all.lua");
	LuaMetaExtracter::PrintDoc(doc);
	LuaMetaExtracter::SaveDocToFile(doc, "./test/extract_result_all.json");

	getchar();
    return 0;
}

