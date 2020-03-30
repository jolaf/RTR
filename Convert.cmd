@echo on
ffmpeg -r 30 -i VESAFILM.PNG -c:v libx264 -pix_fmt yuv420p -preset veryslow -crf 17 -tune zerolatency -movflags +faststart -y VESAFILM-H264.mp4
