@echo on

ffmpeg -r 30 -i RTR-Render.png -c:v libx264 -pix_fmt yuv420p -preset veryslow -crf 17 -tune zerolatency -movflags +faststart -y RTR-H264.mp4

;REM ffmpeg -r 30 -i RTR-Render.png -c:v libx264 -pix_fmt yuv444p -profile:v high444 -preset veryslow -crf 17 -tune zerolatency -movflags +faststart -y RTR-H264-444.mp4

ffmpeg -r 30 -i RTR-Render.png -c:v libx265 -preset veryslow -x265-params lossless=1 -tune zerolatency -movflags +faststart -y RTR-H265.mp4
