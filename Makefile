# Stop echo of command lines
Q:=@

ifdef V
Q:=
endif

APP_NAME := lagom

# Default
.PHONY: all
all: bin/$(APP_NAME)

# Search for source files in src
VPATH += src test/src

CPPFLAGS += -Wall -std=c++14 -g

# Optimization flag for release build
CPPFLAGS += -Wall -Werror -O3

INC += -Isrc

SRCS := $(wildcard src/*.cc)
SRCS := $(filter-out src/lagom.cc,$(SRCS))
SRCS := $(patsubst src/%,%,$(SRCS))
APP_OBJ := out/$(APP_NAME).o
OBJS += $(patsubst %.cc,out/%.o,$(SRCS))

TEST_SRCS := $(wildcard test/src/*.cc)
TEST_OBJS := $(patsubst %.cc,out/%.o,$(notdir $(TEST_SRCS)))

OUT_DIR := out

LIBS += -lcurses -lpanel -pthread

# This makes it work out of the box in an Ericsson VDI environment
ifdef LMR_SITE
export CXX := /app/vbuild/RHEL6-x86_64/gcc/5.2.0/bin/g++
LIBS += -ltinfo -L/app/vbuild/RHEL6-x86_64/gcc/5.2.0/lib64 -Wl,-rpath,/app/vbuild/RHEL6-x86_64/gcc/5.2.0/lib64
LIBS += -ltinfo -L/app/vbuild/RHEL6-x86_64/gcc/5.2.0/lib64 -Wl,-rpath,/app/vbuild/RHEL6-x86_64/gcc/5.2.0/lib64
endif

# Include all dependencies here
include extern/build/re2.mk
include extern/build/gtest.mk

DEPS := $(patsubst %.cc,%.d,$(SRCS))

compile: $(OBJS)

.PHONY: test
test: test/unit_tests
	$(Q)cd test; \
	./unit_tests
	 
test/unit_tests: $(TEST_OBJS) $(OBJS) $(BUILD_DEPS) test/settingsTestFile
	$(Q)echo 'Linking target: $@'; \
	$(CXX) -g -o $@  $(filter %.o,$^) $(LIBS) $(TEST_LIBS)

print:
	$(Q)echo $(TEST_SRCS)
	$(Q)echo $(OBJS)
	$(Q)echo $(TEST_OBJS)

bin/$(APP_NAME): $(APP_OBJ) $(OBJS) $(BUILD_DEPS) | bin
	$(Q)echo 'Linking:   $@'; \
	$(CXX) -g -o $@ $(filter %.o,$^) $(LIBS) 

bin:
	$(Q)mkdir -p bin
		
out/%.o: %.cc
	$(Q)echo 'Compiling: $@'; \
	mkdir -p $(dir $@); \
	$(CXX) -MMD -MP $(INC) $(CPPFLAGS) -c -o $@ $<

clean: $(CLEAN_DEPS)
	$(Q)$(RM) -rf out
	$(Q)$(RM) -rf bin
	$(Q)$(RM) -rf test/unit_tests

-include $(DEPS)
-include $(TEST_DEPS)
