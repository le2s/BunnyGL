Qt Creator 2.0 and later provides support for creating custom project and/or class 
templates.

http://doc.qt.nokia.com/qtcreator-snapshot/creator-project-wizards.html

For GCF developers, we have bundled two project templates
1. GCF Application project template
2. GCF Component project template

These templates quickly generate projects for you to get started. However, a few 
minor modifications may be necessary based on your actual project.

To install these project templates you can copy/paste the gcfcomponent, gcfservercomponent
and gcfapplication directories found in this folder on to any of the following 
folders

1. share/qtcreator/templates/wizards
2. $HOME/.config/Nokia/qtcreator/templates/wizards
3. %APPDATA%\Nokia\qtcreator\templates\wizards

After copy/pasting the folders; restart Qt Creator. When you select File -> New
you will be able to see a new project category called "GCF Projects", within it
you can find "GCF Component" and "GCF Application" project types.
