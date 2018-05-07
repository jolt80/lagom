# Set up compile environment 

# Stop echo of command lines
Q:=@

# Use V=1 for verbose outpout
ifdef V
Q:=
endif

# Check that we have decently new version of g++ in env (>=5) 
GCC_VERSION_GTEQ_5 := $(shell expr `$(CXX) -dumpversion | cut -f1 -d.` \>= 5)
ifneq "$(GCC_VERSION_GTEQ_5)" "1"
$(error Update gcc toolchain in env to version 5.x.x or greater, your toolchain in env is too old: $(shell $(CXX) -dumpversion))
endif

# Add correct path to clib to get the binary to work in Ericssons old redhat VDI env
ifdef LMR_SITE
LMR_SITE_CLIB_PATH = $(subst /bin/,,$(dir $(shell which $(CXX))))/lib64
LIBS += -ltinfo -L$(LMR_SITE_CLIB_PATH) -Wl,-rpath,$(LMR_SITE_CLIB_PATH)
endif

# Output path
OUT := $(CURDIR)/out

ifndef V
REDIRECT := > /dev/null 2>&1
endif
