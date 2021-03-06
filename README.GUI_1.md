
GUI_1  denotes a version with a GUI made by using QT widgets.

In the ".pro" file one may select (or deselect) input devices by uncommenting (commenting) the appropriate "CONFIG = XXX" lines.
A similar facility exists for the CMakeLists.txt file

In the script given below it is assumed that the only device supported is the dabstick. (Note that selecting a device requires installing the
library and the development files.)

unix {
CONFIG		+= dabstick
#CONFIG		+= sdrplay
#CONFIG		+= rtl_tcp
#CONFIG		+= airspy
#CONFIG		+= tcp-streamer		# use for remote listening
CONFIG		+= gui_1
DESTDIR		= ./linux-bin
INCLUDEPATH	+= /usr/local/include
LIBS		+= -lfftw3f  -lusb-1.0 -ldl  #
LIBS		+= -lportaudio
LIBS		+= -lz
LIBS		+= -lsndfile
LIBS		+= -lsamplerate
LIBS		+= -lfaad
}

Since an RPI is often run headless, an option is included to configure such that the PCM output is sent to a simple TCP server,
listening at port 20040. Uncomment the following line.

  ```
CONFIG += TCP_STREAMER will do here.
  ```

The sourcetree contains a directory "sound-client", that contains sources to generate a simple "listener". 

Note further that, whever you want to install the software on a non-intel box, your CPU might not support sse instructions.
In that case uncomment the line in the ".pro" file.

#CONFIG		+= NO_SSE_SUPPORT 

Ubuntu Linux
---
For generating an executable under Ubuntu, you can put the following
commands into a script (assuming you only support a dabstick as input device).

1. Fetch the required components
   #!/bin/bash
   sudo apt-get update
   sudo apt-get install qt4-qmake build-essential g++
   sudo apt-get install libsndfile1-dev qt4-default libfftw3-dev portaudio19-dev  libfaad-dev zlib1g-dev rtl-sdr libusb-1.0-0-dev mesa-common-dev libgl1-mesa-dev libqt4-opengl-dev libsamplerate-dev libqwt-dev
   cd
2. Fetch a version of the library for the dabstick
   # http://www.sm5bsz.com/linuxdsp/hware/rtlsdr/rtlsdr.htm
   wget http://sm5bsz.com/linuxdsp/hware/rtlsdr/rtl-sdr-linrad4.tbz
   tar xvfj rtl-sdr-linrad4.tbz 
   cd rtl-sdr-linrad4
   sudo autoconf
   sudo autoreconf -i
   ./configure --enable-driver-detach
   make
   sudo make install
   sudo ldconfig
   cd
3. Get a copy of the dab-rpi sources and use qmake
   git clone https://github.com/JvanKatwijk/dab-rpi.git
   cp dab-rpi.pro dab-rpi/
   cd dab-rpi
   qmake dab-rpi.pro
   make

