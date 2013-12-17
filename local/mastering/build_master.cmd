@echo off

SET ASK_USER=%1
SET PATH=..\..\library\buildtools\csscript\

CSharpScript.exe scripts/build_master.cse %ASK_USER%