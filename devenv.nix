{
  pkgs,
  lib,
  config,
  inputs,
  ...
}:

{
  packages = with pkgs; [

  ];

  languages.cplusplus.enable = true;
}
