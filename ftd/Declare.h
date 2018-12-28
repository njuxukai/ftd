#ifndef FTD_DECLARE_H
#define FTD_DECLARE_H

#include <memory>

#define DECLARE_PTR(X) typedef std::unique_ptr<X> X##Ptr; /**< define smart ptr */
#define DECLARE_SPTR(X) typedef std::shared_ptr<X> X##SPtr; /**< define smart ptr */

#endif