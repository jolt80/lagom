# Stop echo of command lines
Q=@

# Search for source files in src
VPATH += src

CPPFLAGS += -O3 -Wall -std=c++11 -g

INC += -Isrc
INC += -Ire2

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst %.cpp,obj/%.o,$(notdir $(SRCS)))
DEPS := $(patsubst %.cpp,obj/%.d,$(notdir $(SRCS)))

RE2_OBJS = $(wildcard re2/obj/re2/*.o)
RE2_OBJS += $(wildcard re2/obj/util/*.o)

LIBS += -lcurses -lpanel -pthread

ifdef LMR_SITE
LIBS += -ltinfo -L/app/vbuild/RHEL6-x86_64/gcc/4.9.2/lib64 -Wl,-rpath,/app/vbuild/RHEL6-x86_64/gcc/4.9.2/lib64
endif

# Default
all: bin/rcs_log_parser

compile: $(OBJS) | bin

test:
	$(Q)echo $(LIBS)
	$(Q)echo $(patsubst %.o,%.d,$(OBJS))
	$(Q)echo $(HEADERS)

bin/rcs_log_parser: $(OBJS) | bin
	$(Q)echo 'Linking target: $@'; \
	$(CXX) -g -o $@ $(filter %.o,$^) $(RE2_OBJS) $(LIBS) 
		
obj/%.o: %.cpp Makefile | obj
	$(Q)echo 'Compiling: $<'; \
	$(CXX) -MMD -MP $(INC) $(CPPFLAGS) -c -o $@ $<

obj bin:
	$(Q)mkdir -p $@

clean:
	$(Q)$(RM) -rf obj
	$(Q)$(RM) -rf bin

-include $(DEPS)