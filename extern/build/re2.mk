# Fragment that compiles re2 from source

INC += -Iextern/re2

RE2_SRCS := $(wildcard extern/re2/re2/*.cc)
RE2_SRCS += $(wildcard extern/re2/util/*.cc)
RE2_SRCS := $(filter-out extern/re2/util/threadwin.cc,$(RE2_SRCS))	
RE2_SRCS := $(filter-out extern/re2/util/benchmark.cc,$(RE2_SRCS))
RE2_SRCS := $(filter-out extern/re2/util/test.cc,$(RE2_SRCS))
OBJS += $(patsubst %.cc,out/%.o,$(RE2_SRCS))
