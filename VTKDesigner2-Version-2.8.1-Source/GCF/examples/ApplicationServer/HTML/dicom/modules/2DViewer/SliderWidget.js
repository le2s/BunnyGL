
function SliderWidget(objName)	{
	
	// Private members
	this.objectName = objName;
	this.min = 0;
	this.max = 100;
	this.value = 0;
	this.verticalValue = 0;
	this.initialized = false;
	this.pressed = false;
	this.inMouseEvent = false;
	
	this.onChange = false;
	this.target = false;
	this.continuousUpdate = true;
	this.vertical = true;
	
	var obj = this;
	
	// Private methods
	this.addAnEvent = function(target, event, func) {
		if (target.attachEvent) { // IE
			target.attachEvent("on" + event, func);
		} else if (target.addEventListener) { // Gecko / W3C
			target.addEventListener(event, func, true);
		} else {
			target["on" + event] = func;
		}
	}
	
	this.stopEvent = function(event) {
		if (event.preventDefault) {
			event.preventDefault();
			event.stopPropagation();
		} else {
			event.returnValue=false;
			event.cancelBubble=true;
		}
	}
	
	this.SW_onMousePress = function(event) {
	
		obj.inMouseEvent = true;
		
		var slider = document.getElementById('sliderDiv');
		
		if (!event && window.event) event = window.event;
		if(!this.vertical)
			obj.computeValueFromClientXorY(event.clientX);
		else
			obj.computeValueFromClientXorY(event.clientY);
		
		obj.stopEvent(event);
		obj.pressed = true;
	}
	
	this.SW_onMouseMove = function(event) {
		if(!obj.pressed)
			return;
		
		var slider = document.getElementById('sliderDiv');
		
		if (!event && window.event) event = window.event;
		if(!obj.vertical)
			obj.computeValueFromClientXorY(event.clientX);
		else
			obj.computeValueFromClientXorY(event.clientY);
			
		obj.stopEvent(event);
	}
	
	this.SW_onMouseRelease = function(event) {
		if(!obj.pressed)
			return;
	
		obj.pressed = false;
		var slider = document.getElementById('sliderDiv');
		
		if (!event && window.event) event = window.event;
		if(!obj.vertical)
			obj.computeValueFromClientXorY(event.clientX);
		else
			obj.computeValueFromClientXorY(event.clientY);
		
		obj.stopEvent(event);
		obj.inMouseEvent = false;
	}
	
	this.drawSliderByVal = function() {
		var knob = document.getElementById('knobImg');
		var slider = document.getElementById('sliderDiv');
		var p = (obj.value - obj.min) / (obj.max - obj.min);
		
		if(!obj.vertical) {
		
			var x = (slider.scrollWidth - 30) * p;
			knob.style.left = x + "px";
			
		} else {
		
			var y = (slider.scrollHeight - 30) * p;
			knob.style.top = (y) + "px";
			
		}
	}
	
	this.computeValueFromClientXorY = function(clientXorY) {
		
		var knob = document.getElementById('knobImg');
		var slider = document.getElementById('sliderDiv');
		var p = 0;
		
		var tmp = slider;
		var curTop = 0;
		var sclTop = 0;
		if (tmp.offsetParent) {
			do {
				curTop += tmp.offsetTop;
				sclTop += tmp.scrollTop;
			} while (tmp = tmp.offsetParent);
		}
		
		if(!obj.vertical)
			p = ( clientXorY - slider.offsetLeft - 15 ) / ( slider.scrollWidth - 30 );
		else
			p = ( clientXorY + sclTop - curTop ) / ( slider.scrollHeight - 30 );
		
		var tmp = parseInt( ( obj.max - obj.min ) * p + obj.min );
		
		if((tmp >= obj.min) && (tmp <= obj.max))
			obj.setValue(tmp);
	}
	
	this.stylize = function() {
	
		if(obj.vertical) {
			this.target.innerHTML = '<style type="text/css"> ' +
							'.slider { margin: 0px auto; position: relative; width: 15px; height: 100%; background: url(img/bg2.png) 50% 50% repeat-y;} \
							.slider img {	position: absolute;	top: 85px;left: 0px;} \
							.slider .top, .slider .bottom { height: 2px; width: 12px; position: absolute; left: 0px;}\
							.slider .top { top: 0px; background: url(img/top.png) 0px 1px no-repeat;}\
							.slider .bottom { bottom: 0px;	background: url(img/bottom.png) 0px 1px no-repeat;}' +
								'</style> \
							<div class="slider" id="sliderDiv"> \
								<div class="top"></div> \
								<div class="bottom"> </div> \
								<img id="knobImg" src="img/knob.png" width="15" height="31"/> \
							</div>';
		} else {
			this.target.innerHTML = '<style type="text/css"> ' +
							'.slider { margin: 0.5em auto; position: relative; width: 600px; height: 15px; background: url(img/bg.png) 50% 50% repeat-x;} \
							.slider img {	position: absolute;	top: 0px;left: 85px;} \
							.slider .left, .slider .right { height: 12px; width: 2px; position: absolute; top: 0px;}\
							.slider .left { left: 0px; background: url(img/left.png) 0px 1px no-repeat;}\
							.slider .right { right: 0px;	background: url(img/right.png) 0px 1px no-repeat;}' +
								'</style> \
							<div class="slider" id="sliderDiv"> \
								<div class="left"></div> \
								<div class="right"> </div> \
								<img id="knobImg" src="img/knob.png" width="31" height="15"/> \
							</div>';
		}
	}
}

// Public methods

SliderWidget.prototype.setMinimum = function(value) {
	if(!this.initialized)
		return;
	
	this.min = value;
}

SliderWidget.prototype.minimum = function() {
	return this.min;
}

SliderWidget.prototype.setMaximum = function(value) {
	if(!this.initialized)
		return;

	this.max = value;
}

SliderWidget.prototype.maximum = function() {
	return this.max;
}

SliderWidget.prototype.setRange = function(minValue, maxValue) {
	if(!this.initialized)
		return;

	this.min = minValue;
	this.max = maxValue;
}

SliderWidget.prototype.setValue = function(value) {
	if(!this.initialized)
		return;

	if(this.value == value || this.verticalValue == value)
		return;
	
	if(this.inMouseEvent) {
		this.value = value;
		this.verticalValue = this.min + (this.max - value);
	} else {
		this.verticalValue = value;
		this.value = this.min + (this.max - value);
	}
	
	this.drawSliderByVal();
	
	var emit = true;
	if(!this.continuousUpdate)
		if(this.pressed)
			emit = false;
	if(emit) {
		if(this.vertical)
			this.onChange(this.verticalValue);
		else
			this.onChange(this.value);
	}
}

SliderWidget.prototype.value = function() {
	if(this.vertical)
		return this.verticalValue;
	else
		return this.value;
}

SliderWidget.prototype.setContinuousUpdate = function(value) {
	if(!this.initialized)
		return;

	if(this.continuousUpdate == value)
		return;
		
	this.continuousUpdate = value;
}

SliderWidget.prototype.continuousUpdate = function(value) {
	return this.continuousUpdate;
}

SliderWidget.prototype.setAsVertical = function() {
	if(!this.initialized)
		return;

	if(this.vertical)
		return;
		
	this.vertical = true;
	this.stylize();
	this.update();
}

SliderWidget.prototype.isVertical = function() {
	return this.vertical;
}

SliderWidget.prototype.setAsHorizontal = function() {
	if(!this.initialized)
		return;

	if(!this.vertical)
		return;
		
	this.vertical = false;
	this.stylize();
	this.update();
}

SliderWidget.prototype.isHorizontal = function() {
	return !this.vertical;
}

SliderWidget.prototype.update = function() {
	this.drawSliderByVal();
	if(this.vertical)
		this.onChange(this.verticalValue);
	else
		this.onChange(this.value);
}

SliderWidget.prototype.onValueChange = function(func) {
	if(!this.initialized)
		return;
		
	this.onChange = func;
}

SliderWidget.prototype.installOn = function(divElement) {
	this.target = divElement;
	this.stylize();		
	var targetDiv = document.getElementById("sliderDiv");
	this.addAnEvent(targetDiv, 'mousedown', this.SW_onMousePress);
	this.addAnEvent(targetDiv, 'mousemove', this.SW_onMouseMove);
	this.addAnEvent(targetDiv, 'mouseup', this.SW_onMouseRelease);
	this.addAnEvent(document, 'mousemove', this.SW_onMouseMove);
	this.addAnEvent(document, 'mouseup', this.SW_onMouseRelease);
	this.initialized = true;
}



