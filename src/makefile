############################################################################
##                                                                        ##
##   Makefile for Algorithm                                               ##
##   version 1.0                                                          ##
############################################################################

ifeq ($(BASE_DIR),)
	include ../src/makefile.inc
endif

# Objects used for this class-group
PACKAGES=Algorithms LinkCostFunction LinkStateUpdate Network Parameter RandomVariables IO Utils prf samcra Main/Desine

LIBS=-lUtils -lAlgorithms -lNetwork -lLinkCostFunction -lLinkStateUpdate -lParameter -lRandomVariables -lIO -lprf -lsamcra -lMain

all : desine 

desine: $(PACKAGES)
	$(foreach var,$(PACKAGES),cd $(OBJ_DIR)/$(var);make all;)
#	$(CXX) $(LXXFLAGS) $(ALL_OBJS) $(MAIN_DESINE_OBJ:%=$(OBJ_DIR)/%.o) -o desine
	$(CXX) $(LXXFLAGS) $(SRC_DIR)/Main/Desine/*.o $(LIBS) -o desine -pg

clean:
	-$(RM) -r $(OBJ_DIR)/desine

cleanall: clean
	-$(RM) -r $(OBJ_DIR)/*/*.o
	-$(RM) -r $(OBJ_DIR)/*/*/*.o
	-$(RM) -r $(LIB_DIR)/*.a

runfat: desine
	$(OBJ_DIR)/desine $(DOC_DIR)/parameters-fat4
runslim: desine
	$(OBJ_DIR)/desine $(DOC_DIR)/parameters-slimnet
runcell: desine
	$(OBJ_DIR)/desine $(DOC_DIR)/parameters-cell
