# Fragment that compiles re2 from source

INC += -Iextern/re2
LIBS += -L$(OUT)/lib -lre2

CPPFLAGS += -Wno-error=attributes -Wno-attributes

RE2_SRCS := $(wildcard extern/re2/re2/*.cc)
RE2_SRCS += $(wildcard extern/re2/util/*.cc)

# Remove windows-specific source file
RE2_SRCS := $(filter-out extern/re2/util/threadwin.cc,$(RE2_SRCS))	

# RE2 seems to have it's test in util together with other useful stuff, remove all files that contain a main()
RE2_SRCS := $(filter-out extern/re2/util/benchmark.cc,$(RE2_SRCS))
RE2_SRCS := $(filter-out extern/re2/util/test.cc,$(RE2_SRCS))
RE2_SRCS := $(filter-out extern/re2/util/fuzz.cc,$(RE2_SRCS))

RE2_OBJS += $(patsubst %.cc,$(OUT)/%.o,$(RE2_SRCS))

LIB_DEPS += $(OUT)/lib/libre2.a
$(OUT)/lib/libre2.a: | $(OUT)/lib
	$(Q) echo [re2] compiling...; \
	make -j8 $(RE2_OBJS) $(REDIRECT) || exit 1; \
	echo [re2] archiving $@; \
	$(AR) rcs $@ $(RE2_OBJS) $(REDIRECT) || exit 1