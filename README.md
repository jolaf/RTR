# RTR
My student [CGI](https://en.wikipedia.org/wiki/Computer-generated_imagery) project from 1996 that I've recently discovered in my archives.

It turns out it is the only reasonable piece of code I've ever written on C++.

The code implements pure, non-optimized [ray tracing](https://en.wikipedia.org/wiki/Ray_tracing_(graphics)) with lighting and shadows.

The code is pure C++ and is **NOT** using any 3-rd party libraries, graphics engines, [DirectX](https://en.wikipedia.org/wiki/DirectX) or [OpenGL](https://www.opengl.org).

The code is compiled using [Watcom C++](https://en.wikipedia.org/wiki/Watcom_C/C%2B%2B) with [DOS/4GW](https://en.wikipedia.org/wiki/DOS/4G) to be run under [DOS](https://en.wikipedia.org/wiki/DOS) operating system.

The scene contains 9 fixed [infinite planes](https://en.wikipedia.org/wiki/Infinite_plane), 20 moving [tetrahedrons](https://en.wikipedia.org/wiki/Tetrahedron) and a single moving [point light source](https://en.wikipedia.org/wiki/Point_light_source).

The animation originally contained 52 frames arranged for a looped playback, for the modern version it was modified for 208 frames for smoother playback.

Historically the program supported rendering and displaying rendered videos in two video modes:
* VGA mode `13h`: 320x200 with 256-color grayscale palette
* VESA mode `10Fh`: 320x200 24-bit True Color

This resolution is currently too low and the aspect ratio is not 1:1, so for modern publishing the resolution was increased to 1600x1200.

## Historical version

The historical version can be found in [`historical`](https://github.com/jolaf/RTR/tree/historical) branch. It's almost "as is", but contains a pair of modern scripts to convert the original 52-frame 320x200 animation from original custom format to modern [APNG](https://en.wikipedia.org/wiki/APNG) ([VESAFILM.png](https://raw.githubusercontent.com/jolaf/RTR/historical/VESAFILM.png)) and [MP4](https://en.wikipedia.org/wiki/MP4) ([VESAFILM-H264.mp4](https://raw.githubusercontent.com/jolaf/RTR/historical/VESAFILM-H264.mp4)) formats.

![Historical rendering](https://raw.githubusercontent.com/jolaf/RTR/historical/VESAFILM.png)

## Modern version

The [`master`](https://github.com/jolaf/RTR) branch contains recently updated code that upscales the rendering to 208-frame 1600x1200 and also strips all the functionality that's difficult to observe now on modern DOS emulators like [VirtualBox](https://www.virtualbox.org) and [DOSBox](https://www.dosbox.com).

In particular, the following things were removed: `Show` mode, on-screen rendering, 8-bit grayscale mode, custom rendering modes, RLE data format, command line options, all obsolete code. The modern version only accepts a single parameter – output file name, and all it's doing is rendering a complete 208-frame 1600x1200 scene in 24-bit color mode.

The output `.sht` file can be converted to [APNG](https://en.wikipedia.org/wiki/APNG) format with [`Convert.py`](https://github.com/jolaf/RTR/blob/master/Convert.py) and then to [MP4](https://en.wikipedia.org/wiki/MP4) with [`Convert.cmd`](https://github.com/jolaf/RTR/blob/master/Convert.cmd), which produces two files, [`RTR-H264.mp4`](https://raw.githubusercontent.com/jolaf/RTR/master/RTR-H264.mp4) and [`RTR-H265.mp4`](https://raw.githubusercontent.com/jolaf/RTR/master/RTR-H265.mp4). The latter is lossless, but is not supported everywhere. The former is lossy because of [`4:2:0`](https://en.wikipedia.org/wiki/4:2:0) color encoding, and contains visible [banding](https://en.wikipedia.org/wiki/Colour_banding), but is widely supported. Lossless [`4:4:4`](https://en.wikipedia.org/wiki/4:2:0) encoding for [H.264](https://en.wikipedia.org/wiki/H.264) is unfortunately not supported by most players.

![Modern rendering](https://raw.githubusercontent.com/jolaf/RTR/master/RTR.png)
