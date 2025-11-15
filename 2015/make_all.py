#!/bin/python3

from os import scandir, system

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

def main():
    print("[*] Making the bin directory.")
    system("mkdir -p bin")
    dir = scandir('.')
    for entry in dir:
        name = entry.name
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
main()
