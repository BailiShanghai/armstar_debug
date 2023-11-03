@echo off
setlocal enabledelayedexpansion

set file_all=all.bin
set file_spe=bk7236_spe.bin
set file_nspe=bk7236_nspe.bin
set file_pad=pad.bin

set file_spe_size=0
set file_pad_size=0
set file_nspe_start=0x20000

rem 更新 bin
del %file_all%
del %file_spe%
del %file_nspe%
del %file_pad%
copy ..\bk7236_spe\output\Objects\bk7236_spe.bin .
copy ..\bk7236_spe\output\Objects\bk7236_spe.elf .
copy ..\bk7236_nspe\output\Objects\bk7236_nspe.bin .
copy ..\bk7236_nspe\output\Objects\bk7236_nspe.elf .

rem 创建 pad.bin
for %%F in ("!cd!\%file_spe%") do (
    set "file_spe_size=%%~zF"
)
echo "%file_spe%" size %file_spe_size%
set /a file_pad_size=file_nspe_start - file_spe_size
echo "%file_pad%" size %file_pad_size%
fsutil file createnew %file_pad% %file_pad_size%

rem 生成 all.bin
copy /B %file_spe% + /B %file_pad% + /B %file_nspe% %file_all%

rem 清除临时 bin
del %file_pad%

encrypt.exe %file_all% 00000000
encrypt.exe %file_spe% 00000000

endlocal