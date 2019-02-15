# libunr #

libunr is a library that implements various functionalities of the original Unreal Engine 1 in a game/version agnostic way.

# Goals #

This library has two goals. 

The first goal is to provide a means for people to make content for all UE1 games with tools that
are modern and open source. This not only provides a platform for content creation that can be expanded upon with ease, but
a platform that can be run on a variety of operating systems, allowing many more people to participate properly in keeping
games like Unreal and UT99 alive for years to come. This goal can be realized by the use of years of knowledge surrounding
UnrealEd in the form of tutorials, documentation, and the collective experience of everyone in the community, past and present.

The second goal is to provide an engine that allows people to play all known UE1 games. Again, this provides a way for people to
continue playing these games on a wide variety of operating systems. As such, this library must support all previously made content,
both from official game developers and mod creators alike. 

While this goal allows the possibility of adding new features to games beyond what their original versions provide, this is not
something that will be pursued initially. It is desirable and necessary to support players and creators that wish to continue using 
official versions of various games until it is no longer feasible or reasonable to do so. This cooperation is necessary in order to
avoid dividing the community in half (i.e., Unreal 225/226 vs Unreal 227).

When it is no longer possible to run official versions of the game on modern hardware (at the time of this reading) or when content
creation has stagnated due to age of the engine, the addition of new features that would prevent a map or mod from working on said
version will be considered. This includes the case for Unreal if the 227 patch were to ever cease development indefinitely.

Although this project can be used to create entirely new games, new features that make it easier to create new games will not
ever be considered for this project. One is of course free to fork and add such changes as per the GPLv3 license, but the libunr 
project will remain focused on providing a platform for keeping legacy UE1 games alive.

In the future, these goals may be changed to include Unreal Engine 2. Support for Unreal Engine 3 is not likely to happen.
Support for Unreal Engine 4 will never happen as the code for UE4 is available for all to use.

# Code Guidelines #

Due to the largely proprietary nature of Unreal Engine 1, some guidelines for writing code are necessary. 

The use of public headers may be necessary to have a complete idea of what a particular class or component needs to know about to work 
at an abstract level, but copying and pasting any code from any other project, or use of any knowledge about how the engine works 
outside of abstract concepts is strictly forbidden. It is encouraged to think about the engine in terms of inputs and outputs 
(if I do <X>, what should the result <Y> be?)

It is also encouraged to think about how to reimplement various components with ideas from other projects that permit the
use of said ideas outside of that project. An example of this is Unreal Engine 4. While their EULA prohibits copying and pasting
code into other projects, the use of ideas that UE4 implements in other projects (including competing projects) is permitted.

# Building #

1) Install CMake 3.7 and git

2) Download/Build XSTL (eXtenstible Standard Template Library)
	- Currently, there are no binaries, pull from the repo and build here -> https://Xaleros@bitbucket.org/Xaleros/xstl.git

3) Pull this repo with the following command
	- git clone https://Xaleros@bitbucket.org/Xaleros/libunr.git

4) Run, in the root folder, the following command
	- cmake .
	- make

5) Install the library somewhere
	- for windows users, drag libxstl.dll and libunr.dll where ever your application is
	- for linux users, run "sudo make install" after

6) (optional) Download a program to use this library with
	- Libunr UCC (lucc) can be found here -> https://bitbucket.org/Xaleros/lucc/

# Installing #
	
TODO

# Licensing #
	GNU General Public License v3