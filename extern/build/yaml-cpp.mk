# Fragment that compiles yaml-cpp from source

INC += -Iextern/yaml-cpp/include
LIBS += -L$(OUT)/lib -lyamlcpp

YAML_CPP_SRCS := $(wildcard extern/yaml-cpp/src/*.cpp)
YAML_CPP_SRCS += $(wildcard extern/yaml-cpp/src/contrib/*.cpp)

YAML_CPP_OBJS += $(patsubst %.cpp,$(OUT)/%.o,$(YAML_CPP_SRCS))

LIB_DEPS += $(OUT)/lib/libyamlcpp.a
$(OUT)/lib/libyamlcpp.a: | $(OUT)/lib
	$(Q) echo [yaml-cpp] compiling...; \
	make -j8 $(YAML_CPP_OBJS) $(REDIRECT) || exit 1; \
	echo [yaml-cpp] archiving $@; \
	$(AR) rcs $@ $(YAML_CPP_OBJS) $(REDIRECT) || exit 1