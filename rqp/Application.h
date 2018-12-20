#ifndef RQP_APPLICATION_H
#define RQP_APPLICATION_H
#include <string>
#include "rqp.h"
class Application
{
	//app
	virtual void onPrivateFtdcMessage(const std::string& msg);
	virtual void onBoardcastFtdcMessage(const std::string& msg);
	virtual void onDialogFtdcMessage(const std::string& msg, const DialogExtHeader& header);
};

#endif