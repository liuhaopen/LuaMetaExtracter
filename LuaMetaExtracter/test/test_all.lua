--测试注释
--[[
测试多行注释
测试多行注释
--]]
require("game.guild.play.GuildPlayModel")--测试require调用

Test = Test or BaseClass(BaseTest)
Test.IsDebug = true

function Test:__init()
	self.model = TestModel:GetInstance()
	self:AddEvents()
	self:RegisterAllProtocal()
	
end

function Test:RegisterAllProtocal(  )
	--测试函数调用 
    self:RegisterProtocal(40100, "handle40100")--活动时间
end

function Test:AddEvents()
	--测试local function定义
	local function ON_REQ_INFO(  )
		if not Test.IsDebug then
			self:SendFmtToGame(40100)
		else
			self:handle40100()
		end	
	end
	self.model:Bind(TestEvent.REQ_INFO, ON_REQ_INFO)

end

function Test:handle40100(  )
	if not Test.IsDebug then
		local info = {}
		--测试函数调用和返回值的提取
		info.status = self:ReadFmt("c")
		info.time = self:ReadFmt("i")
	else
		local info = {}
		info.status = GuildPlayModel.Status.UnOpen
		info.time = 1
	end
end
