# OpenFNaF
A (WiP) Open-Source Reimplementation of Scott Cawthon's Five Nights at Freddy's. Written in C. Licensed under MIT.

# In It's Current State
Part of the first [Five Night's at Freddy's](https://store.steampowered.com/app/319510/Five_Nights_at_Freddys/) title has been implemented. The OpenFNaF engine will initialize a game window, and the game code will load the player into the office room. The room will pan with the player's cursor, and there is a camera button that will enter the WiP camera system. Animatronics will roam around, althought this is not yet shown visually.

# Building
OpenFNaF requires the following additional libraries to build:
* [libcsfml-dev](https://www.sfml-dev.org/) (zlib/png license)

Simply run `make`; if provided all additional libraries OpenFNaF should build without issue. Object files will be stored in the created `build` directory, an executable will be located in the root directory of the project.

OpenFNaF also supports `make clean`.

# Running
## Command Line Arguments
At this time, OpenFNaF supports the following arguments on launch:
* `-no-render`: Disable Window and Renderer.
* `-verbose`: Enable printing/console logs. Note that verbose mode's usefulness can vary depending on the Developer(s) willingness to use and/or the frequency of use.
# Regarding Copyright
Under no circumstances will licensed assets be provided on this GitHub repository. This includes, but is not limited to, sounds and graphics. The Five Nights at Freddy's IP belongs to it's rightholders, Clickteam LLC and Scott Cawthon.
