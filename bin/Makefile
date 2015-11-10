# Stop echo of command lines
Q=

# Search for source files in src
VPATH +=../src

# Search for include files in src
CPATH += ../src

CPPFLAGS+=-O3 -Wall -pedantic -fmessage-length=0

SRCS=$(wildcard ../src/*.cpp)
OBJS=$(notdir $(patsubst %.cpp,%.o,$(SRCS)))

RE2_LIB=../re2/obj/libre2.a

LIBS += -lcurses -pthread

# All Target
all: rcs_log_parser

print_debug:
	@echo "SRCS = "${SRCS}
	@echo "OBJS = "${OBJS}
	@echo "VPATH = "${VPATH}
	@echo "CPATH = "${CPATH}
	
# Tool invocations
rcs_log_parser: ${OBJS} ${RE2_LIB}
	${Q}echo 'Building target: $@'; \
	${Q}echo 'Invoking: GCC C++ Linker'; \
	${Q}g++ $@ ${LIBS} -o "RcsLogParser"; \
	${Q}echo 'Finished building target: $@'; \
	${Q}echo ' '

%.o: %.cpp
	${Q}echo 'Compiling: $<'; \
	${Q}g++ ${CPPFLAGS} -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
