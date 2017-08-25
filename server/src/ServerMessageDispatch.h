#pragma once
#include "MessageDispatch.h"

class ServerMessageDispatch : public MessageDispatch
{
public:
	virtual void OnConnectionRead(int fd, int msgid, std::shared_ptr<char> pdata, int len);
	virtual void OnConnectionSuccess(int fd);
	virtual void OnConnectionClose(int fd);
};