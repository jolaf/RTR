# RTR
My student CGI project from 1996 that I've recently discovered in my archives.

## Historical version

The historical version can be found in [`historical`](https://github.com/jolaf/RTR/tree/historical) branch. It's almost "as is", but contains a pair of modern scripts to convert the original 52-frame 320x200 rendering from original custom format to modern [APNG](https://raw.githubusercontent.com/jolaf/RTR/historical/VESAFILM.PNG) and [MP4](https://raw.githubusercontent.com/jolaf/RTR/historical/VESAFILM-H264.mp4) formats.

![Historical rendering](https://raw.githubusercontent.com/jolaf/RTR/historical/VESAFILM.PNG)

## Modern version

The [`master`](https://github.com/jolaf/RTR) branch contains recently updated code that upscales the rendering to 208-frame 1600x1200 and also strips all the functionality that's difficult to observe now on modern DOS emulators like [VirtualBox](https://www.virtualbox.org) and [DOSBox](https://www.dosbox.com).

In particular, the following things were removed: `Show` mode, on-screen rendering, 8-bit grayscale mode, custom rendering modes, RLE data format, command line options, all obsolete code. The modern version only accepts a single parameter – output file name, and all it's doing is rendering a complete 208-frame 1600x1200 scene in 24-bit color mode.

The output `.sht` file then can be converted to APNG with [`Convert.py`](https://github.com/jolaf/RTR/blob/master/Convert.py) and then to MP4 with [`Convert.cmd`](https://github.com/jolaf/RTR/blob/master/Convert.cmd), which produces two files, [`RTR-H264.mp4`](https://raw.githubusercontent.com/jolaf/RTR/master/RTR-H264.mp4) and [`RTR-H265.mp4`](https://raw.githubusercontent.com/jolaf/RTR/master/RTR-H265.mp4). The latter is lossless, but is not supported everywhere. The former is lossy because of `4:2:0` color encoding, and contains visible [banding](https://en.wikipedia.org/wiki/Colour_banding), but is widely supported. Lossless `4:4:4` encoding for H.264 is unfortunately not supported by most players.

Full-scale APNG is too large to commit, so here's a single frame rendering, use `*.mp4` links above for full scale animation:

![Modern rendering](https://raw.githubusercontent.com/jolaf/RTR/master/RTR.png)
