#!/usr/bin/env python3
from setuptools import setup, Extension

pog = Extension('pog',
	define_macros = [
		# -D_REENTRANT
		("_REENTRANT", None)
	],
	sources = [
		'src/pog.c',
	],
	include_dirs = [
		# pkg-config --cflags sdl2
		'/nix/store/bgc3rvghyjdby7lw94hkjjgwv1d20zp6-SDL2-2.0.20-dev/include/SDL2',
	],
	extra_link_args = [
		# pkg-config --libs sdl2
		'-L/nix/store/536n1g1ca44v2q3c21809wz8ff602lmr-SDL2-2.0.20/lib',
		'-Wl,-rpath,/nix/store/536n1g1ca44v2q3c21809wz8ff602lmr-SDL2-2.0.20/lib',
		'-Wl,--enable-new-dtags',
		'-lSDL2'
	])

setup(name = 'pog2D',
	version = '0.0.8',
	description = 'A simple 2D library',
	ext_modules = [pog])
