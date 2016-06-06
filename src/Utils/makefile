############################################################################
##                                                                        ##
##   Makefile for Algorithm                                               ##
##   version 1.0                                                          ##
############################################################################

MOD=Utils
ifeq ($(BASE_DIR),)
	include ../../src/makefile.inc
endif
# Objects used for this class-group

OBJ=Util.o TraceManager.o

$(MOD) : cd AR 
all : AR

%.o: $(SRC_DIR)/$(MOD)/%.cpp $(SRC_DIR)/$(MOD)/%.h
	$(CXX) $(CXXFLAGS) -o $*.o -c $(SRC_DIR)/$(MOD)/$*.cpp

cd :
	@cd $(OBJ_DIR)/$(MOD)

AR: $(OBJ)
	ar cr $(BASE_DIR)/lib/lib$(MOD).a *.o
clean: 
	rm -rf *.o
