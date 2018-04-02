# Fragment that compiles glog from source

INC += -I$(OUT)/inc
LIBS += -L$(OUT)/lib -lglog

LIB_DEPS += $(OUT)/lib/libglog.a
$(OUT)/lib/libglog.a: $(OUT)/lib $(OUT)/inc/glog
	$(Q)cd extern/glog || exit 1;\
	echo [glog] autogen...;\
	./autogen.sh $(REDIRECT) || exit 1;\
	echo [glog] configure...;\
	./configure $(REDIRECT) || exit 1;\
	echo [glog] compile...;\
	make -j 8 $(REDIRECT) || exit 1; \
	cp .libs/libglog.a $(OUT)/lib || exit 1; \
	cp src/glog/*.h $(OUT)/inc/glog || exit 1; \
	git clean -fd $(REDIRECT) || exit 1; \
	git reset --hard $(REDIRECT) || exit 1; \
	echo [glog] done with $(OUT)/lib/libglog.a;\
	touch $(OUT)/lib/libglog.a

$(OUT)/inc/glog:
	$(Q)mkdir -p $(OUT)/inc/glog