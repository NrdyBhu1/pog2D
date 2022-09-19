#!/usr/bin/env python3
from setuptools import setup, Extension

pog = Extension('pog',
	define_macros = [
		("_REENTRANT", None)
	],
	sources = [
		'src/pog.c',
	],
	include_dirs = [
		'/nix/store/bgc3rvghyjdby7lw94hkjjgwv1d20zp6-SDL2-2.0.20-dev/include/SDL2',
	],
	extra_link_args = [
		'-L/nix/store/536n1g1ca44v2q3c21809wz8ff602lmr-SDL2-2.0.20/lib',
		'-Wl,-rpath,/nix/store/536n1g1ca44v2q3c21809wz8ff602lmr-SDL2-2.0.20/lib',
		'-Wl,--enable-new-dtags',
		'-lSDL2'
	])

setup(name = 'pog',
	version = '0.0.6-dev',
	description = 'A simple 2D library',
	ext_modules = [pog])
