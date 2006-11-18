PREFIX        = /usr/local
INSTALLBINDIR = ${PREFIX}/bin
INSTALLDIR    = ${PREFIX}/share/plow

CPP       = g++
CPPFLAGS  = -Wall -ansi `taglib-config --cflags`

LIBS      = -lsqlite3
LIBDIRS   =
LDFLAGS   = $(LIBDIRS) $(LIBS) `taglib-config --libs`

OBJS      = bin/helper.o\
            bin/Sqlite3Result.o bin/Sqlite3.o \
            bin/IniParser.o bin/StringParser.o \
            bin/TagReader.o bin/OggReader.o bin/ID3Reader.o

plow: ${OBJS} src/main.cpp
	${CPP} ${CPPFLAGS} -o bin/$@ ${OBJS} src/main.cpp ${LDFLAGS}

bin/%.o : src/%.h src/%.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ src/$*.cpp

cleanbak:          # remove editor backup files
	rm -f   ./*~
	rm -f   ./*.bak
	rm -f src/*~
	rm -f src/*.bak
	rm -f sql/*~
	rm -f sql/*.bak

clean:
	-/usr/bin/rm -f bin/*

cleanall: cleanbak clean

install: #plow sql/plow.struct.no_id.sql sql/plow.struct.sql plowrc
	install -d ${INSTALLDIR}
	install -d ${INSTALLDIR}/sql
	install -d ${INSTALLBINDIR}
	install bin/plow ${INSTALLBINDIR}
	install plowrc ${INSTALLDIR}
	install sql/plow.struct.no_id.sql ${INSTALLDIR}/sql
	install sql/plow.struct.sql ${INSTALLDIR}/sql
	sqlite3 ${INSTALLDIR}/plow.db < sql/plow.struct.no_id.sql
#	sqlite3 ${INSTALLDIR}/plow.db < sql/plow.struct.sql

uninstall:
	rm -rf ${INSTALLDIR}
	rm -f ${INSTALLBINDIR}/plow