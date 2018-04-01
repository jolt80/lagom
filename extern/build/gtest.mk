# Fragment that compiles gtest/gmock from source

INC += -Iextern/googletest/googletest/include -Iextern/googletest/googletest
INC += -Iextern/googletest/googlemock/include -Iextern/googletest/googlemock

GTEST_SRCS := extern/googletest/googlemock/src/gmock-all.cc
GTEST_SRCS += extern/googletest/googletest/src/gtest-all.cc
GTEST_SRCS += extern/googletest/googletest/src/gtest_main.cc

TEST_OBJS += $(patsubst %.cc,out/%.o,$(GTEST_SRCS))
