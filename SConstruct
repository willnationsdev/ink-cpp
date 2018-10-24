#!python
import os

def add_sources(sources, directory):
    for file in os.listdir(directory):
        if file.endswith('.cpp'):
            sources.append(os.path.join(directory, file))

def add_sub_sources(sources, directory):
    for file in os.listdir(directory):
        if os.path.isdir(os.path.join(directory, file)):
            add_sources(sources, os.path.join(directory, file))

target = ARGUMENTS.get("target", "debug")
platform = ARGUMENTS.get("p", ARGUMENTS.get("platform", "linux"))

env = Environment()
if platform == "windows":
    env = Environment(ENV = os.environ)

env.Append(
    PLATFORM = platform,
    INCDIR = '.',
    CPPPATH = ['.', 'src', 'thirdparty'],
    LIBPATH = ['lib']
)

if ARGUMENTS.get("use_llvm", "no") == "yes":
    env["CXX"] = "clang++"

if platform == "osx":
    env.Append(CCFLAGS = ['-g','-O3', '-std=c++14', '-arch', 'x86_64'])
    env.Append(LINKFLAGS = ['-arch', 'x86_64', '-framework', 'Cocoa', '-Wl,-undefined,dynamic_lookup'])

if platform == "linux":
    env.Append(CCFLAGS = ['-fPIC', '-g','-O3', '-std=c++14'])

if platform == "windows":
    if target == "debug":
        env.Append(CCFLAGS = ['-EHsc', '-D_DEBUG', '/MDd'])
    else:
        env.Append(CCFLAGS = ['-O2', '-EHsc', '-DNDEBUG', '/MD'])

sources = []
add_sources(sources, "src")
#add_sub_sources(sources, "src") # unnecessary for now
add_sub_sources(sources, "thirdparty")

program = env.Program('main', sources)

Default(program)

