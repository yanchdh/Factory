#pragma once
#include <memory>

class MessageDispatch
{
public:
	virtual void OnConnectionRead(int fd, int msgid, std::shared_ptr<char> pdata, int len) = 0;
	virtual void OnConnectionSuccess(int fd) {}
	virtual void OnConnectionClose(int fd) {}
};