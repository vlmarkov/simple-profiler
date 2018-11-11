#include <include/profiler/mem-leak/mem-leak-object.hpp>


MemLeakObj::MemLeakObj(QString addr,
                       QString fun,
                       QString sz,
                       QString ptr) noexcept :
    address(addr),
    function(fun),
    size(sz),
    pointer(ptr) { }
