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
  ];
}
