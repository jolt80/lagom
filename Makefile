# Stop echo of command lines
Q=@


APP_NAME := lagom

# Search for source files in src
VPATH += src test/src

CPPFLAGS += -Wall -std=c++11 -g

# Optimization flag for release build
CPPFLAGS += -Wall -O3

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

LIBS += -Lre2/obj -lre2 -lcurses -lpanel -pthread

ifdef LMR_SITE
export CXX := /app/vbuild/RHEL6-x86_64/gcc/5.2.0/bin/g++
LIBS += -ltinfo -L/app/vbuild/RHEL6-x86_64/gcc/5.2.0/lib64 -Wl,-rpath,/app/vbuild/RHEL6-x86_64/gcc/5.2.0/lib64
LIBS += -ltinfo -L/app/vbuild/RHEL6-x86_64/gcc/5.2.0/lib64 -Wl,-rpath,/app/vbuild/RHEL6-x86_64/gcc/5.2.0/lib64
endif

# Default
#all: test
all: bin/$(APP_NAME)

compile: $(OBJS) | bin

test: test/unit_tests
	$(Q)cd test; \
	./unit_tests
	 

test/unit_tests: $(TEST_OBJS) $(OBJS) test/settingsTestFile
	$(Q)echo 'Linking target: $@'; \
	$(CXX) -g -o $@  $(filter %.o,$^) $(LIBS) $(TEST_LIBS)

print:
	$(Q)echo $(TEST_SRCS)
	$(Q)echo $(OBJS)
	$(Q)echo $(TEST_OBJS)

bin/$(APP_NAME): $(APP_OBJ) $(OBJS) re2/obj/libre2.a | bin
	$(Q)echo 'Linking target: $@'; \
	$(CXX) -g -o $@ $(filter %.o,$^) $(LIBS) 
		
obj/%.o test/obj/%.o: %.cpp | obj  test/obj
	$(Q)echo 'Compiling: $<'; \
	$(CXX) -MMD -MP $(INC) $(CPPFLAGS) -c -o $@ $<

obj bin test/obj:
	$(Q)mkdir -p $@

re2/obj/libre2.a:
	$(Q)make -C re2 -j8 all

clean:
	$(Q)$(RM) -rf obj
	$(Q)$(RM) -rf test/obj
	$(Q)$(RM) -rf bin
	$(Q)$(RM) -rf test/unit_tests
	$(Q)make -C re2 clean

-include $(DEPS)
-include $(TEST_DEPS)
