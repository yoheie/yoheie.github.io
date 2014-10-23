@echo off
rem ############################################################################
rem #
rem # isodatetime.cmd - print date and time in ISO8601 format
rem #
rem # version 1.5
rem #
rem # Copyright (c) 2010-2012 Yohei Endo <yoheie@gmail.com>
rem #
rem # This software is provided 'as-is', without any express or implied
rem # warranty. In no event will the authors be held liable for any damages
rem # arising from the use of this software.
rem #
rem # Permission is granted to anyone to use this software for any purpose,
rem # including commercial applications, and to alter it and redistribute it
rem # freely, subject to the following restrictions:
rem #
rem #   1. The origin of this software must not be misrepresented; you must not
rem #      claim that you wrote the original software. If you use this software
rem #      in a product, an acknowledgment in the product documentation would be
rem #      appreciated but is not required.
rem #
rem #   2. Altered source versions must be plainly marked as such, and must not
rem #      be misrepresented as being the original software.
rem #
rem #   3. This notice may not be removed or altered from any source
rem #      distribution.
rem #
rem ############################################################################

setlocal

set EXTENDED=0
set FRACTIONAL=0

rem # Parameter check ##########################################################

:prm
if    "%1" == ""   (goto main)
if    "%1" == "/?" (goto help)
if /i "%1" == "/h" (goto help)
if /i "%1" == "-h" (goto help)
if /i "%1" == "/e" (set EXTENDED=1 & goto next)
if /i "%1" == "-e" (set EXTENDED=1 & goto next)
if /i "%1" == "/f" (set FRACTIONAL=1 & goto next)
if /i "%1" == "-f" (set FRACTIONAL=1 & goto next)
goto help
:next
shift
goto prm

rem # Main #####################################################################

:main

set ISODATETIME=%DATE:/=-%T%TIME: =0%

if      %FRACTIONAL% == 1 (set ISODATETIME=%ISODATETIME:.=,%)   ^
else                      (set ISODATETIME=%ISODATETIME:~0,19%)

if      %EXTENDED%   == 0 (set ISODATETIME=%ISODATETIME:-=%)
if      %EXTENDED%   == 0 (set ISODATETIME=%ISODATETIME::=%)

echo %ISODATETIME%

goto end

rem # Print help ###############################################################

:help

echo usage : isodatetime [options]
echo options : /h, /? : show this help
echo           /e     : extended format
echo           /f     : add fractional part of a second

goto end

rem # Exit #####################################################################

:end

endlocal
