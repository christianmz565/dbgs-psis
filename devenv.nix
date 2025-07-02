{
  pkgs,
  lib,
  config,
  inputs,
  ...
}:

{
  packages = with pkgs; [
    gcc
    cmake
    stdenv.cc.cc.lib
    doxygen
    texlive.combined.scheme-full
  ];
}
