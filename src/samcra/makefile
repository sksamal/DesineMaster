############################################################################
##                                                                        ##
##   Makefile for Algorithm                                               ##
##   version 1.0                                                          ##
############################################################################

MOD=samcra
ifeq ($(BASE_DIR),)
	include ../../src/makefile.inc
endif
# Objects used for this class-group

OBJ=Fibonacci_mod.o linked_list_mod.o samcra_mod.o samcra2_mod.o samcra3_mod.o tamcra_mod.o samcra_util.o

$(MOD) : cd AR 
all : AR

%.o: $(SRC_DIR)/$(MOD)/%.c $(SRC_DIR)/$(MOD)/%.h
	$(CC) $(CFLAGS) -o $*.o -c $(SRC_DIR)/$(MOD)/$*.c

cd :
	@cd $(OBJ_DIR)/$(MOD)

AR: $(OBJ)
	ar cr $(BASE_DIR)/lib/lib$(MOD).a *.o

clean: 
	rm -rf *.o
