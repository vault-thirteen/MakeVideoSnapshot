# MakeVideoSnapshot
Video Snapshot Maker

A simple Library to make Video Snapshots.

For now it only makes a single Snapshot of the first Video Frame of the best Video Stream of the File. 

An installed Version of the `LibAVCodec` is required to use this Tool.

Some useful instructions for project building using the Linux O.S. Shell:
```shell
sudo apt-get -y install autoconf automake build-essential libass-dev libfreetype6-dev libsdl1.2-dev libtheora-dev libtool libva-dev libvdpau-dev libvorbis-dev libxcb1-dev libxcb-shm0-dev libxcb-xfixes0-dev pkg-config texi2html zlib1g-dev
sudo apt install -y libavdevice-dev libavfilter-dev libswscale-dev libavcodec-dev libavformat-dev libswresample-dev libavutil-dev
sudo apt-get install yasm
```

Unfortunately the `LibAVCodec` Library does not have a good Support for the Windows O.S.

Before building a Project, get the 'stb' Library using the Shell Script `get-stb.sh`.
