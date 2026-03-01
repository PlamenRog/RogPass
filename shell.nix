{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  packages = [
    pkgs.gcc
    pkgs.openssl
    pkgs.pkg-config
  ];
}
