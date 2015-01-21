function ImageWidget() {
    // The image to show
    this.image = false;
    this.imageRect = false;
    
    // The canvas on which the image will be shown.
    this.canvas = false;
    
    // The parent DIV
    this.widgetDiv = false;
        
    // Attributes
    this.backgroundColor = "#000000";
    this.keepAspectRatio = true;
    
    // Event handlers
    this.onClick = false;
    this.onMouseDown = false;
    this.onMouseMove = false;
    this.onMouseUp = false;
	this.onMouseDblClick = false;
    this.onScrollWheelUp = false;
    this.onScrollWheelDown = false;
    this.onKeyPress = false;
    this.onDrawForeground = false;
    this.onDrawBackground = false;
	this.onScroll = false;
}

ImageWidget.prototype.getMousePos = function(e) {
    var pos = new Object();

    var success = false;
    
	pos.x = e.offsetX;
    pos.y = e.offsetY;
    
	success = (typeof(pos.x) == "number");
	if(!success) {
	
		var tmp = e.currentTarget;
		var curTop = 0;
		var sclTop = 0;
		var curLeft = 0;
		var sclLeft = 0;
		if (tmp.offsetParent) {
			do {
				curTop += tmp.offsetTop;
				sclTop += tmp.scrollTop;
				curLeft += tmp.offsetLeft;
				sclLeft += tmp.scrollLeft;
			} while (tmp = tmp.offsetParent);
		}
	
		pos.x = e.clientX + sclLeft - curLeft;
		pos.y = e.clientY + sclTop - curTop;
	}
    
    return pos;
}

ImageWidget.prototype.installOn = function(div) {
    if(this.widgetDiv)
        return;
        
    var thisObject = this;

    // Initialize parent
    this.widgetDiv = div;
    // this.widgetDiv.style.margin = "1px";
    document.body.onresize = function() {
        thisObject.parentWidgetResized();
    }

    // Create a canvas object on the DIV
    this.canvas = document.createElement("canvas");
    this.canvas.setAttribute("width", '' + div.clientWidth + 'px');
    this.canvas.setAttribute("height", '' + div.clientHeight + 'px');
    this.canvas.setAttribute("style", "cursor: crosshair;");
    this.widgetDiv.appendChild(this.canvas);
    
    // Event Handlers.
    this.canvas.onclick = function(e) {
        var evt = window.event || e;
        if( thisObject.onClick )
            thisObject.onClick(evt, thisObject.getMousePos(evt));
    }
    this.canvas.onmousedown = function(e) {
        var evt = window.event || e;
        if( thisObject.onMouseDown ) 
            thisObject.onMouseDown(evt, thisObject.getMousePos(evt));
    }
    this.canvas.onmousemove = function(e) {
        var evt = window.event || e;
        if( thisObject.onMouseMove ) 
            thisObject.onMouseMove(evt, thisObject.getMousePos(evt));
    }
    this.canvas.onmouseup = function(e) {
        var evt = window.event || e;
        if( thisObject.onMouseUp ) 
            thisObject.onMouseUp(evt, thisObject.getMousePos(evt));
    }
    this.canvas.onmouseup = function(e) {
        var evt = window.event || e;
        if( thisObject.onMouseUp ) 
            thisObject.onMouseUp(evt, thisObject.getMousePos(evt));
    }
    this.canvas.onkeypress = function(e) {
        var evt = window.event || e;
        if( thisObject.onKeyPress )
            thisObject.onKeyPress(evt);
    }
	this.canvas.ondblclick = function(e) {
		var evt = window.event || e;
		if( thisObject.onMouseDblClick )
			thisObject.onMouseDblClick(evt, thisObject.getMousePos(evt));
	}
    
    // Dude! Aneesh....
    // FF doesn't recognize mousewheel as of FF3.x
    var wheelEventName = (/Firefox/i.test(navigator.userAgent)) ? "DOMMouseScroll" : "mousewheel";
    if( this.canvas.addEventListener ) {
        this.canvas.addEventListener(wheelEventName, 
                function(e) {
                    var evt = window.event || e;
                    var delta = e.detail ? e.detail*(-120) : e.wheelDelta
                    if(delta < 0) {
                        if(thisObject.onScrollWheelDown)
                            thisObject.onScrollWheelDown(evt, thisObject.getMousePos(evt));
                    } else {
                        if(thisObject.onScrollWheelUp)
                            thisObject.onScrollWheelUp(evt, thisObject.getMousePos(evt));
                    }
                }, false);
    }

    // Update the drawing of the widget.
    this.update();
}

ImageWidget.prototype.parentWidgetResized = function() {
    if(!this.widgetDiv)
        return;

    this.canvas.width = this.widgetDiv.clientWidth;
    this.canvas.height = this.widgetDiv.clientHeight;
    this.update();
}

ImageWidget.prototype.update = function() {
    if(!this.widgetDiv)
        return;

    var context = this.canvas.getContext("2d");
    context.fillStyle = this.backgroundColor;
    context.fillRect(0, 0, this.canvas.width, this.canvas.height);
    
    if( this.image && this.image.width >= 1 && this.image.height >= 1) {    
        var imageRect = new Object();
        imageRect.x = 0;
        imageRect.y = 0;
        imageRect.width = this.image.width;
        imageRect.height = this.image.height;
        imageRect.left = function() { return this.x; }
        imageRect.top = function() { return this.y; }
        imageRect.right = function() { return this.x+this.width-1; }
        imageRect.bottom = function() { return this.y+this.height-1; }
        
        if(this.keepAspectRatio) {
        
            // Adjust width and height
            var rw = this.canvas.height * imageRect.width / imageRect.height;
            var useHeight = (rw <= this.canvas.width);
            if(useHeight) {
                imageRect.width = rw;
                imageRect.height = this.canvas.height;
            } else {
                imageRect.height = this.canvas.width * imageRect.height / imageRect.width;
                imageRect.width = this.canvas.width;
            }
            
            // Adjust image rectangle's center.
            imageRect.x = (this.canvas.width - imageRect.width)/2.0;
            imageRect.y = (this.canvas.height - imageRect.height)/2.0;
        }
        
        this.imageRect = imageRect;
        
        // Draw the layers.
        context.globalAlpha = 1.0;
        if(this.onDrawBackground)
            this.onDrawBackground(context, imageRect);
            
        context.globalAlpha = 1.0;
        context.drawImage(this.image, imageRect.x, imageRect.y, imageRect.width, imageRect.height);
        
        if(this.onDrawForeground)
            this.onDrawForeground(context, imageRect);
    }
}

ImageWidget.prototype.setImage = function(imageObject) {
    this.image = imageObject;
    var thisObject = this;
    this.image.onload = function() {
        thisObject.update();
    }
}

ImageWidget.prototype.clearImage = function(imageObject) {
    this.image = false;
    this.update();
}

Image.prototype.setKeepAspectRatio = function(val) {
    if(this.keepAspectRatio == val)
        return;
        
    this.keepAspectRatio = val;
    this.update();
}

ImageWidget.prototype.isKeepAspectRatio = function() {
    return this.keepAspectRatio;
}

