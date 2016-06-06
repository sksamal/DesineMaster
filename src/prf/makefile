############################################################################
##                                                                        ##
##   Makefile for Algorithm                                               ##
##   version 1.0                                                          ##
############################################################################

MOD=prf
ifeq ($(BASE_DIR),)
	include ../../src/makefile.inc
endif
# Objects used for this class-group

OBJ=h_prf_t.o

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
