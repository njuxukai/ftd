#ifndef USUAL_MACRO_H
#define USUAL_MACRO_H

#include <memory>

#define DECLARE_PTR(X) typedef std::unique_ptr<X> X##Ptr; /**< define smart ptr */
#define DECLARE_SPTR(X) typedef std::shared_ptr<X> X##SPtr; /**< define smart ptr */


#define DISABLE_COPY_AND_ASSIGN(ClassName)\
		ClassName(const ClassName&) = delete;\
		ClassName& operator=(const ClassName&) = delete;

#endif