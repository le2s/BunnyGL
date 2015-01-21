mkdir Update1
cp -f ../Update1/Binary/Install-Linux.js Update1/Install.js
cp -f ../Update1/Binary/SnapshotCapturerComponent.xml Update1
cp -f ../Update1/Binary/libUpdate1.so.1.0.0 Update1/libUpdate1.so
cp -f ../Update1/Binary/libUpdate1d.so.1.0.0 Update1/libUpdate1d.so
../../../bin/CreateGVZ -i Update1 -o Update1-Lin32.gvz
mv Update1-Lin32.gvz ../SimpleBrowser/ServerSite
rm -fr Update1

mkdir Update2
cp -f ../Update2/Binary/Install-Linux.js Update2/Install.js
cp -f ../Update2/Binary/BookmarkComponent.xml Update2
cp -f ../Update2/Binary/libUpdate2.so.1.0.0 Update2/libUpdate2.so
cp -f ../Update2/Binary/libUpdate2d.so.1.0.0 Update2/libUpdate2d.so
../../../bin/CreateGVZ -i Update2 -o Update2-Lin32.gvz
mv Update2-Lin32.gvz ../SimpleBrowser/ServerSite
rm -fr Update2

echo Deployed Update1 and Update2 to ../SimpleBrowser/ServerSite

