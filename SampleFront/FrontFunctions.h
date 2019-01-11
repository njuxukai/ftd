#pragma once

#include <string>
#include <functional>
#include <mqclient/MQClientWrapper.h>

typedef std::function<void(PlainHeaders& headers, const std::string&)> UplinkFunction;

typedef std::function<void(const PlainHeaders& headers, const std::string&)> DownlinkFunction;