# This is a quick install script for Debian systems

this script needs to be run from in the install folder. It will install all system and python3 dependencies, then it will update your branch with the latest taproot updates.

The firstthing to do is to download latest version of the arm-none-eabi toolchain.
In particular, download "gcc-arm-none-eabi-9-2020-q2-update-x86_64-linux.tar.bz2" from https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads

Make sure the tar.bz2 is in your downloads folder and the download is finished. 
Now clone your project and run the install.