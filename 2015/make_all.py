#!/bin/python3

from os import scandir, system
from sys import argv

CC="gcc"
CXX="g++"
CFLAGS="-Wall -Wextra -pedantic -Werror -ggdb"
CXXFLAGS="-Wall -Wextra -pedantic -Werror -std=c++20 -ggdb"
# CFLAGS="-Wno-deprecated  -ggdb"
cpp = 0
c   = 0

def execute_command(command) -> None:
    code = system(command)
    if code & 0xf7:
        print("[CMD-ABORTED-BY-SIGNAL] ", command)
        exit(code);
    code = (code & 0xff00) >> 8
    if code != 0:
        print("[CMD-FAILED] ", command)
        exit(code)

def compile_one(name: str) -> None:
    c   = name.endswith(".c")
    cpp = name.endswith(".cpp")
    if c or cpp:
        bin_name = name.split(".")[0]
        out = f"./bin/{bin_name}"
        lib = ""
        if name == "04.c":
            lib = "-lm"
        if name == "07.c":
            lib = "./hashtable/HashTable.c"
        if cpp:
            command = f"{CXX} {CXXFLAGS} -o {out} {name} {lib}"
        else:
            command = f"{CC} {CFLAGS} -o {out} {name} {lib}"
        print(f"[BUILDING] {name} -> {out}")
        print(f"[CMD] {command}")
        execute_command(command)

def compile_all():
    print("[*] Making the bin directory.")
    system("mkdir -p bin")
    dir = scandir('.')
    for entry in dir: compile_one(entry.name)
def clean_all():
    print("rm -rf ./bin/")
    system("rm -rf ./bin/")
def main():
    subcommand = ""
    to_compile = ""
    if len(argv) > 1:
        if argv[1] != "re" and argv[1] != "clean":
            to_compile = argv[1]
        else:
            subcommand = argv[1]
    if to_compile: compile_one(to_compile)
    else:
        if subcommand == "":
            compile_all()
        elif subcommand == "re":
            clean_all()
            compile_all()
        elif subcommand == "clean":
            clean_all()
        else:
            print("Invalid subcommand: ", subcommand)
main()
