#SConstruct

fileList=Split("""bin/main.cpp
                  bin/helper.cpp
                  bin/IniParser.cpp
                  bin/StringParser.cpp
                  bin/ID3Reader.cpp
                  bin/TagReader.cpp
                  bin/OggReader.cpp
                  bin/Sqlite3.cpp
                  bin/Sqlite3Result.cpp""")

BuildDir('bin', 'src', duplicate = 0)

env = Environment(CC        = "g++",
                  CCFLAGS   = "-O2 -march=pentium4 -pipe -Wall `taglib-config --cflags`",
                  LINKFLAGS = "`taglib-config --libs`")

### configuration stuff
taglibVersion = 1.4

def CheckTagLib(context, version):
  result = 1

  context.Message('Checking for taglib >= %s ... ' % version)

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

conf = Configure(env, custom_tests = { 'CheckTagLib' : CheckTagLib })

if not conf.CheckTagLib(taglibVersion):
  print "Couldn't find taglib (>= %s) configuration tool. Exiting." % taglibVersion
  Exit(1)

if not conf.CheckLib("sqlite3"):
  print "Couldn't find sqlite3 library. Exiting."
  Exit(1)

env = conf.Finish()
### end configuration stuff

env.Append(CCFLAGS = " -g")
env.Program("bin/plow", Split(fileList))
#import sys
#print sys
#for f, w in sys.items():
#	print f
#	print w

