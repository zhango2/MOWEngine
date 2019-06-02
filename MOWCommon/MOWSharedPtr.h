#ifndef MOWSharedPtr_H
#define MOWSharedPtr_H

#include <memory>

#define DECLARE_SHARED_PTR(className) \
    class className;\
    using className##Ptr = std::shared_ptr<className>;\
    using className##PtrC = std::shared_ptr<const className>;

#endif
