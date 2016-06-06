############################################################################
##                                                                        ##
##   Global Makefile                                                      ##
##   version 1.0                                                          ##
############################################################################

# empty target to prevent accidental use
.phony: 
	@echo "make desine"

# Directories
BASE_DIR=/home/deogun/ssamal/DesineMaster
DOC_DIR=$(BASE_DIR)/doc
SRC_DIR=$(BASE_DIR)/src
OBJ_DIR=$(SRC_DIR)
TST_DIR=$(BASE_DIR)/test
LIB_DIR=$(BASE_DIR)/lib

# PACKAGES is a list of all packages
PACKAGES=Algorithm LinkCostFunction LinkStateUpdate Main Network Parameter RandomVariables IO Utils prf samcra

# Local compiler flags/directives
LOCALCFLAGS=-std=gnu++11 -fno-strict-aliasing
LOCALCXXFLAGS=-std=gnu++11 -fno-strict-aliasing

# Compiler and flags
CXX=g++
CC=gcc

# Debug directives
#DEBUGS=-Wall -DNO_TIMER -DNO_TRACES #-DNO_THROUGHPUT -DNO_STD_DEV
DEBUGS=-Wall -DNO_TIMER #-DNO_THROUGHPUT -DNO_STD_DEV

# Includes and libraries
#ISUBDIRS=$(PACKAGES:%=-I$(SRC_DIR)/%)
ISUBDIRS=-I$(SRC_DIR)
TMP_PKGS=$(PACKAGES:Main=) # exclude Main
ALL_OBJS=$(TMP_PKGS:%=$(OBJ_DIR)/%/*.o)

# Optimization Flags
#FLAGS=-O3 -funroll-loops -finline-functions -fexpensive-optimizations
FLAGS=-g -funroll-loops -finline-functions -fexpensive-optimizations

# Compiler settings
CFLAGS=$(FLAGS) $(LOCALCFLAGS)
CXXFLAGS=$(DEBUGS) $(FLAGS) $(ISUBDIRS) $(LOCALCXXFLAGS)

# Linker settings
LXXFLAGS=$(DEBUGS) $(FLAGS) $(ISUBDIRS) -L$(OBJ_DIR) -L$(LIB_DIR) -lm