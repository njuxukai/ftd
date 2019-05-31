#ifndef MCO_EXCEPTIONS_H_
#define MCO_EXCEPTIONS_H_

#include <string>
#include <stdexcept>
#include <ftd/Errors.h>

#define MCO_ERROR_CODE_TRANSACTION_ERROR 100
#define MCO_ERROR_CODE_INDEX_FIND_ERROR 101
#define MCO_ERROR_CODE_USER_ID_NOT_EXIST 102
#define MCO_ERROR_CODE_PASSWORD_ERROR 103
#define MCO_ERROR_CODE_AUTHORIZATION_DENIED 104
#define MCO_ERROR_CODE_GENERATE_SNO  105
#define MCO_ERROR_CODE_STATUS_UNNORMAL 106
namespace dbcore 
{
	/// Base MCO exception type.
	struct Exception : public std::logic_error
	{
		Exception(int code, const std::string& text, const std::string& d)
			: std::logic_error(d.size() ? text + ": " + d : text),
			errorCode(code), errorText(text), detail(d)
		{}
		~Exception() throw() {}
		
		int errorCode;
		std::string errorText;
		std::string detail;
	};

	struct SNoGenerateError : public Exception
	{
		SNoGenerateError(const std::string& detail="") 
			: Exception(MCO_ERROR_CODE_GENERATE_SNO,
				"ÐòÁÐºÅÉú³ÉÊ§°Ü",
				detail)
		{

		}
	};

	struct IndexFindError : public Exception
	{
		IndexFindError(const std::string& detail="") 
			:Exception(MCO_ERROR_CODE_INDEX_FIND_ERROR,
				"Ë÷Òý´íÎó",
				detail)
		{}
	};

	struct StatusError : public Exception
	{
		StatusError(const std::string& detail="")
			:Exception(MCO_ERROR_CODE_STATUS_UNNORMAL,
			"×´Ì¬´íÎó",
			detail)
			{}
	};

	struct AuthorizationDenied : public Exception
	{
		AuthorizationDenied(const std::string& detail = "")
			:Exception(MCO_ERROR_CODE_INDEX_FIND_ERROR,
				"Authorization denied",
				detail)
		{}
	};
}

#endif