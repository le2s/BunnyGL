function DoInstall()
{
    System.Progress(0, "Installing Bookmark component");
    FileSystem.Copy("libUpdate2.so", "${APPDIR}/libUpdate2.so");
	FileSystem.Copy("libUpdate2d.so", "${APPDIR}/libUpdate2.so");


    System.Progress(50, "Configuring Bookmark component");
    FileSystem.Copy("BookmarkComponent.xml", "${GUIXMLDIR}/BookmarkComponent.xml");

    System.Progress(95, "Registering the update");
    System.RegisterComponent("BookmarkComponent", "Update2");

    System.Progress(100, "Finished installing update to Bookmark component");

    
    return true;
}
