############################################################################
##                                                                        ##
##   Makefile for Algorithm                                               ##
##   version 1.0                                                          ##
############################################################################

MOD=Network
ifeq ($(BASE_DIR),)
	include ../../src/makefile.inc
endif
# Objects used for this class-group

OBJ=Cluster.o DegreeDistributionVisitor.o Flow.o FlowList.o FlowManager.o Link.o LinkList.o LinkListIterator.o LinkListMemento.o Topology.o TopologyFactory.o

$(MOD) : cd AR 
all : AR

%.o: $(SRC_DIR)/$(MOD)/%.cpp $(SRC_DIR)/$(MOD)/%.h TopologyFactory*.cpp TopologyFactory*.h
	$(CXX) $(CXXFLAGS) -o $*.o -c $(SRC_DIR)/$(MOD)/$*.cpp

cd :
	@cd $(OBJ_DIR)/$(MOD)

AR: $(OBJ)
	ar cr $(BASE_DIR)/lib/lib$(MOD).a *.o
clean: 
	rm -rf *.o
