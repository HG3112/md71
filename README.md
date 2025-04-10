Both ffmpeg_eac371 and md71 are released by HellGauss under the terms of the GNU LESSER GENERAL PUBLIC LICENSE Version 2.1 (See LICENSE file and readme txts)

USAGE

Drag and drop your 7.1 file into the preferred .bat script icon and wait for eac3 output in the script folder. Produced streams are either Bluray compliant (ac3Core) or full eac3 (eac3Core). Temp files are created in the same folder and can be cancelled at the end of the script.

Requires also a standard build of ffmpeg.exe (either installed or in the same directory)

The scripts are divided in three automatic stages:

- Downmix 7.1 to 5.1 and use ffmpeg to create the core (either ac3 or eac3)

- Extract the surround channels to create a 4ch dependent eac3 stream with ffmpeg_eac371

- Mix core and dependent streams for the final output with md71.exe

For advanced usage and options, see the attached readme txts.

Note: md71 and ffmpeg_eac371 are in test phase, please check outputs carefully