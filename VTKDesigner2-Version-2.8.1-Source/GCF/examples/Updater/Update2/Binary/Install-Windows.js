function DoInstall()
{
    System.Progress(0, "Installing Bookmark component");
    FileSystem.Copy("Update2.dll", "${APPDIR}/Update2.dll");
    FileSystem.Copy("Update2d.dll", "${APPDIR}/Update2d.dll");

    System.Progress(50, "Configuring Bookmark component");
    FileSystem.Copy("BookmarkComponent.xml", "${GUIXMLDIR}/BookmarkComponent.xml");

    System.Progress(95, "Registering the update");
    System.RegisterComponent("BookmarkComponent", "Update2");

    System.Progress(100, "Finished installing update to Bookmark component");

    return true;
}
