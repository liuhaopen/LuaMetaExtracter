function func1( ... )
	local func2 = function (  )
		local func3 = function (  )
			call_func3(3)
		end
		call_func2(2)
	end
	call_func1(1)
end
call_func_in_out_side(0)
