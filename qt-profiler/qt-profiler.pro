QT        += quick quickcontrols2
CONFIG    += c++14
DEFINES   += QT_DEPRECATED_WARNINGS
RESOURCES += qml.qrc

# Include boost library
LIBS += -L/usr/lib/ -lboost_system

# Main files
SOURCES += \
    src/main.cpp \
    src/i-factory.cpp \
    src/base-profiler.cpp \
    src/profiler/result.cpp \
    src/profiler/observable.cpp

# Memory profiler's files
SOURCES += \
    src/profiler/memory/memory-view.cpp \
    src/profiler/memory/memory-model.cpp \
    src/profiler/memory/memory-controller.cpp \
    src/profiler/memory/malloc-object.cpp \
    src/profiler/memory/memory-profiler.cpp

# Perfomance profiler's files
SOURCES += \
    src/profiler/perfomance/perf/utils.cpp \
    src/profiler/perfomance/perf/event.cpp \
    src/profiler/perfomance/perf/exception.cpp \
    src/profiler/perfomance/perf/profiler-events.cpp \
    src/profiler/perfomance/perf/profiler-sampling.cpp \
    src/profiler/perfomance/perf/perf-controller.cpp \
    src/profiler/perfomance/perf/perf-model-sampling.cpp \
    src/profiler/perfomance/perf/perf-view-sampling.cpp \
    src/profiler/perfomance/perf/perf-view-events.cpp \
    src/profiler/perfomance/perf/perf-model-events.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Main files
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
    include/profiler/i-controller.hpp

# Memory profiler's files
HEADERS += \
    include/profiler/memory/memory-view.hpp \
    include/profiler/memory/memory-model.hpp \
    include/profiler/memory/memory-controller.hpp \
    include/profiler/memory/malloc-object.hpp \
    include/profiler/memory/memory-profiler.hpp

# Perfomance profiler's files
HEADERS += \
    include/profiler/perfomance/perf/event.hpp \
    include/profiler/perfomance/perf/utils.hpp \
    include/profiler/perfomance/perf/exception.hpp \
    include/profiler/perfomance/perf/profiler-events.hpp \
    include/profiler/perfomance/perf/profiler-sampling.hpp \
    include/profiler/perfomance/perf/perf-controller.hpp \
    include/profiler/perfomance/perf/perf-model-events.hpp \
    include/profiler/perfomance/perf/perf-model-sampling.hpp \
    include/profiler/perfomance/perf/perf-view-events.hpp \
    include/profiler/perfomance/perf/perf-view-sampling.hpp


SHARED_LIB_SRC = ../hook-malloc/src/hook_malloc.c

# Manual rule to make c-style .so library
system(gcc -Wall $$SHARED_LIB_SRC -fPIC -shared -o $$OUT_PWD/lib_hook_malloc.so -ldl -rdynamic -pthread -std=c11 -D GUI)

# Manual rule to make samples
system(gcc -g -Wall -O2 -o $$OUT_PWD/sample samples/sample.c -std=c11)
system(gcc -g -Wall -O2 -o $$OUT_PWD/leak-sample samples/leak-sample.c -std=c11)
