############################################################################
##                                                                        ##
##   Makefile for Algorithm                                               ##
##   version 1.0                                                          ##
############################################################################

MOD=Algorithms
# Objects used for this class-group
ifeq ($(BASE_DIR),)
	include ../../src/makefile.inc
endif

OBJ=Algorithm.o DijkstraAlgorithm.o WSDijkstraAlgorithm.o NewMIRAAlgorithm.o SMIRAAlgorithm.o CellAlgorithm.o AbstractShortestPathAlgorithm.o SamcraAfterAlgorithm.o SamcraBeforeAlgorithm.o SamcraBAlgorithm.o TEDBAlgorithm.o SlimNetAlgorithm.o BCubeAlgorithm.o FatTreeAlgorithm.o AlgorithmFactory.o


$(MOD) : cd AR 
all : AR

cd :
	@cd $(OBJ_DIR)/$(MOD)

%.o: $(SRC_DIR)/$(MOD)/%.cpp $(SRC_DIR)/$(MOD)/%.h
	$(CXX) $(CXXFLAGS) -o $*.o -c $(SRC_DIR)/$(MOD)/$*.cpp

AR: $(OBJ)
	ar cr $(BASE_DIR)/lib/lib$(MOD).a *.o
clean: 
	rm -rf $(OBJ_DIR)/$(MOD)/*.o
