QT        += quick quickcontrols2
CONFIG    += c++14
DEFINES   += QT_DEPRECATED_WARNINGS
RESOURCES += qml.qrc

SOURCES += \
    src/base_profiler.cpp \
    src/main.cpp \
    src/malloc_object.cpp \
    src/observer.cpp \
    src/result.cpp \
    src/memory/memory_controller.cpp \
    src/memory/memory_model.cpp \
    src/memory/memory_view.cpp \
    src/perfomance/perfomance_controller.cpp \
    src/perfomance/perfomance_model.cpp \
    src/perfomance/perfomance_view.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    include/base_profiler.h \
    include/controller.h \
    include/event.h \
    include/malloc_object.h \
    include/model.h \
    include/observer.h \
    include/view.h \
    include/result.h \
    include/memory/memory_controller.h \
    include/memory/memory_model.h \
    include/memory/memory_view.h \
    include/perfomance/perfomance_controller.h \
    include/perfomance/perfomance_model.h \
    include/perfomance/perfomance_view.h

SHARED_LIB_SRC = src/hook_malloc.c

# Manual rule to make c-style .so library
system(gcc -Wall $$SHARED_LIB_SRC -fPIC -shared -o $$OUT_PWD/lib_hook_malloc.so -ldl -rdynamic -pthread -std=c11)

# Manual rule to make samples
system(gcc -g -Wall -O2 -o $$OUT_PWD/sample samples/sample.c -std=c11)
system(gcc -g -Wall -O2 -o $$OUT_PWD/leak-sample samples/leak-sample.c -std=c11)
