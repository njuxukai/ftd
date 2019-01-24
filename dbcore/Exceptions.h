#ifndef MCO_EXCEPTIONS_H_
#define MCO_EXCEPTIONS_H_

#include <string>
#include <stdexcept>
#include <ftd/Errors.h>

#define MCO_ERROR_CODE_TRANSACTION_ERROR 0x0100
#define MCO_ERROR_CODE_INDEX_FIND_ERROR 0x0101
#define MCO_ERROR_CODE_USER_ID_NOT_EXIST 0x0102
#define MCO_ERROR_CODE_PASSWORD_ERROR 0x0103
#define MCO_ERROR_CODE_AUTHORIZATION_DENIED 0x0104
namespace MCO 
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

	struct IndexFindError : public Exception
	{
		IndexFindError(const std::string& what="") 
			:Exception(MCO_ERROR_CODE_INDEX_FIND_ERROR,
				"Index find error or unique index exists",
				what)
		{}
	};


	struct AuthorizationDenied : public Exception
	{
		AuthorizationDenied(const std::string& what = "")
			:Exception(MCO_ERROR_CODE_INDEX_FIND_ERROR,
				"Authorization denied",
				what)
		{}
	};
}

#endif