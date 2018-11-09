CC        := gcc
CFLAGS    := -g -Wall -std=gnu99
INC       := -I include

SRCDIR    := src
TESTDIR   := test
BUILDDIR  := build

all: profiler-event profiler-sample tests

profiler-event: perf-event profiler-hw-events
	@echo " Building $@..."
	@mkdir -p $(BUILDDIR);
	$(CC) $(CFLAGS) $(INC) -o $(BUILDDIR)/$@ $(SRCDIR)/main.c \
	$(BUILDDIR)/perf-event.o \
	$(BUILDDIR)/profiler-hw-events.o

profiler-sample: perf-event profiler-sampling
	@echo " Building $@..."
	@mkdir -p $(BUILDDIR);
	$(CC) $(CFLAGS) $(INC) -o $(BUILDDIR)/$@ $(SRCDIR)/main.c \
	$(BUILDDIR)/perf-event.o \
	$(BUILDDIR)/profiler-sampling.o -D SAMPLING

perf-event:
	@echo " Building $@..."
	@mkdir -p $(BUILDDIR);
	$(CC) $(CFLAGS) $(INC) -c -o $(BUILDDIR)/$@.o $(SRCDIR)/$@.c 

profiler-hw-events:
	@echo " Building $@..."
	@mkdir -p $(BUILDDIR);
	$(CC) $(CFLAGS) $(INC) -c -o $(BUILDDIR)/$@.o $(SRCDIR)/$@.c 

profiler-sampling:
	@echo " Building $@..."
	@mkdir -p $(BUILDDIR);
	$(CC) $(CFLAGS) $(INC) -c -o $(BUILDDIR)/$@.o $(SRCDIR)/$@.c 

tests:
	@echo " Building $@..."
	@mkdir -p $(BUILDDIR);
	$(CC) $(CFLAGS) -o $(BUILDDIR)/loop $(TESTDIR)/sample/loop.c;
	$(CC) $(CFLAGS) -o $(BUILDDIR)/hello-world $(TESTDIR)/sample/hello-world.c;

clean:
	@echo " Cleaning...";
	rm -Rf $(BUILDDIR)
