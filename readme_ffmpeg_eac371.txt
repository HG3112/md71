ffmpeg_eac371 has been built from ffmpeg 7.1.1 changing the files ac3enc.c and eac3enc.c in libavcodec folder

Built with msys2 with the following command line

CC=cl PKG_CONFIG_PATH=../../installed/lib/pkgconfig ../../sources/ffmpeg_eac371/configure --prefix=../../installed --toolchain=msvc --target-os=win64 --arch=x86_64 --enable-x86asm --enable-asm --disable-shared --disable-gpl --disable-nonfree --disable-openssl --enable-static --disable-network --enable-debug --extra-ldflags="-LIBPATH:../../installed/lib" --extra-cxxflags="-I../../installed/include/ -MTd -Od -Zi" --extra-cflags="-I../../installed/include/ -MTd -Od -Zi"

It produce eac3 outputs tagged as dependent, with compression set to 0x00 and with channelmap 0x1a00 (standard 7.1). Note that it is still in testing phase, please carefully check outputs.

-------------------

USAGE

It is suggested to use ffmpeg_eac371 only to produce dependent eac3 streams, e.g.

ffmpeg_eac371 -i "input_71.wav" -af "pan=quad|c0=c6|c1=c7|c2=c4|c3=c5" -c:a eac3 -b:a 384k "dependent_stream.eac3"

This extract the 4 surround channels from a 7.1 wav, rearrange channels as required by the channelmap, and encodes in a eac3 dependent 4ch stream.

Note that the output is unplayable, since it is tagged as dependent. It must be mixed with a core 5.1 independent stream to produce a full 7.1 output.