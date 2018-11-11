QT        += quick quickcontrols2
CONFIG    += c++14
DEFINES   += QT_DEPRECATED_WARNINGS
RESOURCES += qml.qrc

# Include boost library
LIBS += -L/usr/lib/ -lboost_system

# Common files
SOURCES += \
    src/main.cpp \
    src/i-factory.cpp \
    src/base-profiler.cpp \
    src/profiler/result.cpp \
    src/profiler/exception.cpp \
    src/profiler/observable.cpp

# Memory-leak profiler's files
SOURCES += \
    src/profiler/mem-leak/mem-leak-object.cpp \
    src/profiler/mem-leak/mem-leak-profiler.cpp \
    src/profiler/mem-leak/mem-leak-view.cpp \
    src/profiler/mem-leak/mem-leak-model.cpp \
    src/profiler/mem-leak/mem-leak-controller.cpp

# Perf profiler's files
SOURCES += \
    src/profiler/perf/perf-utils.cpp \
    src/profiler/perf/perf-event.cpp \
    src/profiler/perf/perf-profiler-events.cpp \
    src/profiler/perf/perf-profiler-sampling.cpp \
    src/profiler/perf/perf-controller.cpp \
    src/profiler/perf/perf-model-events.cpp \
    src/profiler/perf/perf-model-sampling.cpp \
    src/profiler/perf/perf-view-sampling.cpp \
    src/profiler/perf/perf-view-events.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Common files
HEADERS += \
    include/i-factory.hpp \
    include/i-profiler.hpp \
    include/base-profiler.hpp \
    include/profiler/event.hpp \
    include/profiler/result.hpp \
    include/profiler/exception.hpp \
    include/profiler/observable.hpp \
    include/profiler/i-model.hpp \
    include/profiler/i-view.hpp \
    include/profiler/i-observer.hpp \
    include/profiler/i-controller.hpp


# Memory-leak profiler's files
HEADERS += \
    include/profiler/mem-leak/mem-leak-view.hpp \
    include/profiler/mem-leak/mem-leak-model.hpp \
    include/profiler/mem-leak/mem-leak-controller.hpp \
    include/profiler/mem-leak/mem-leak-object.hpp \
    include/profiler/mem-leak/mem-leak-profiler.hpp

# Perf profiler's files
HEADERS += \
    include/profiler/perf/perf-event.hpp \
    include/profiler/perf/perf-utils.hpp \
    include/profiler/perf/perf-profiler-events.hpp \
    include/profiler/perf/perf-profiler-sampling.hpp \
    include/profiler/perf/perf-controller.hpp \
    include/profiler/perf/perf-model-events.hpp \
    include/profiler/perf/perf-model-sampling.hpp \
    include/profiler/perf/perf-view-events.hpp \
    include/profiler/perf/perf-view-sampling.hpp


SHARED_LIB_SRC = ../hook-malloc/src/hook_malloc.c

# Manual rule to make c-style .so library
system(gcc -Wall $$SHARED_LIB_SRC -fPIC -shared -o $$OUT_PWD/lib_hook_malloc.so -ldl -rdynamic -pthread -std=c11 -D GUI)

# Manual rule to make samples
system(gcc -g -Wall -O2 -o $$OUT_PWD/mem-leak-sample test/samples/mem-leak-sample.c -std=c11)
system(g++ -g -Wall -o $$OUT_PWD/perf-loop-sample test/samples/perf-loop.cpp -std=c++14)
