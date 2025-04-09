set "core=eac3"

set "bcore=960k"

set "bdepn=576k"

cd /D "%~dp0"

rem Create core (thanks to tebasuna51 for downmix command)
ffmpeg -bitexact -i %1 -filter_complex^
 "asplit [f][s]; [f] pan=3.1|c0=c0|c1=c1|c2=c2|c3=c3 [r]; [s] pan=stereo|c0=0.5*c4+0.5*c6|c1=0.5*c5+0.5*c7, compand=attacks=0:decays=0:points=-90/-84|-8/-2|-6/-1|-0/-0.1, aformat=channel_layouts=stereo [d]; [r][d] amerge [a]"^
 -map "[a]" -c:a %core% -b:a %bcore% -bitexact "tmp_%~n1_core.%core%"

rem Create dependent (swap channels)
ffmpeg_eac371 -bitexact -i %1 -af "pan=quad|c0=c6|c1=c7|c2=c4|c3=c5" -c:a eac3 -b:a %bdepn% -bitexact "tmp_%~n1_depn_%core%.eac3"

rem Mux core+dependent
md71 "tmp_%~n1_core.%core%" "tmp_%~n1_depn_%core%.eac3" "output_%~n1_%core%Core.eac3" "output_%~n1_%core%Core.eac3"

pause