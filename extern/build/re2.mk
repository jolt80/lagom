# Fragment that compiles re2 from source

INC += -Iextern/re2

CPPFLAGS += -Wno-error=attributes -Wno-attributes

RE2_SRCS := $(wildcard extern/re2/re2/*.cc)
RE2_SRCS += $(wildcard extern/re2/util/*.cc)

# Remove windows-specific source file
RE2_SRCS := $(filter-out extern/re2/util/threadwin.cc,$(RE2_SRCS))	

# RE2 seems to have it's test in util together with other useful stuff, remove all files that contain a main()
RE2_SRCS := $(filter-out extern/re2/util/benchmark.cc,$(RE2_SRCS))
RE2_SRCS := $(filter-out extern/re2/util/test.cc,$(RE2_SRCS))
RE2_SRCS := $(filter-out extern/re2/util/fuzz.cc,$(RE2_SRCS))

OBJS += $(patsubst %.cc,out/%.o,$(RE2_SRCS))