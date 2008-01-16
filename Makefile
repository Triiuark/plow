PREFIX         = /usr/local
INSTALLBINDIR  = ${PREFIX}/bin
INSTALLMANDIR  = ${PREFIX}/man/man1
CPP            = g++
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

bin/plow:  ${OBJS} src/main.cpp
	${CPP} ${CPPFLAGS} -o $@ ${OBJS} src/main.cpp ${LDFLAGS}

bin/%.o: src/%.h src/%.cpp
	mkdir -p ./bin/
	${CPP} ${CPPFLAGS} -c -o $@ src/$*.cpp

install: plow
	install -d ${INSTALLBINDIR}
	install bin/plow ${INSTALLBINDIR}
	install -d ${INSTALLMANDIR}
	install plow.1 ${INSTALLMANDIR}

uninstall:
	rm -f ${INSTALLBINDIR}/plow

clean:
	-rm -f  ./bin/*

# remove editor backup files
cleanbak:
	-rm -f   ./*~
	-rm -f   ./*.bak
	-rm -f ./src/*~
	-rm -f ./src/*.bak

cleanall: cleanbak clean
	-rm -rf ./bin/
