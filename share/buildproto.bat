del /f /s /q .\proto_cpp\*.*
del /f /s /q .\proto_lua\*.*

for /f %%i in ('dir /b "proto\*.proto"') do protoc.exe --plugin=protoc-gen-lua="protoc-gen-lua.bat" -I=proto --cpp_out=proto_cpp --lua_out=proto_lua proto\%%i
   
@pause