# Stop echo of command lines
Q=@


APP_NAME := lagom

# Search for source files in src
VPATH += src test/src

CPPFLAGS += -O3 -Wall -std=c++11 -g

INC += -Isrc
INC += -Ire2

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst %.cpp,obj/%.o,$(filter-out $(APP_NAME).cpp,$(notdir $(SRCS))))
APP_OBJ := obj/$(APP_NAME).o
DEPS := $(patsubst %.cpp,obj/%.d,$(notdir $(SRCS)))

RE2_OBJS = $(wildcard re2/obj/re2/*.o)
RE2_OBJS += $(wildcard re2/obj/util/*.o)

TEST_SRCS := $(wildcard test/src/*.cpp)
TEST_OBJS := $(patsubst %.cpp,test/obj/%.o,$(notdir $(TEST_SRCS)))

TEST_LIBS := /usr/src/gtest/libgtest.a /usr/src/gtest/libgtest_main.a -pthread

LIBS += -lcurses -lpanel -pthread

ifdef LMR_SITE
LIBS += -ltinfo -L/app/vbuild/RHEL6-x86_64/gcc/4.9.2/lib64 -Wl,-rpath,/app/vbuild/RHEL6-x86_64/gcc/4.9.2/lib64
endif

# Default
all: test
#all: bin/$(APP_NAME)

compile: $(OBJS) | bin

test: test/unit_tests
	$(Q)cd test; \
	./unit_tests
	 

test/unit_tests: $(TEST_OBJS) $(OBJS)
	$(Q)echo 'Linking target: $@'; \
	$(CXX) -g -o $@  $(filter %.o,$^) $(RE2_OBJS) $(LIBS) $(TEST_LIBS)

print:
	$(Q)echo $(TEST_SRCS)
	$(Q)echo $(OBJS)
	$(Q)echo $(TEST_OBJS)

bin/$(APP_NAME): $(APP_OBJ) $(OBJS) | bin
	$(Q)echo 'Linking target: $@'; \
	$(CXX) -g -o $@ $(filter %.o,$^) $(RE2_OBJS) $(LIBS) 
		
obj/%.o test/obj/%.o: %.cpp | obj  test/obj
	$(Q)echo 'Compiling: $<'; \
	$(CXX) -MMD -MP $(INC) $(CPPFLAGS) -c -o $@ $<

obj bin test/obj:
	$(Q)mkdir -p $@

clean:
	$(Q)$(RM) -rf obj
	$(Q)$(RM) -rf test/obj
	$(Q)$(RM) -rf bin
	$(Q)$(RM) -rf test/unit_tests

-include $(DEPS)
-include $(TEST_DEPS)