# change installation directories below ###################################
PREFIX         = /usr/local
INSTALLBINDIR  = ${PREFIX}/bin
INSTALLMANDIR  = ${PREFIX}/man/man1
# #########################################################################
CPP            = g++
INSTALL        = install
RM             = -rm -f
RMDIR          = -rmdir
MKDIR          = mkdir -p
CPPFLAGS       = -Wall -ansi `taglib-config --cflags` ${CXXFLAGS}
LIBS           = -lsqlite3
LIBDIRS        =
LDFLAGS        = ${LIBDIRS} ${LIBS} `taglib-config --libs`

OBJS = bin/helper.o bin/PlowException.o \
       bin/Sqlite3Result.o bin/Sqlite3.o \
       bin/IniParser.o bin/StringParser.o \
       bin/TagReader.o bin/AbstractReader.o \
       bin/VorbisReader.o bin/ID3v2Reader.o

all: bin/plow

bin/plow: bin/ ${OBJS} src/main.cpp
	${CPP} ${CPPFLAGS} -o $@ ${OBJS} src/main.cpp ${LDFLAGS}

bin/%.o: src/%.h src/%.cpp
	${CPP} ${CPPFLAGS} -c -o $@ src/$*.cpp

bin/:
	${MKDIR} ./bin/

install: bin/plow
	${INSTALL} -d ${INSTALLBINDIR}
	${INSTALL} bin/plow ${INSTALLBINDIR}
	${INSTALL} -d ${INSTALLMANDIR}
	${INSTALL} plow.1 ${INSTALLMANDIR}

uninstall:
	${RM} ${INSTALLBINDIR}/plow
	${RMDIR} ${INSTALLBINDIR}
	${RM} ${INSTALLMANDIR}/plow.1
	${RMDIR} ${INSTALLMANDIR}
	
clean:
	${RM} ./bin/*

# remove editor backup files
cleanbak:
	${RM} ./*~
	${RM} ./*.bak
	${RM} ./src/*~
	${RM} ./src/*.bak

cleanall: cleanbak clean
	${RMDIR} ./bin/

