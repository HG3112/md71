md71

Mux - Demux (e)ac3 streams

This script is designed as a tool for 7.1 eac3 audio file

** W A R N I N G **

This script is in development/test stage. Please check outputs carefully. Use at your own risk.

USAGE:

md71 inputFileA inputFileB outputFileA outputFileB dialn_value compr_flags chanmap_flags dialn2_value compr2_flags

md71 script take ac3/eac3 inputFileA and inputFileB and creates two outputs with the corrensponding flags/values changed, recalculating crc for each frame. It also display statistics, and header info for first frame in inputs.

If inputFileA==inputFileB , the two input streams are taken from the same file assuming interleaved frames. The same holds for outputs. This is useful e.g. to mux/demux 7.1 eac3 files, which usually consist of an eac3/ac3 independent 5.1 core and a eac3 dependent 4.0 substream with surround speakers. See examples.

NOTES:

- The script checks for syncword at the beginning of each inputfile frame. No further checking is performed on inputs.

- If a parameter is set to ":" it will be ignored and left unchanged. If not all arguments are specified, the unspecifyed ones will be ignored.

- If a flag/value is not found in input frames, it WILL NOT be inserted. The script can only changes existing flags and values.

- Flags and values change applies to both inputs. Note that dialn2 and compr2 exist only in streams with acmod==0 (dual mono mode). The suffix "2" DOES NOT refer to the second input, but to the second channel.

- If the input contain more than two substreams, the script is quite useless for demux (it can still be used to change values and flags to all frames)

- dynrng and dynrng2 flags change is still not supported

VALUES AND FLAGS:

Refer to https://www.atsc.org/atsc-documents/a522012-digital-audio-compression-ac-3-e-ac-3-standard-12172012/

- dialn , dialn2 : use one or two decimal digits in the range 0-31 or 00-31. Suggested: 31

- compr_flags , compr2_flags : use a two hex string. Suggested: 00 (+0 dB) - See section 7.7.2 in reference.

- chanmap_flags : use a four hex string. Suggested: 1A00 (standard 7.1) - See Table E2.5 in reference. Note that chanmap can be present only in dependent streams.

------------------------------------

EXAMPLES:

rem display info for input (first frame and statistics)

md71 input.ac3

---------------------

rem display info for inputA and inputB (first frame and statistics)

md71 inputA.ac3 inputB.eac3

---------------------

rem display info for input (first and second frames and statistics). Note that if the number of frames is odd the script returns error.

md71 input.eac3 input.eac3 

---------------------

rem Demux input71.eac3 into core and dependent stream, no change

md71 input71.eac3 input71.eac3 output51_core.(e)ac3 output40_dependent.eac3

---------------------

rem Demux input71.eac3, only dependent stream, change dialn to 31 and compr to 0B

md71 input71.eac3 input71.eac3 : output40_dependent.eac3 31 0B

rem Only the second in each two frames is taken from input

---------------------

rem Demux input71.eac3, only core independent stream, change compr to A3

md71 input71.eac3 input71.eac3 output51_core.(e)ac3 : : A3

rem Only the first in each two frames is taken from input

---------------------

rem Mux input51 and input40 into output71 , change dialn to 25 and chanmap to 1A00

md71 input51_core.(e)ac3 input40_dependent.eac3 output71.eac3 output71.eac3 25 : 1A00

---------------------

rem Change channel map to all frames in input, no mux/demux

md71 input.eac3 : output.eac3 : : : 1A00

------------------------------------

ADVANCED EXAMPLE (experimental)

If your eac3-7.1 encoder does not support blu-ray compliant encodes, you can use ffmpeg to create an ac3 core. ffmpeg does not support the encoding into dependent streams, so we can use it only for 5.1 eac3. In this example we use an external eac3-7.1 encoder to get a 4.0 dependent stream, while using ffmpeg for the core. For simplicity, we use the standard ffmpeg 7.1-->5.1 downmix, which perhaps is not optimal for any input. We also set dialn to 31, compr to 00 and chanmpa to 1A00.

rem Script begin

eac3_71_encoder input71.waw tmp_output71.eac3

md71 tmp_output71.eac3 tmp_output71.eac3 : tmp_output40.eac3

ffmpeg -bitexact -i input71.wav -ac 6 -c:a ac3 -b:a 640k -bitexact tmp_output51.ac3

md71 tmp_output51.ac3 tmp_output40.eac3 output71.eac3 output71.eac3 31 00 1A00







