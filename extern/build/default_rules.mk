# Set up compile environment 

ARTIFACTS += bin/$(APP_NAME)

compile: $(OBJS)

print:
	$(Q)echo $(TEST_SRCS)
	$(Q)echo $(OBJS)
	$(Q)echo $(TEST_OBJS)

bin:
	$(Q)mkdir -p bin

$(OUT)/lib:
	$(Q)mkdir -p $(OUT)/lib

$(OUT)/inc:
	$(Q)mkdir -p $(OUT)/inc
		
$(OUT)/%.o: %.cc
	$(Q)echo 'Compiling: $@'; \
	mkdir -p $(dir $@); \
	$(CXX) -MMD -MP $(INC) $(CPPFLAGS) -c -o $@ $<

$(OUT)/%.o: %.cpp
	$(Q)echo 'Compiling: $@'; \
	mkdir -p $(dir $@); \
	$(CXX) -MMD -MP $(INC) $(CPPFLAGS) -c -o $@ $<
	
clean:
	$(Q)$(RM) -f $(OBJS)
	$(Q)$(RM) -f $(TEST_OBJS)
	$(Q)$(RM) -f $(DEPS)
	$(Q)$(RM) $(ARTIFACTS)
	
realclean: clean
	$(Q)$(RM) -rf $(OUT)
	$(Q)$(RM) -rf bin
	$(Q)$(RM) $(ARTIFACTS)

lib-deps: $(LIB_DEPS)
	$(Q)echo Done.