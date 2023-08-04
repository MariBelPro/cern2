# Define C++ compiler
CXX = g++

# Define linker
LINKER = g++

# Debug flag: either nothing or -ggdb
DEBUGFLAG =

# Opitmization flag
OPTFLAG = -O2

# ROOT flag: -D__ROOT__
ROOTFLAG =

# Define C++ flags (-D_VANILLA_ROOT_ is needed to avoid StMessMgr confusion)
CXXFLAGS = $(shell root-config --cflags) -I. -I${FEMTODST} -I${STEPDUTIL} $(OPTFLAG) -fPIC -Wall -pipe -std=c++11
CXXFLAGS += -D_VANILLA_ROOT_ $(DEBUGFLAG)

# Define libraries to be added
LIBS = $(shell root-config --libs)
LIBS += -L${FEMTODST} -lStFemtoDst
LIBS += -L${STEPDUTIL} -lStEpdUtil

# Define path to include directories
INCS = $(shell root-config --incdir)

# Linker flags //  -D__ROOT__
LDFLAGS = $(shell root-config --cflags) -I. -I${FEMTODST} -I${STEPDUTIL} $(OPTFLAG) -fPIC -Wall -pipe -D_VANILLA_ROOT_

all: epd_eta epd v1 FemtoDstAnalizer epd_pt run_analisis Raw Recentring Flat v1_TPC v2_TPC


# Compile C++ files
epd_eta: epd_eta.o
	$(LINKER) $< $(LDFLAGS) $(LIBS) $(GLIBS) -o $@
epd: epd.o
	$(LINKER) $< $(LDFLAGS) $(LIBS) $(GLIBS) -o $@
v1: v1.o
	$(LINKER) $< $(LDFLAGS) $(LIBS) $(GLIBS) -o $@
FemtoDstAnalizer: FemtoDstAnalizer.o
	$(LINKER) $< $(LDFLAGS) $(LIBS) $(GLIBS) -o $@
epd_pt: epd_pt.o
	$(LINKER) $< $(LDFLAGS) $(LIBS) $(GLIBS) -o $@
run_analisis: run_analisis.o
	$(LINKER) $< $(LDFLAGS) $(LIBS) $(GLIBS) -o $@
Raw: Raw.o
	$(LINKER) $< $(LDFLAGS) $(LIBS) $(GLIBS) -o $@
Recentring: Recentring.o
	$(LINKER) $< $(LDFLAGS) $(LIBS) $(GLIBS) -o $@	
Flat: Flat.o
	$(LINKER) $< $(LDFLAGS) $(LIBS) $(GLIBS) -o $@
v1_TPC: v1_TPC.o
	$(LINKER) $< $(LDFLAGS) $(LIBS) $(GLIBS) -o $@
v2_TPC: v2_TPC.o
	$(LINKER) $< $(LDFLAGS) $(LIBS) $(GLIBS) -o $@		
# Compile C++ files
%.o: %.C
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	@echo 'Compiling: $@ $<'

# PHONY options
.PHONY: clean distclean

# Remove object files and dictionary
clean:
	rm -vf *.o

# Remove object files, dictionary and library
distclean:
	rm -vf *.o epd_eta epd v1 FemtoDstAnalizer epd_pt run_analisis Raw Recentring Flat v1_TPC v2_TPC 
