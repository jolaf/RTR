#!/usr/bin/python3

from itertools import chain
from os.path import join
from struct import calcsize, unpack
from sys import argv
from tempfile import TemporaryDirectory
from typing import List

try:
    from pygame.image import frombuffer, save # type: ignore[import]
    print()
except ImportError as ex:
    raise ImportError(f"{type(ex).__name__}: {ex}\n\nPlease install PyGame v1.9.6 or later: http://pygame.org\n")

try:
    from apng import APNG # type: ignore[import]
except ImportError as ex:
    raise ImportError(f"{type(ex).__name__}: {ex}\n\nPlease install apng v0.3.3 or later: https://pypi.org/project/apng\n")

HEADER_FORMAT = 'cccciii'
HEADER_LENGTH = calcsize(HEADER_FORMAT)
assert HEADER_LENGTH == 16

VGA_PALETTE_SIZE = 256 * 3

WIDTH = 320
HEIGHT = 200

FPS = 30

def main(shtfileName: str) -> None:
    with open(shtfileName, 'rb') as f:
        data = f.read()
    (bits, rle, _mode, _direction, nFrames, _start, _reserved) = unpack(HEADER_FORMAT, data[:HEADER_LENGTH])
    nBits = int(bits[0])
    isRLE = bool(rle[0])
    print(f"{shtfileName}: {nBits}-bit {'RLE' if isRLE else 'Uncompressed'} {nFrames} frames")
    assert nBits in (8, 24)
    assert not isRLE, "RLE format is not supported, use -n when generating .sht file"
    startPosition = HEADER_LENGTH + (VGA_PALETTE_SIZE if nBits == 8 else 0)
    pixelSize = 1 if nBits == 8 else 3
    frameSize = WIDTH * HEIGHT * pixelSize
    assert len(data) == startPosition + frameSize * nFrames
    fileNamePrefix = shtfileName[:shtfileName.rindex('.')]
    with TemporaryDirectory(prefix = 'rtr') as tempDir:
        fileNameTemplate = join(tempDir, f'{fileNamePrefix}-%08d.png')
        fileNames: List[str] = []
        index = startPosition
        for nFrame in range(1, nFrames + 1):
            newIndex = index + frameSize
            frame = data[index : newIndex]
            index = newIndex
            if nBits == 8:
                frame = bytes(chain.from_iterable(zip(frame, frame, frame)))
            surface = frombuffer(frame, (WIDTH, HEIGHT), 'RGB')
            fileName = fileNameTemplate % nFrame
            save(surface, fileName)
            fileNames.append(fileName)
            print(nFrame)
        apngFileName = f'{fileNamePrefix}.png'
        print(f"Creating APNG {apngFileName}...")
        APNG.from_files(fileNames, delay = int(1000.0 / FPS)).save(apngFileName)

if __name__ == '__main__':
    main(argv[1])
