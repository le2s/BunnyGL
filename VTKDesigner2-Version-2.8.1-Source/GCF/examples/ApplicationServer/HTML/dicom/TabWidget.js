function TabWidget(objName) {
    this.objectName = objName;

    this.activeTabColor = '#000000';
    this.activeTabTextColor = '#ffffff';

    this.inactiveTabColor = '#aaaaaa';
    this.inactiveTabTextColor = '#000000';
    
    this.borderColor = '#000000';
    this.backgroundColor = '#ffffff';
    
    this.tabNames = new Array();
    this.tabFrames = new Array();
    this.tabDiv = false;
    this.tabBarRow = false;
    this.tabWidgetArea = false;
    this.activeTabIndex = -1;
    
    this.onCurrentTabIndexChanged = false;
    this.onCurrentTabNameChanged = false;
}

TabWidget.prototype.installOn = function(div) {
    if(!div || this.tabDiv)
        return;
        
    this.tabDiv = div;
    
    var tabDivHTML =
                '<table border="0" width="100%" height="100%" cellspacing="0" cellpadding="0">\n' +
                '<tr><td width="100%" height="30px" id="' + this.objectName + '_tabBar" align="center" valign="bottom"></td></tr>\n' +
                '<tr><td width="100%" id="' + this.objectName + '_tabWidgetArea" style="border: 1px; ' +
                'border-style: solid; padding: 10px; border-color: ' + this.borderColor + '; ' +
                'background-color: ' + this.backgroundColor + ';';
    tabDivHTML += '"></td></tr>\n</table>';

    this.tabDiv.innerHTML = tabDivHTML;
    this.tabBarRow = document.getElementById(this.objectName + '_tabBar');
    this.tabWidgetArea = document.getElementById(this.objectName + '_tabWidgetArea');
}

TabWidget.prototype.changeBackgroundImage = function(image) {
    var bgImage = "url('" + image.src + "')";

    var tabWidgetArea = document.getElementById(this.objectName + '_tabWidgetArea');
    tabWidgetArea.style.backgroundImage = bgImage;
    tabWidgetArea.style.backgroundPosition = "top left";
    tabWidgetArea.style.backgroundRepeat = "repeat";
}

TabWidget.prototype.addTab = function(name, source) {
    if( !this.tabDiv )
        return;

    // Store the tab name
    this.tabNames[ this.tabNames.length ] = name;
    
    // Create a tab frame
    var tabFrame = document.createElement('iframe');
    tabFrame.id = name + '_frame';
    tabFrame.setAttribute('width', '0px');
    tabFrame.setAttribute('height', '0px');
    tabFrame.setAttribute('src', source);
    tabFrame.setAttribute('frameborder', '0');
    tabFrame.setAttribute('marginheight', '5px');
    tabFrame.setAttribute('marginwidth', '5px');
    tabFrame.setAttribute('style', 'visibility: hidden; background-color: transparent;');
    if(source && source.length)
        tabFrame.setAttribute('givenSource', source);
    else
        tabFrame.setAttribute('givenSource', 'EmptyTab.html');
    this.tabWidgetArea.appendChild(tabFrame);
    
    // Store the tab frame
    this.tabFrames[ this.tabFrames.length ] = tabFrame;
    
    // Create a tab-bar for the tab.
    var tabBarTab = document.createElement('span');
    tabBarTab.id = this.objectName + ' ' + name + ' tab';
    tabBarTab.setAttribute('onclick', this.objectName + '.activateTab("' + name + '");');
    tabBarTab.setAttribute('style', 'padding-left: 15px; padding-right: 15px; padding-top: 5px; ' +
                                    'padding-bottom: 5px; text-align: center; cursor: hand; ' +
                                    'background-color: ' + this.inactiveTabColor + '; ' + 
                                    'color: ' + this.inactiveTabTextColor + ';' +
                                    'border: 1px; border-style: solid; border-color: #000000');
    tabBarTab.innerHTML = name;
    this.tabBarRow.appendChild(tabBarTab);
    
    // Activate the first tab
    this.activateTab(this.tabNames[0]);
}

TabWidget.prototype.indexOfTab = function(name) {
    var i=0;
    for(i=0; i<this.tabNames.length; i++)
    {
        if(this.tabNames[i] == name)
            return i;
    }
    return -1;
}

TabWidget.prototype.activateTab = function(name) {
    var index = 0;
    if( (typeof name) == 'string')
        index = this.indexOfTab(name);
    else
        index = name;
        
    if(index == this.activeTabIndex)
        return;
        
    var tabFrame = false;
    var tabBarTab = false;
    if(this.activeTabIndex >= 0)
    {
        tabFrame = this.tabFrames[this.activeTabIndex];
        tabFrame.setAttribute("width", "0px");
        tabFrame.setAttribute("height", "0px");
        tabFrame.style.visibility = 'hidden';
        tabFrame = false;
        
        tabBarTab = document.getElementById(this.objectName + ' ' + 
                                            this.tabNames[this.activeTabIndex] +
                                            ' tab');
        tabBarTab.style.backgroundColor = this.inactiveTabColor;
        tabBarTab.style.color = this.inactiveTabTextColor;
        tabBarTab = false;
    }
    
    this.activeTabIndex = index;
    if(this.activeTabIndex >= 0)
    {
        tabFrame = this.tabFrames[this.activeTabIndex];
        tabFrame.setAttribute("width", "100%");
        tabFrame.setAttribute("height", "100%");
        tabFrame.style.visibility = 'visible';
        if(!tabFrame.getAttribute('src').length)
            tabFrame.setAttribute('src', tabFrame.getAttribute('givenSource'));
        tabFrame = false;
        
        tabBarTab = document.getElementById(this.objectName + ' ' + 
                                            this.tabNames[this.activeTabIndex] +
                                            ' tab');
        tabBarTab.style.backgroundColor = this.activeTabColor;
        tabBarTab.style.color = this.activeTabTextColor;
        tabBarTab = false;
        
        if(this.onCurrentTabIndexChanged)
            this.onCurrentTabIndexChanged(this.activeTabIndex);
            
        if(this.onCurrentTabNameChanged)
            this.onCurrentTabNameChanged(this.tabNames[this.activeTabIndex]);
    }
}

TabWidget.prototype.getCurrentTabIndex = function() {
    return this.activeTabIndex;
}



