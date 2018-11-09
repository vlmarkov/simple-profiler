QT        += quick quickcontrols2
CONFIG    += c++14
DEFINES   += QT_DEPRECATED_WARNINGS
RESOURCES += qml.qrc

SOURCES += \
    src/main.cpp \
    src/base-profiler.cpp \
    src/profiler/result.cpp \
    src/profiler/observable.cpp \
    src/profiler/memory/memory-view.cpp \
    src/profiler/memory/memory-model.cpp \
    src/profiler/memory/memory-controller.cpp \
    src/profiler/memory/memory-factory.cpp \
    src/profiler/memory/malloc-object.cpp \
    src/profiler/memory/memory-profiler.cpp \
    src/profiler/perfomance/perfomance-view.cpp \
    src/profiler/perfomance/perfomance-model.cpp \
    src/profiler/perfomance/perfomance-controller.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/i-factory.hpp \
    include/i-profiler.hpp \
    include/base-profiler.hpp \
    include/profiler/event.hpp \
    include/profiler/result.hpp \
    include/profiler/observable.hpp \
    include/profiler/i-model.hpp \
    include/profiler/i-view.hpp \
    include/profiler/i-observer.hpp \
    include/profiler/i-controller.hpp \
    include/profiler/memory/memory-view.hpp \
    include/profiler/memory/memory-model.hpp \
    include/profiler/memory/memory-controller.hpp \
    include/profiler/memory/malloc-object.hpp \
    include/profiler/memory/memory-factory.hpp \
    include/profiler/memory/memory-profiler.hpp \
    include/profiler/perfomance/perfomance-view.hpp \
    include/profiler/perfomance/perfomance-model.hpp \
    include/profiler/perfomance/perfomance-controller.hpp


SHARED_LIB_SRC = ../hook-malloc/src/hook_malloc.c

# Manual rule to make c-style .so library
system(gcc -Wall $$SHARED_LIB_SRC -fPIC -shared -o $$OUT_PWD/lib_hook_malloc.so -ldl -rdynamic -pthread -std=c11 -D GUI)

# Manual rule to make samples
system(gcc -g -Wall -O2 -o $$OUT_PWD/sample samples/sample.c -std=c11)
system(gcc -g -Wall -O2 -o $$OUT_PWD/leak-sample samples/leak-sample.c -std=c11)
