{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
	nativeBuildInputs = with pkgs; [
		gcc
		gdb
		pkgconfig
		SDL2
	];
}
