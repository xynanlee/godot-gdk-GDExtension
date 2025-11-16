#!/usr/bin/env python
import os
import sys

from methods import print_error


libname = "GDK"
projectdir = "demo"

localEnv = Environment(tools=["default"], PLATFORM="")

# Build profiles can be used to decrease compile times.
# You can either specify "disabled_classes", OR
# explicitly specify "enabled_classes" which disables all other classes.
# Modify the example file as needed and uncomment the line below or
# manually specify the build_profile parameter when running SCons.

# localEnv["build_profile"] = "build_profile.json"

customs = ["custom.py"]
customs = [os.path.abspath(path) for path in customs]

opts = Variables(customs, ARGUMENTS)
opts.Update(localEnv)

Help(opts.GenerateHelpText(localEnv))

env = localEnv.Clone()

if not (os.path.isdir("godot-cpp") and os.listdir("godot-cpp")):
    print_error("""godot-cpp is not available within this folder, as Git submodules haven't been initialized.
Run the following command to download godot-cpp:

    git submodule update --init --recursive""")
    sys.exit(1)

env = SConscript("godot-cpp/SConstruct", {"env": env, "customs": customs})

env.Append(CPPPATH=[
    "src/",
    "C:/Program Files (x86)/Microsoft GDK/250403/GRDK/GameKit/Include",
    "C:/Program Files (x86)/Microsoft GDK/250403/GRDK/ExtensionLibraries/Xbox.Services.API.C/Include",
    "C:/Program Files (x86)/Microsoft GDK/250403/GRDK/ExtensionLibraries/Xbox.LibHttpClient/Include",
    "C:/Program Files (x86)/Microsoft GDK/250403/GRDK/ExtensionLibraries/Xbox.Services.API.C/Include/Xal"
    ])
sources = Glob("src/*.cpp")

env.Append(LIBPATH=[
    "C:/Program Files (x86)/Microsoft GDK/250403/GRDK/GameKit/Lib/amd64",
    "C:/Program Files (x86)/Microsoft GDK/250403/GRDK/GameKit/Lib",
    "C:/Program Files (x86)/Microsoft GDK/250403/GRDK/ExtensionLibraries/Xbox.Services.API.C/Lib/x64/Release",
    "C:/Program Files (x86)/Microsoft GDK/250403/GRDK/ExtensionLibraries/Xbox.Services.API.C/Lib/x64/Release/v142",
    "C:/Program Files (x86)/Microsoft GDK/250403/GRDK/ExtensionLibraries/Xbox.LibHttpClient/Lib/x64"
])
env.Append(LINKFLAGS=["xgameruntime.thunks.lib", "Microsoft.Xbox.Services.GDK.C.Thunks.lib"])
env.Append(LINKFLAGS=[
    "bcrypt.lib", "user32.lib", "kernel32.lib", "advapi32.lib", "libHttpClient.GDK.lib", "appnotify.lib"
])

env.Append(CPPDEFINES=["HC_PLATFORM=HC_PLATFORM_GDK", "HC_DATAMODEL=HC_DATAMODEL_LP64"])

if env["target"] in ["editor", "template_debug"]:
    try:
        doc_data = env.GodotCPPDocData("src/gen/doc_data.gen.cpp", source=Glob("doc_classes/*.xml"))
        sources.append(doc_data)
    except AttributeError:
        print("Not including class reference as we're targeting a pre-4.3 baseline.")

# .dev doesn't inhibit compatibility, so we don't need to key it.
# .universal just means "compatible with all relevant arches" so we don't need to key it.
suffix = env['suffix'].replace(".dev", "").replace(".universal", "")

lib_filename = "{}{}{}{}".format(env.subst('$SHLIBPREFIX'), libname, suffix, env.subst('$SHLIBSUFFIX'))

library = env.SharedLibrary(
    "bin/{}/{}".format(env['platform'], lib_filename),
    source=sources,
)

copy = env.Install("{}/bin/{}/".format(projectdir, env["platform"]), library)

default_args = [library, copy]
Default(*default_args)
