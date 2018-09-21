#include "include/malloc_object.h"


MallocObject::MallocObject(QString addr,
                           QString fun,
                           QString size,
                           QString ptr) :
    _addr(addr), _fun(fun), _size(size), _ptr(ptr)
{
    ;
}

MallocObject::~MallocObject()
{
    ;
}
