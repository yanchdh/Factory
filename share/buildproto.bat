del /f /s /q .\proto-cpp\*.*

for /f %%i in ('dir /b "proto\*.proto"') do protoc.exe -I=proto --cpp_out=proto-cpp proto\%%i
   
@pause