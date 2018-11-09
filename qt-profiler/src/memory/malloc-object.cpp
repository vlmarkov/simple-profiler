#include <include/memory/malloc-object.hpp>


MallocObject::MallocObject(QString addr,
                           QString fun,
                           QString sz,
                           QString ptr) noexcept :
    address(addr),
    function(fun),
    size(sz),
    pointer(ptr) { }
