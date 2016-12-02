package.path = '../protobuf/?.lua;../proto_lua/?.lua;'
local ddz = require "classicddz_pb"

local generator_classicddz_cards = nil
local check_classicddz_cards_valid = nil
local compare_classicddz_cards = nil

local byte2cardval = nil
local pairsbykey = nil
local check_cardtype_match = nil
local check_cardtype_valid = nil
local check_cards_valid = nil
local get_cards_table = nil


local MIN_CARD_NUM = 1
local MAX_CARD_NUM = 20
local MIN_CARD_VAL = 1
local MAX_CARD_VAL = 54
local MAX_CARD_REPEAT_NUM = 4
local MAX_CARD_SHUNZI_NUM = 12

local ctlist = {
	[ddz.e_card_1] 		= {0x1,0x0,0x1,0x0,0x5}, --0x10105
	[ddz.e_card_2] 		= {0x2,0x0,0x1,0x0,0x3}, --0x20103
	[ddz.e_card_3] 		= {0x3,0x0,0x1,0x0,0x2}, --0x30102
	[ddz.e_card_3_1] 	= {0x3,0x1,0x1,0x1,0x2}, --0x31112
	[ddz.e_card_3_2] 	= {0x3,0x2,0x1,0x1,0x2}, --0x32112
	[ddz.e_card_4] 		= {0x4,0x0,0x1,0x0,0x0}, --0x40100
	[ddz.e_card_4_1] 	= {0x4,0x1,0x1,0x1,0x0}, --0x41110
	[ddz.e_card_4_2_1] 	= {0x4,0x1,0x1,0x2,0x0}, --0x41120
	[ddz.e_card_4_2_2] 	= {0x4,0x2,0x1,0x2,0x0}, --0x42120
}

local checklist = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 1, 2, 14}

pairsbykey = function( tbl )
	if type(tbl) ~= "table" then
		return
	end
	local keys = {}
	for k, _ in pairs(tbl) do
		keys[#keys+1] = k
	end
	table.sort(keys)
	local i = #keys + 1
	return function()  
		i = i - 1
		return keys[i], tbl[keys[i]]
	end
end 

byte2cardval = function( byte )
	return math.ceil(byte / MAX_CARD_REPEAT_NUM)
end

get_cards_table = function( cards )
	local len = string.len(cards)
	local valtbl = {}
	for i = 1, len do
		local val = byte2cardval(string.byte(string.sub(cards, i, i)))
		local oval = valtbl[val]
		valtbl[val] = ((oval == nil) and (1) or (oval + 1))
	end
	return valtbl
end

check_cards_valid = function( cards )
	if type(cards) ~= 'string' then
		return false
	end
	local len = string.len(cards)
	local repeattbl = {}
	for i = 1, len do
		local val = string.byte(string.sub(cards, i, i))
		if repeattbl[val] or val < MIN_CARD_VAL or val > MAX_CARD_VAL then
			return false
		end
		repeattbl[val] = true
	end
	return true
end

check_cardtype_valid = function(i, len, n1, n2, p1, p2, cardtbl)
	--check n1
	local j = (i + len - 1)
	for k = i, j do
		local cardnum = cardtbl[checklist[k]]
		if not cardnum or cardnum < n1 then
			return false
		end
	end
	--check n2
	if p2 == 0 then
		return true
	end
	for k = 1, #checklist do
		local cardnum = cardtbl[checklist[k]]
		if cardnum then
			if k >= i and k <= j then
				cardnum = cardnum - n1
			end
			if cardnum % n2 ~= 0 then
				return false
			end
		end
	end
	return true
end

check_cardtype_match = function( e_type, cardtbl, cardnum )
	if cardnum < MIN_CARD_NUM or cardnum > MAX_CARD_NUM then
		return false
	end
	local ct = ctlist[e_type]
	if ct == nil then
		return false
	end
	local n1, n2, p1, p2, rn = ct[1], ct[2], ct[3], ct[4], ct[5]
	local np = n1 * p1 + n2 * p2
	local len = cardnum / np
	if (cardnum % np ~= 0) or (rn == 0 and len ~= 1) or (rn ~= 0 and len ~= 1 and len < rn) then
		return false
	end
	local bg, ed = 1, #checklist - len + 1
	if len > 1 then
		ed = MAX_CARD_SHUNZI_NUM - len + 1
	end
	for i = ed, bg, -1 do
		if check_cardtype_valid(i, len, n1, n2, p1, p2, cardtbl) then
			local desc = string.char(i) .. string.char(len)
			return true, desc
		end
	end
	return false
end

generator_classicddz_cards = function( cards )
	if type(cards) ~= 'string' then
		return nil
	end
	local cardnum = string.len(cards)
	if cardnum < MIN_CARD_NUM or cardnum > MAX_CARD_NUM or not check_cards_valid(cards) then
		return nil
	end
	local cardtbl = get_cards_table(cards)
	--check type
	for e_type in pairsbykey(ctlist) do
		local bvalid, desc = check_cardtype_match(e_type, cardtbl, cardnum)
		if bvalid and desc then
			local msg = ddz.classicddz_cards()
			msg.e_type = e_type
			msg.cards = cards
			msg.desc = desc
			return msg:SerializeToString()
		end
	end
	return nil
end

check_classicddz_cards_valid = function( pbbytes )
	if type(pbbytes) ~= 'string' then
		return false
	end
	local msg = ddz.classicddz_cards()
	msg:ParseFromString(pbbytes)
	print(msg.e_type)
	local cardnum = string.len(msg.cards)
	if cardnum < MIN_CARD_NUM or cardnum > MAX_CARD_NUM or not check_cards_valid(msg.cards) or string.len(msg.desc) ~= 2 then
		return nil
	end
	local ct = ctlist[msg.e_type]
	if ct == nil then
		return false
	end
	local n1, n2, p1, p2, rn = ct[1], ct[2], ct[3], ct[4], ct[5]
	local np = n1 * p1 + n2 * p2
	local i = string.byte(string.sub(msg.desc, 1, 1))
	local len = string.byte(string.sub(msg.desc, 2, 2))
	if np * len ~= cardnum then
		return false
	end
	local cardtbl = get_cards_table(msg.cards)
	return check_cardtype_valid(i, len, n1, n2, p1, p2, cardtbl)
end

compare_classicddz_cards = function( pbbytes1, pbbytes2 )
	if type(pbbytes1) ~= 'string' or type(pbbytes2) ~= 'string' then
		return false
	end
	local msg1 = ddz.classicddz_cards()
	msg1:ParseFromString(pbbytes1)
	local msg2 = ddz.classicddz_cards()
	msg2:ParseFromString(pbbytes2)
	local i1 = string.byte(string.sub(msg1.desc, 1, 1))
	local i2 = string.byte(string.sub(msg2.desc, 1, 1))
	local len1 = string.byte(string.sub(msg1.desc, 2, 2))
	local len2 = string.byte(string.sub(msg2.desc, 2, 2))
	local king1 = false
	local king2 = false
	if len1 == 1 and msg1.e_type == ddz.e_card_2 and string.byte(string.sub(msg1.cards, 1, 1)) >= 53 then
		king1 = true
	end
	if len2 == 1 and msg2.e_type == ddz.e_card_2 and string.byte(string.sub(msg2.cards, 1, 1)) >= 53 then
		king2 = true
	end
	if king1 then
		return false
	end
	if king2 then
		return true
	end
	if msg1.e_type == msg2.e_type then
		if i2 > i1 then
			return true
		end
	else
		if msg2.e_type == ddz.e_card_4 then
			return true
		end
	end
	return false
end


-- test 
----------------------------------------------------------
local a = ''
for i = 1, 54 do
	a = a .. string.char(i)
	--print(string.byte(string.sub(a, i, i)))
	--print(byte2cardval(i))
end

for k in pairsbykey(ctlist) do
	print(k)
end

print ((1 == nil) and 2 or 3)
print (1 and 2)
print (2 and 3)
print (1 or 2)
print (2 or 3)

local cards = ''
cards = cards .. string.char(5)
cards = cards .. string.char(6)
cards = cards .. string.char(7)
cards = cards .. string.char(8)
cards = cards .. string.char(54)
cards = cards .. string.char(53)
local pbbytes = generator_classicddz_cards(cards)
if pbbytes then
	if check_classicddz_cards_valid(pbbytes) then
		print('check_classicddz_cards_valid ok')
	end
else
	print("can't generator")
end

local cards2 = ''
cards2 = cards2 .. string.char(1)
cards2 = cards2 .. string.char(2)
cards2 = cards2 .. string.char(3)
cards2 = cards2 .. string.char(4)
local pbbytes2 = generator_classicddz_cards(cards2)
if pbbytes2 then
	if check_classicddz_cards_valid(pbbytes2) then
		print('check_classicddz_cards_valid ok')
	end
else
	print("can't generator")
end

if compare_classicddz_cards(pbbytes, pbbytes2) then
	print("pbbytes2 > pbbytes1")
else
	print("pbbytes2 < pbbytes1")
end

print("over")