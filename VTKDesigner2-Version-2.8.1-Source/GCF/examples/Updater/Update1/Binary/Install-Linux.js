function DoInstall()
{
    System.Progress(0, "Installing SnapshotCapturer component");
    FileSystem.Copy("libUpdate1.so", "${APPDIR}/libUpdate1.so");
	FileSystem.Copy("libUpdate1d.so", "${APPDIR}/libUpdate1d.so");

    System.Progress(50, "Configuring SnapshotCapturer component");
    FileSystem.Copy("SnapshotCapturerComponent.xml", "${GUIXMLDIR}/SnapshotCapturerComponent.xml");

    System.Progress(95, "Registering the update");
    System.RegisterComponent("SnapshotCapturerComponent", "Update1");

    System.Progress(100, "Finished installing update to SnapshotCapturer component");
    
    return true;
}
