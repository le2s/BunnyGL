RootDir=`pwd`
echo "Creating Release from $RootDir"

echo "Deleting old release directory" 
rm -fr ~/VTKDesigner2_Release
cd $RootDir

echo "Creating release directory structure"
mkdir ~/VTKDesigner2_Release/
mkdir ~/VTKDesigner2_Release/Qt
mkdir ~/VTKDesigner2_Release/VTK5
mkdir ~/VTKDesigner2_Release/VTKD2
mkdir ~/VTKDesigner2_Release/VTKD2/components
mkdir ~/VTKDesigner2_Release/VTKD2/guixml
mkdir ~/VTKDesigner2_Release/VTKD2/Samples
mkdir ~/VTKDesigner2_Release/VTKD2/Data
cd $RootDir

echo "Copying VTKDesigner2 binaries to release directory"
cd bin
cp -varf --preserve links lib*.so* ~/VTKDesigner2_Release/VTKD2
cp VTKDesigner2 ~/VTKDesigner2_Release/VTKD2
cd $RootDir/bin/guixml
cp * ~/VTKDesigner2_Release/VTKD2/guixml
cd $RootDir/bin/Samples
cp * ~/VTKDesigner2_Release/VTKD2/Samples
cd $RootDir/bin/Data
cp -varf * ~/VTKDesigner2_Release/VTKD2/Data
cd $RootDir

echo "Copying Qt binaries to release directory"
cd /usr/local/Trolltech/Qt-4.4.0/lib
cp -varf --preserve links libQtCore.so.* ~/VTKDesigner2_Release/Qt
cp -varf --preserve links libQtDBus.so.* ~/VTKDesigner2_Release/Qt
cp -varf --preserve links libQtGui.so.* ~/VTKDesigner2_Release/Qt
cp -varf --preserve links libQtHelp.so.* ~/VTKDesigner2_Release/Qt
cp -varf --preserve links libQtNetwork.so.* ~/VTKDesigner2_Release/Qt
cp -varf --preserve links libQtOpenGL.so.* ~/VTKDesigner2_Release/Qt
cp -varf --preserve links libQtScript.so.* ~/VTKDesigner2_Release/Qt
cp -varf --preserve links libQtSvg.so.* ~/VTKDesigner2_Release/Qt
cp -varf --preserve links libQtXml.so.* ~/VTKDesigner2_Release/Qt
cp -varf --preserve links libQtDesigner.so.* ~/VTKDesigner2_Release/Qt
cp -varf --preserve links libQtDesignerComponents.so.* ~/VTKDesigner2_Release/Qt
cd ~/VTKDesigner2_Release/Qt
rm -fr *.debug
cd $RootDir

echo "Copying VTK binaries to release directory"
cd VTK5/bin
cp -varf --preserve links lib*.so* ~/VTKDesigner2_Release/VTK5
cd $RootDir

echo "Writing VTKDesigner2 Launcher script"
echo "export LD_LIBRARY_PATH=\`pwd\`/Qt:\`pwd\`/VTK5:\`pwd\`/VTKD2:\$LD_LIBRARY_PATH" > ~/VTKDesigner2_Release/VTKDesigner2
echo "\`pwd\`/VTKD2/VTKDesigner2 2> /dev/null" >> ~/VTKDesigner2_Release/VTKDesigner2
chmod a+x ~/VTKDesigner2_Release/VTKDesigner2
cd $RootDir

FileName="VTKDesigner2_`date +%a_%b_%d_%Y`.tar.gz"
echo "Creating a compressed binary release in ~/$FileName"
cd ~
tar -czvf $FileName VTKDesigner2_Release
cd $RootDir

echo "Release directory can now be found at ~/VTKDesigner2_Release"
echo "Release compressed binary can now be found at ~/$FileName"





