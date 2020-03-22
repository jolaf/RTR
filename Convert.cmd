@echo on

python3 Convert.py VESA1600.SHT

python3 Convert.py VGA1600.SHT

ffmpeg -r 30 -i RTR-1600-Color.png -c:v libx264 -pix_fmt yuv444p -profile:v high444 -preset veryslow -crf 17 -tune zerolatency -movflags +faststart -y RTR-1600-Color-H264.mp4

# ffmpeg -r 30 -i RTR-1600-Color.png -c:v libx264 -pix_fmt yuv420p -preset veryslow -crf 17 -tune zerolatency -movflags +faststart -y RTR-1600-Color-H264.mp4
# ffmpeg -r 30 -i RTR-1600-BW.png    -c:v libx264 -pix_fmt gray    -preset veryslow -crf 17 -tune zerolatency -movflags +faststart -y RTR-1600-BW-H264.mp4 

# ffmpeg -r 30 -i RTR-1600-Color.png -c:v libx265 -preset veryslow -x265-params lossless=1 -tune zerolatency -movflags +faststart -y RTR-1600-Color-H265.mp4
# ffmpeg -r 30 -i RTR-1600-BW.png    -c:v libx265 -vf format=gray -preset veryslow -x265-params lossless=1 -tune zerolatency -movflags +faststart -y RTR-1600-BW-H265.mp4
