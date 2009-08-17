# change installation directories below ###################################
ifeq ($(prefix),)
	prefix = /usr/local
endif
exec_prefix = $(prefix)
datarootdir = $(prefix)/share
bindir      = $(exec_prefix)/bin
mandir      = $(datarootdir)/man
man1dir     = $(mandir)/man1
# #########################################################################
ifeq ($(shell pkg-config --version 2>/dev/null),)
	export ERR = $(error pkg-config not found)
endif
ifeq ($(shell pkg-config --atleast-version=1.4 taglib && echo "1"),)
	export ERR = $(error taglib >= 1.4 not found)
endif
ifeq ($(shell pkg-config --atleast-version=3.3 sqlite3 && echo "1"),)
	export ERR = $(error sqlite3 not found)
endif
# #########################################################################
INSTALL = install
RM      = -rm -f
RMDIR   = -rmdir
MKDIR   = mkdir -p
STRIP   = strip --strip-unneeded -R .comment
# #########################################################################
ifeq ($(version),)
	version = svn-$(shell svn info 2>/dev/null |\
              grep "Revision" 2>/dev/null |\
              tr -d "[:alpha:][:space:]:" 2>/dev/null)
	ifeq ($(version), svn-)
		version = "UNKNOWN VERSION"
	endif
endif

CXXFLAGS += $(shell pkg-config --cflags sqlite3)
CXXFLAGS += $(shell pkg-config --cflags taglib)
CXXFLAGS += -Wall -DVERSION=\"$(version)\"
LIBS      = $(shell pkg-config --libs sqlite3)
LIBS     += $(shell pkg-config --libs taglib)
LIBDIRS   =
LDFLAGS   = $(LIBDIRS) $(LIBS)
# #########################################################################
OBJS = bin/PlowException.o bin/helper.o \
       bin/StringParser.o bin/IniParser.o \
       bin/AbstractReader.o bin/ID3v2Reader.o \
       bin/VorbisReader.o bin/ReaderSelector.o \
       bin/Sqlite3.o bin/Plow.o bin/main.o

HEADS = src/constants.h src/types.h
# #########################################################################
.PHONY: all
all: bin/plow

.PHONY: strip
strip: bin/plow
	@echo " [STRIP]     bin/plow"
	@$(STRIP) bin/plow

bin/plow: bin/ $(OBJS)
	@echo " [LD]        $@"
	@$(CXX) -o $@ $(OBJS) $(LDFLAGS)

bin/%.o: src/%.h src/%.cpp $(HEADS)
	@echo " [CXX]       $@"
	@$(CXX) $(CXXFLAGS) -c -o $@ src/$*.cpp

bin/%.o: src/%.cpp $(HEADS)
	@echo " [CXX]       $@"
	@$(CXX) $(CXXFLAGS) -c -o $@ src/$*.cpp

bin/:
	@$(MKDIR) ./bin/

.PHONY: install
install: all
	@$(INSTALL) -d $(bindir)
	@echo " [INSTALL]   $(bindir)/plow"
	@$(INSTALL) bin/plow $(bindir)
	@$(INSTALL) -d $(man1dir)
	@echo " [INSTALL]   $(man1dir)/plow.1"
	@$(INSTALL) plow.1 $(man1dir)

.PHONY: uninstall
uninstall:
	@echo " [UNINSTALL] $(bindir)/plow"
	@$(RM) $(bindir)/plow
	@echo " [UNINSTALL] $(bindir)"
	@$(RMDIR) $(bindir)
	@echo " [UNINSTALL] $(man1dir)/plow.1"
	@$(RM) $(man1dir)/plow.1
	@echo " [UNINSTALL] $(man1dir)"
	@$(RMDIR) $(man1dir)

.PHONY: clean
clean:
	$(RM) ./bin/*

# remove editor backup files
.PHONY: cleanbak
cleanbak:
	$(RM) ./*~
	$(RM) ./*.bak
	$(RM) ./src/*~
	$(RM) ./src/*.bak

.PHONY: cleanall
cleanall: cleanbak clean
	$(RMDIR) ./bin/

