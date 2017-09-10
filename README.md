# ZelPac

Zelpac is a very simple game of Pacman made with Qt, coming with a level editor.
For for information about the project itself, you will find a Doxygen documentation with it.

## Table of contents
[Content](#content)  
[Requirements](#requirements)  
[Build instructions](#build-instructions)  

## Content
1) A pacman game

Ghosts behave the same way they did in the original version.
There is currenlty only one level available and no fruits.
You can play the game with your keyboard, most controllers and an home-made Arduino controller.
If you finish the level, expect some fancy fireworks ;)

Music: [Bensound - Happy rock](www.bensound.com)

![Alt text](/Illustrations/My_pacman.png)

2) A level editor for the pacman game (in French)
	- You can change the textures used for the tiles and for the dots
	- You can change difficulty (by changing Pacman and Ghosts' speed)
	- You can change timers used by ghosts (to switch between Dispersion and Chasing)
	
![Alt text](/Illustrations/My_pacman_editor.png)

## Requierements

- [Qt](https://www.qt.io/)
- [SFML](https://www.sfml-dev.org/download/sfml/2.4.2/index-fr.php)

## Build instructions

- In the Pacman project file (Pacman.pro), update the last four lines (depending on where you put the SFML folder) :
	- win32:CONFIG(debug, debug|release): LIBS += -Lpath/to/SMLF/lib -lsfml-system-d -lsfml-graphics-d -lsfml-window-d
	- else:win32:CONFIG(release, debug|release): LIBS += -Lpath/to/SMLF/lib -lsfml-system -lsfml-graphics -lsfml-window
	and
	- INCLUDEPATH += path/to/SFML/include
	- DEPENDPATH += path/to/SFML/include
	
- Put the SFML .dll in your build folder (from SFML\bin) before running the program
