#SConstruct

fileList=Split("""bin/main.cpp
                  bin/helper.cpp
		  bin/Plow.cpp
                  bin/PlowException.cpp
                  bin/IniParser.cpp
                  bin/StringParser.cpp
                  bin/AbstractReader.cpp
                  bin/ID3v2Reader.cpp
                  bin/ReaderSelector.cpp
                  bin/VorbisReader.cpp
                  bin/Sqlite3.cpp""")

BuildDir("bin", "src", duplicate = 0)

CXXFLAGS=[]

if ARGUMENTS.get("cxxflags", 0):
  CXXFLAGS=ARGUMENTS.get("cxxflags").split()

if ARGUMENTS.get("version", 0):
  CXXFLAGS.append('-DVERSION="\\"' + ARGUMENTS.get("version",0 ) + '\\""')

if ARGUMENTS.get("debug", 0):
  CXXFLAGS.append("-g")

CXXFLAGS.append("-Wall")
CXXFLAGS.append("-ansi")
CXXFLAGS.append("`taglib-config --cflags`")

env = Environment(CC        = "g++",
                  CCFLAGS   = CXXFLAGS,
                  LINKFLAGS = "`taglib-config --libs`")

### configuration stuff
taglibVersion = 1.4

def CheckTagLib(context, version):
  result = 1

  context.Message("Checking for taglib >= %s ... " % version)

  import os
  try:
    fp = os.popen("taglib-config --version")
    _version = float(fp.readline())
    fp.close()
  except:
    _version = -1

  if _version < version:
    result = 0

  context.Result(result)
  return result

conf = Configure(env, custom_tests = { "CheckTagLib" : CheckTagLib })

if not conf.CheckTagLib(taglibVersion):
  print "Couldn't find taglib (>= %s) configuration tool. Exiting." % taglibVersion
  Exit(1)

if not conf.CheckLib("sqlite3"):
  print "Couldn't find sqlite3 library. Exiting."
  Exit(1)

env = conf.Finish()
### end configuration stuff

env.Program("bin/plow", Split(fileList))
