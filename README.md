YALP-MTA v0.1
==========

This plugin is an extension of [_YALP_](//github.com/IllidanS4/YALP/) allowing import of [MTA modules](https://wiki.multitheftauto.com/wiki/Modules) for use from within Lua.

Since MTA uses Lua 5.1 while YALP uses Lua 5.3, you'll also need something like [this](//github.com/IllidanS4/luawrapper/) to actually load any MTA module.

## Installation
Download the latest [release](//github.com/IllidanS4/YALP-MTA/releases/latest) for your platform to the "plugins/lua" directory and add "lua/YALP-MTA" (or "lua/YALP-MTA.so" on Linux) to the `plugins` line in server.cfg *after* YALP.

_YALP_ must be at least version 1.1 in order to use this plugin!

## Building
Use Visual Studio to build the project on Windows, or `make` on Linux.

## Usage

Simply call `require "YALP-MTA"`, which will modify the package loader table to allow importing modules from the "plugins/mta" directory. Global `require` function must be present and unmodified.

This example uses the [SHA](//github.com/mabako/mta-sha/) module:
```lua
require "YALP-MTA"
local sha = require "sha" -- will check plugins/mta/

for k, v in pairs(sha) do
  print(k, v)
end

print(sha.sha256("abc"))
```

In case the `package` table is accessible, `package.mpath` can be modified to change the directory where the modules are located.