![](https://runelite.net/img/logo.png)
# launcher [![Discord](https://img.shields.io/discord/301497432909414422.svg)](https://discord.gg/mePCs8U)

This repository holds the launcher for the [RuneLite client](https://github.com/runelite/runelite).

If you have any questions, please join our [Discord](https://discord.gg/mePCs8U) server or alternatively our IRC channel on [irc.rizon.net #runelite](http://qchat.rizon.net/?channels=runelite&uio=d4).

### License

RuneLite is [licensed under the BSD 2-clause license](https://github.com/runelite/launcher/blob/master/LICENSE).

# BUILD INSTRUCTIONS
Initial setup:
- Install visual studio with the c++ stuff
- Clone detours to ``%userprofile%/source/repos``
- Then you have to go ``tools`` > ``developer command line`` in visual studio with the c++ package installed because for some reason normal cmd does not work...
- cd to ``detours/src``
- ``nmake``
- For me this only generated ``%userprofile%\source\repos\detours\lib.X86\detours.lib``
- Run ``VsDevCmd.bat -host_arch=amd64 -arch=amd64`` still in the developer command line
- ``nmake``
- Copy those two lib folders to ``launcher-fork\detours\``
- Install innosetup 6.2.2 and add it to your path

Always:
- run mvn install so it builds to target
- run ``build_win86/64.sh`` with git bash by double clicking on it
- Output can be found in the ``build`` folder. These are the files that normally get installed to ``AppData\Local\RuneLite`` The setup itself can be found in ``launcher-fork\RuneLiteSetup.exe``