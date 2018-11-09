QT        += quick quickcontrols2
CONFIG    += c++14
DEFINES   += QT_DEPRECATED_WARNINGS
RESOURCES += qml.qrc

SOURCES += \
    src/main.cpp \
    src/result.cpp \
    src/observable.cpp \
    src/base-profiler.cpp \
    src/memory/memory-view.cpp \
    src/memory/memory-model.cpp \
    src/memory/memory-controller.cpp \
    src/memory/malloc-object.cpp \
    src/perfomance/perfomance-view.cpp \
    src/perfomance/perfomance-model.cpp \
    src/perfomance/perfomance-controller.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/event.hpp \
    include/result.hpp \
    include/observable.hpp \
    include/base-profiler.hpp \
    include/i-model.hpp \
    include/i-view.hpp \
    include/i-observer.hpp \
    include/i-controller.hpp \
    include/memory/memory-view.hpp \
    include/memory/memory-model.hpp \
    include/memory/memory-controller.hpp \
    include/memory/malloc-object.hpp \
    include/perfomance/perfomance-view.hpp \
    include/perfomance/perfomance-model.hpp \
    include/perfomance/perfomance-controller.hpp


SHARED_LIB_SRC = ../hook-malloc/src/hook_malloc.c

# Manual rule to make c-style .so library
system(gcc -Wall $$SHARED_LIB_SRC -fPIC -shared -o $$OUT_PWD/lib_hook_malloc.so -ldl -rdynamic -pthread -std=c11 -D GUI)

# Manual rule to make samples
system(gcc -g -Wall -O2 -o $$OUT_PWD/sample samples/sample.c -std=c11)
system(gcc -g -Wall -O2 -o $$OUT_PWD/leak-sample samples/leak-sample.c -std=c11)
