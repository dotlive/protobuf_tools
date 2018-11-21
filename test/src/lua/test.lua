package.cpath  = "../?.so;../?.dll;"..package.cpath
local pb       = (package.loadlib("lpblib.dll", "luaopen_pb"))()           --require "pb"
local conv     = (package.loadlib("lpblib.dll", "luaopen_pb_conv"))()      --require "pb.conv"
local pbio     = (package.loadlib("lpblib.dll", "luaopen_pb_io"))()        --require "pb.io"
local buffer   = (package.loadlib("lpblib.dll", "luaopen_pb_buffer"))()    --require "pb.buffer"
local slice    = (package.loadlib("lpblib.dll", "luaopen_pb_slice"))()     --require "pb.slice"

--print(package.path)
package.path  = "../?.lua;../utils/?.lua;"..package.path
--print(package.path)
local serpent = require("utils.serpent")
local util = require("utils.util")

local test_deserialize = function()
   local encode_msg = pbio.read("../../data/cpp/message.pb")
   if encode_msg then
      --print("1")
      assert(pb.loadfile("../../../output/lua/MESSAGE.proto.bytes"))
      local decode_msg = assert(pb.decode("MESSAGE", encode_msg))
      --print(serpent.block(decode_msg))
      util.dump(decode_msg)
   else
      print("read cpp2lua_message file failed")
   end
end

local test_serialize = function()
   assert(pb.loadfile("../../../output/lua/MESSAGE.proto.bytes"))
   local msg_data = {
      header = 3032,
      errorMsgParams = "人民"
   }
   local encode_msg = assert(pb.encode("MESSAGE", msg_data))
   local file = io.output("../../data/lua/message.pb")
   io.write(encode_msg)
   io.flush()
   io.close()
end

test_deserialize()
test_serialize()
