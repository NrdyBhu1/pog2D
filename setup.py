#!/usr/bin/env python3
from setuptools import setup, Extension
from subprocess import getstatusoutput
from sys import exit

libs = "sdl2"
cflags = getstatusoutput(f"pkg-config --cflags {libs}")
ldflags = getstatusoutput(f"pkg-config --libs {libs}")
if cflags[0] != 0 or ldflags[0] != 0:
	print("Please install libraries properly!")
	exit(1)

pog = Extension('pog',
	define_macros = [
		("_REENTRANT", None)
	],
	sources = [
		'src/pog.c',
	],
	include_dirs = [
		# pkg-config --cflags sdl2
        "src/include",
		cflags[1]
	],
	extra_link_args = [
		# pkg-config --libs sdl2
		ldflags[1]
		# '-L/nix/store/536n1g1ca44v2q3c21809wz8ff602lmr-SDL2-2.0.20/lib',
		# '-Wl,-rpath,/nix/store/536n1g1ca44v2q3c21809wz8ff602lmr-SDL2-2.0.20/lib',
		# '-Wl,--enable-new-dtags',
		# '-lSDL2'
	])

setup(name = 'pog2D',
	version = '0.0.8',
	description = 'A simple 2D library',
	license='mit',
	ext_modules = [pog])
