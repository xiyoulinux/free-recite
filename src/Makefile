#############################################################################
# Makefile for building: FreeRecite
# Author1: Kermit Mei<kermit.mei@gmail.com>
# Author2: Jianjun Kong <kongjianjun@gmail.com>
# Template: app
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = 
CFLAGS        = -pipe -O2 -Wall -W $(DEFINES)
CXXFLAGS      = -pipe -O2 -Wall -W $(DEFINES)
INCPATH       = -I.
LINK          = g++
LFLAGS        = -Wl
LIBS          = $(SUBLIBS)   
AR            = ar cqs
RANLIB        = 
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -sf
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = Cui.cpp \
		Dict.cpp \
		DictItem.cpp \
		ForgetCurve.cpp \
		main.cpp \
		Manager.cpp \
		Reciter.cpp \
		Task.cpp \
		WordList.cpp 
OBJECTS       = Cui.o \
		Dict.o \
		DictItem.o \
		ForgetCurve.o \
		main.o \
		Manager.o \
		Reciter.o \
		Task.o \
		WordList.o

TARGET        = FreeRecite

####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

clean:
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core

####### Compile

Cui.o: Cui.cpp Cui.h \
		Manager.h \
		Task.h \
		Dict.h \
		DictItem.h \
		Reciter.h \
		WordList.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Cui.o Cui.cpp

Dict.o: Dict.cpp Dict.h \
		DictItem.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Dict.o Dict.cpp

DictItem.o: DictItem.cpp DictItem.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o DictItem.o DictItem.cpp

ForgetCurve.o: ForgetCurve.cpp ForgetCurve.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o ForgetCurve.o ForgetCurve.cpp

main.o: main.cpp Cui.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

Manager.o: Manager.cpp Manager.h \
		Task.h \
		ForgetCurve.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Manager.o Manager.cpp

Reciter.o: Reciter.cpp Reciter.h \
		WordList.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Reciter.o Reciter.cpp

Task.o: Task.cpp Task.h \
		ForgetCurve.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o Task.o Task.cpp

WordList.o: WordList.cpp WordList.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o WordList.o WordList.cpp

####### Install

install:   FORCE
	mkdir -p ~/.FreeRecite/
	cp -r ../dict/FreeRecite/* ~/.FreeRecite/
	cp -r ../doc ~/.FreeRecite/
	sudo install -m 755 ./FreeRecite /usr/bin/

uninstall:   FORCE
	rm -rf ~/.FreeRecite/
	sudo rm -rf /usr/bin/FreeRecite
FORCE:

