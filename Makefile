include extern/build/env.mk

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
SRCS := $(patsubst src/%,%,$(SRCS))
OBJS += $(patsubst %.cc,$(OUT)/%.o,$(SRCS))

TEST_SRCS := $(wildcard test/src/*.cc)
TEST_OBJS := $(patsubst %.cc,$(OUT)/%.o,$(notdir $(TEST_SRCS)))

LIBS += -lcurses -lpanel -pthread -ltinfo

# Include all dependencies here
include extern/build/re2.mk
include extern/build/gtest.mk
include extern/build/glog.mk
include extern/build/yaml-cpp.mk

DEPS := $(patsubst %.o,%.d,$(OBJS))
TEST_DEPS := $(patsubst %.o,%.d,$(TEST_OBJS))

.PHONY: test
test: test/unit_tests
	$(Q)cd test; \
	./unit_tests
	 
ARTIFACTS += test/unit_tests
test/unit_tests: $(TEST_OBJS) $(filter-out $(OUT)/$(APP_NAME).o,$(OBJS)) $(BUILD_DEPS) test/settingsTestFile
	$(Q)echo 'Linking target: $@'; \
	$(CXX) -g -o $@  $(filter %.o,$^) $(LIBS) $(TEST_LIBS)

bin/$(APP_NAME): $(BUILD_DEPS) $(OBJS) | bin
	$(Q)echo 'Linking:   $@'; \
	$(CXX) -g -o $@ $(filter %.o,$^) $(LIBS) 


include extern/build/default_rules.mk
-include $(DEPS)
-include $(TEST_DEPS)
