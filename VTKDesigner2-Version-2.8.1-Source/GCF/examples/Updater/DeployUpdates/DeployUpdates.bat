@ECHO OFF

mkdir Update1
copy /Y ..\Update1\Binary\Install-Windows.js Update1\Install.js
copy /Y ..\Update1\Binary\SnapshotCapturerComponent.xml Update1\
copy /Y ..\Update1\Binary\Update1.dll Update1\
copy /Y ..\Update1\Binary\Update1d.dll Update1\
..\..\..\bin\CreateGVZ -i Update1 -o Update1-Win32.gvz
move /Y Update1-Win32.gvz ..\SimpleBrowser\ServerSite
del /Q Update1\*
rmdir Update1

mkdir Update2
copy /Y ..\Update2\Binary\Install-Windows.js Update2\Install.js
copy /Y ..\Update2\Binary\BookmarkComponent.xml Update2\
copy /Y ..\Update2\Binary\Update2.dll Update2\
copy /Y ..\Update2\Binary\Update2d.dll Update2\
..\..\..\bin\CreateGVZ -i Update2 -o Update2-Win32.gvz
move /Y Update2-Win32.gvz ..\SimpleBrowser\ServerSite
del /Q Update2\*
rmdir Update2

ECHO Deployed Update1 and Update2 to ../SimpleBrowser/ServerSite
