<p align="center">
<img src="https://repository-images.githubusercontent.com/3925242/a4566200-912a-11ea-984f-c387546a3126">
</p>

[bgfx](https://github.com/bkaradzic/bgfx) - Cross-platform rendering library
============================================================================

<p align="center">
    <a href="#what-is-it">What is it?</a> -
    <a href="https://bkaradzic.github.io/bgfx/build.html">Building</a> -
    <a href="https://bkaradzic.github.io/bgfx/overview.html#getting-involved">Getting Involved</a> -
    <a href="https://bkaradzic.github.io/bgfx/examples.html">Examples</a> -
    <a href="https://bkaradzic.github.io/bgfx/bgfx.html">API Reference</a> -
    <a href="https://bkaradzic.github.io/bgfx/tools.html">Tools</a> -
    <a href="#who-is-using-it-madewithbgfx">Who is using it?</a> -
    <a href="#license-bsd-2-clause">License</a>
</p>

[![Build Status](https://travis-ci.com/bkaradzic/bgfx.svg?branch=master)](https://travis-ci.com/bkaradzic/bgfx)
[![Build status](https://ci.appveyor.com/api/projects/status/ipa3ojgeaet1oko5?svg=true)](https://ci.appveyor.com/project/bkaradzic/bgfx)
[![License](https://img.shields.io/badge/license-BSD--2%20clause-blue.svg)](https://bkaradzic.github.io/bgfx/license.html)
[![Join the chat at https://discord.gg/9eMbv7J](https://img.shields.io/discord/712512073522872352?color=%237289DA&label=bgfx&logo=discord&logoColor=white)](https://discord.gg/9eMbv7J)

 * [GitHub Discussions](https://github.com/bkaradzic/bgfx/discussions)
 * [Discord Chat](https://discord.gg/9eMbv7J)
 * [AppVeyor CI](https://ci.appveyor.com/project/bkaradzic/bgfx)
 * [Travis CI](https://travis-ci.com/bkaradzic/bgfx)

[What is it?](https://bkaradzic.github.io/bgfx/overview.html)
-------------------------------------------------------------

Cross-platform, graphics API agnostic, "Bring Your Own Engine/Framework" style
rendering library.

Supported rendering backends:

 * Direct3D 9
 * Direct3D 11
 * Direct3D 12
 * GNM (only for licensed PS4 developers, search DevNet forums for source)
 * Metal
 * OpenGL 2.1
 * OpenGL 3.1+
 * OpenGL ES 2
 * OpenGL ES 3.1
 * Vulkan
 * WebGL 1.0
 * WebGL 2.0
 * WebGPU/Dawn (experimental)

Supported platforms:

 * Android (14+, ARM, x86, MIPS)
 * asm.js/Emscripten (1.25.0)
 * FreeBSD
 * iOS (iPhone, iPad, AppleTV)
 * Linux
 * MIPS Creator CI20
 * OSX (10.12+)
 * PlayStation 4
 * RaspberryPi
 * Windows (XP, Vista, 7, 8, 10)
 * UWP (Universal Windows, Xbox One)

Supported compilers:

 * Clang 3.3 and above
 * GCC 5 and above
 * VS2017 and above

Languages:

 * [C/C++ API documentation](https://bkaradzic.github.io/bgfx/bgfx.html)
 * [Beef API bindings](https://github.com/bkaradzic/bgfx/tree/master/bindings/bf)
 * [C# language API bindings #1](https://github.com/bkaradzic/bgfx/tree/master/bindings/cs)
 * [C#/VB/F# language API bindings #2](https://github.com/MikePopoloski/SharpBgfx)
 * [D language API bindings](https://github.com/GoaLitiuM/bindbc-bgfx)
 * [Go language API bindings](https://github.com/james4k/go-bgfx)
 * [Haskell language API bindings](https://github.com/haskell-game/bgfx)
 * [Lightweight Java Game Library 3 bindings](https://github.com/LWJGL/lwjgl3)
 * [Lua language API bindings](https://github.com/cloudwu/lua-bgfx)
 * [Nim language API bindings](https://github.com/Halsys/nim-bgfx)
 * [Pascal language API bindings](https://github.com/Akira13641/PasBGFX)
 * [Python language API bindings #1](https://github.com/fbertola/bgfx-python#-----bgfx-python--)
 * [Python language API bindings #2](https://github.com/jnadro/pybgfx#pybgfx)
 * [Rust language API bindings (obsolete)](https://github.com/rhoot/bgfx-rs)
 * [Rust language API bindings (new)](https://github.com/emoon/bgfx-rs)
 * [Swift language API bindings](https://github.com/stuartcarnie/SwiftBGFX)







[License (BSD 2-clause)](https://bkaradzic.github.io/bgfx/license.html)
-----------------------------------------------------------------------

<a href="http://opensource.org/licenses/BSD-2-Clause" target="_blank">
<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">
</a>

	Copyright 2010-2021 Branimir Karadzic
	
	Redistribution and use in source and binary forms, with or without modification,
	are permitted provided that the following conditions are met:
	
	   1. Redistributions of source code must retain the above copyright notice, this
	      list of conditions and the following disclaimer.
	
	   2. Redistributions in binary form must reproduce the above copyright notice,
	      this list of conditions and the following disclaimer in the documentation
	      and/or other materials provided with the distribution.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
	BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
	OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
	OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
	OF THE POSSIBILITY OF SUCH DAMAGE.
