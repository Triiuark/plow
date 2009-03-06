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
TAGLIB_VERSION = `taglib-config --version`
CPPFLAGS       = -Wall `taglib-config --cflags` ${CXXFLAGS}\
                 -DTAGLIB_VERSION=${TAGLIB_VERSION}
LIBS           = -lsqlite3
LIBDIRS        =
LDFLAGS        = ${LIBDIRS} ${LIBS} `taglib-config --libs`
# #########################################################################
OBJS  = bin/PlowException.o bin/helper.o \
        bin/StringParser.o bin/IniParser.o \
        bin/AbstractReader.o bin/ID3v2Reader.o \
        bin/VorbisReader.o bin/ReaderSelector.o \
        bin/Sqlite3.o bin/Plow.o bin/main.o

HEADS = src/constants.h src/types.h

all: bin/plow

bin/plow: bin/ ${OBJS}
	@echo " [LD]  $@"
	@${CPP} -o $@ ${OBJS} ${LDFLAGS}

bin/%.o: src/%.h src/%.cpp ${HEADS}
	@echo " [CXX] $@"
	@${CPP} ${CPPFLAGS} -c -o $@ src/$*.cpp

bin/%.o: src/%.cpp ${HEADS}
	@echo " [CXX] $@"
	@${CPP} ${CPPFLAGS} -c -o $@ src/$*.cpp

bin/:
	@${MKDIR} ./bin/

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

