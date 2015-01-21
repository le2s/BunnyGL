/******************************************************************************
This JavaScript has been tested with the following browsers
    a) Chrome 5.0.375.125
    b) Firefox 3.6.11
******************************************************************************/

///////////////////////////////////////////////////////////////////////////////

var GlobalMessageCounter = 0;

///////////////////////////////////////////////////////////////////////////////

function GetGUID()
{
    var guid = '';

    if(document.cookie.length > 0)
    {
        var guidStartIndex = document.cookie.indexOf('GUID=');
        if(guidStartIndex >= 0)
        {
            guidStartIndex += 5;
            var guidEndIndex = document.cookie.indexOf(';', guidStartIndex);
            if(guidEndIndex > guidStartIndex)
                guid = document.cookie.substring(guidStartIndex, guidEndIndex);
            else
                guid = document.cookie.substring(guidStartIndex);
        }
    }

    if(!guid.length)
    {
        var chars = '0123456789abcdef'.split('');

        var uuid = [], rnd = Math.random, r;
        uuid[8] = uuid[13] = uuid[18] = uuid[23] = '-';
        uuid[14] = '4'; // version 4

        for (var i = 0; i < 36; i++)
        {
            if (!uuid[i])
            {
                r = 0 | rnd()*16;
                uuid[i] = chars[(i == 19) ? (r & 0x3) | 0x8 : r & 0xf];
            }
        }

        guid = uuid.join('');
        if(document.cookie.length)
            document.cookie += ';GUID=' +  guid;
        else
            document.cookie = 'GUID=' +  guid;
    }

    return  guid;
}

var GUID = GetGUID();
var GCFServerSideScript = "GCFApplicationServer.php";

// Custom data type support
var CustomTypes = new Object();
function GCFRegisterType(TypeName, FunctionName)
{
    CustomTypes[TypeName] = FunctionName;
}


///////////////////////////////////////////////////////////////////////////////
// XML Helper methods

function CreateDOMDocument(rootNodeName) {
    if ( (typeof ActiveXObject) != "undefined") {
        var progIDs = new Array (
                                    "Msxml2.DOMDocument.6.0",
                                    "Msxml2.DOMDocument.5.0",
                                    "Msxml2.DOMDocument.4.0",
                                    "Msxml2.DOMDocument.3.0",
                                    "MSXML2.DOMDocument",
                                    "MSXML.DOMDocument"
                                );
        var i=0;
        for(i=0; i<progIDs.length; i++) {
            try {
                var domDoc = new ActiveXObject(progIDs[i]);
                var rootNode = domDoc.createElement(rootNodeName);
                domDoc.appendChild(rootNode);
                return domDoc;
            } catch(e) {};
        }
    }
    
    try {
        return document.implementation.createDocument("", rootNodeName, null);
    } catch(e) {}
    
    return null;
}

function DOMDocumentToString(domDoc) {
    var xmlString = '';
    
    try {
        xmlString = (new XMLSerializer()).serializeToString(domDoc);
    } catch (e1) {
        try {
            xmlString = domDoc.xml;
        } catch(e2) {
            xmlString = '';
        }
    }
    
    return xmlString;
}

function DOMDocumentFromString(xmlString) {
    var domDoc = false;
    if(window.DOMParser)
        domDoc = (new DOMParser()).parseFromString(xmlString, "text/xml");
    else 
    {
        if ( (typeof ActiveXObject) != "undefined") {
            var progIDs = new Array (
                                        "Msxml2.DOMDocument.6.0",
                                        "Msxml2.DOMDocument.5.0",
                                        "Msxml2.DOMDocument.4.0",
                                        "Msxml2.DOMDocument.3.0",
                                        "MSXML2.DOMDocument",
                                        "MSXML.DOMDocument"
                                    );
            var i=0;
            for(i=0; i<progIDs.length; i++) {
                try {
                    var domDoc = new ActiveXObject(progIDs[i]);
                    domDoc.loadXML(xmlString);
                    return domDoc;
                } catch(e) {};
            }
        }
    }
    
    return null;
}

function TypeCheck(val, proto) {
    var ret = false;
    try {
        ret = (val.__proto__ == proto);
    } catch(e) { }
    return ret;
}

///////////////////////////////////////////////////////////////////////////////

function GCFValue(type, value) {
    this.type = type;
    this.value = value;
}

GCFValue.prototype.getType = function() {
    return this.type;
}

GCFValue.prototype.getValue = function() {
    return this.value;
}

GCFValue.prototype.isValid = function() {
    return this.type.length > 0 && this.type != 'Unsupported';
}

GCFValue.prototype.read = function(domElement) {
    var typeE = domElement.attributes.getNamedItem("Type");
    this.type = typeE.nodeValue;
    
    if(this.type == "Unsupported")
        return;

    if(this.type == "int")
        this.value = parseInt(domElement.firstChild.nodeValue);
    else if(this.type == "double")
        this.value = parseFloat(domElement.firstChild.nodeValue);
    else if(this.type == "bool")
        this.value = (domElement.firstChild.nodeValue == "true") ? true : false;
    else if(this.type == "QVariantList")
    {
        this.value = new GCFList();
        this.value.read(domElement);
    }
    else if(this.type == "QVariantMap")
    {
        this.value = new GCFDictionary();
        this.value.read(domElement);
    }
    else if(this.type == "QStringList")
    {
        var itemListE = domElement.getElementsByTagName("Item");
        var array = new Array();
        var i=0;
        for(i=0; i<itemListE.length; i++)
            array[i] = itemListE[i].firstChild.nodeValue;
        this.value = array;
    }
    else if(this.type == "QPixmap" || this.type == "QImage" || this.type == "QBitmap")
    {
        var img = new Image();
        var i=0;
        
        for(i=0; i<domElement.childNodes.length; i++)
            img.src += domElement.childNodes[i].nodeValue;
        this.value = img;
    }
    else if(CustomTypes.hasOwnProperty(this.type))
    {
        var func = new CustomTypes[this.type];
        func.read(domElement);
        this.value = func;
    }
    else
    {
        try {
            this.value = domElement.firstChild.nodeValue;
        } catch (e) {
            this.value = null;
        }
    }
    
}

GCFValue.prototype.write = function(domElement) {
    var doc = domElement.ownerDocument;
    var variantE = domElement;
    
    // Store Type Attribute
    var typeAttr = doc.createAttribute("Type");
    typeAttr.nodeValue = this.type;
    variantE.setAttributeNode(typeAttr);

    // Store the Value.
    // Handle special cases.
    if(this.type == "QVariantList" || this.type == "QVariantMap")
    {
        this.value.write(variantE);
        return;
    }
    else if(this.type == "QStringList")
    {
        var i=0;
        for(i=0; i<this.value.length; i++)
        {
            var itemE = doc.createElement("Item");
            itemE.appendChild(doc.createTextNode(this.value[i]));
            variantE.appendChild(itemE);
        }
    }
    else
        variantE.appendChild(doc.createTextNode(this.value));    
}

///////////////////////////////////////////////////////////////////////////////

function GCFList() {
    this.values = new Array();
    this.nullValue = new GCFValue();
}

GCFList.prototype.getLength = function() {
    return this.values.length;
}

GCFList.prototype.getSize = function() {
    return this.values.length;
}

GCFList.prototype.add = function(val) {
    if( !TypeCheck(val, GCFValue.prototype) )
        return;

    this.values[this.values.length] = val;
}

GCFList.prototype.at = function(index) {
    if(index < 0 || index >= this.values.length)
        return this.nullValue;
        
    return this.values[index];
}

GCFList.prototype.addInteger = function(val) {
    this.values[this.values.length] = new GCFValue("int", val);
}

GCFList.prototype.addBoolean = function(val) {
    this.values[this.values.length] = new GCFValue("bool", val);
}

GCFList.prototype.addDouble = function(val) {
    this.values[this.values.length] = new GCFValue("double", val);
}

GCFList.prototype.addString = function(val) {
    this.values[this.values.length] = new GCFValue("QString", val);
}

GCFList.prototype.addColor = function(val) {
    this.values[this.values.length] = new GCFValue("QColor", val);
}

GCFList.prototype.addPixmap = function(val) {
    this.values[this.values.length] = new GCFValue("QPixmap", val);
}

GCFList.prototype.addImage = function(val) {
    this.values[this.values.length] = new GCFValue("QImage", val);
}

GCFList.prototype.addUrl = function(val) {
    this.values[this.values.length] = new GCFValue("QUrl", val);
}

GCFList.prototype.addDate = function(val) {
    this.values[this.values.length] = new GCFValue("QDate", val);
}

GCFList.prototype.addTime = function(val) {
    this.values[this.values.length] = new GCFValue("QTime", val);
}

GCFList.prototype.addDateTime = function(val) {
    this.values[this.values.length] = new GCFValue("QDateTime", val);
}

GCFList.prototype.addList = function(val) {
    this.values[this.values.length] = new GCFValue("QVariantList", val);
}

GCFList.prototype.addDictionary = function(val) {
    this.values[this.values.length] = new GCFValue("QVariantMap", val);
}

GCFList.prototype.addStringList = function(val) {
    this.values[this.values.length] = new GCFValue("QStringList", val);
}

GCFList.prototype.read = function(domElement) {  
    var typeE = domElement.attributes.getNamedItem("Type");
    if(typeE.nodeValue != "QVariantList")
        return;
        
    this.values = new Array();

    var i=0;
    for(i=0; i<domElement.childNodes.length; i++)
    {
        if(domElement.childNodes[i].nodeName != 'Variant')
            continue;
        
        var variantE = domElement.childNodes[i];
        
        var value = new GCFValue();
        value.read(variantE);
        this.values[this.values.length] = value;
    }
}

GCFList.prototype.write = function(domElement) {
    var doc = domElement.ownerDocument;
    var variantE = domElement;
    
    // Store Type Attribute as QVariantList
    if(variantE.tagName == "Variant")
    {
        var typeAttr = doc.createAttribute("Type");
        typeAttr.nodeValue = "QVariantList";
        variantE.setAttributeNode(typeAttr);
    }
    
    // Now write all the variants.
    var i=0;
    for(i=0; i<this.values.length; i++)
    {
        var valueE = doc.createElement("Variant");
        this.values[i].write(valueE);
        variantE.appendChild(valueE);
    }
}

///////////////////////////////////////////////////////////////////////////////

function GCFDictionary() {
    this.keys = new Array();
    this.values = new Array();
    this.invalidValue = new GCFValue("", 0);
    
    this.currentKey = "";
    this.currentValue = this.invalidValue;
}

GCFDictionary.prototype.getLength = function() {
    return this.keys.length;
}

GCFDictionary.prototype.getSize = function() {
    return this.keys.length;
}

GCFDictionary.prototype.getKeys = function() {
    return this.keys;
}

GCFDictionary.prototype.getValues = function() {
    return this.values;
}

GCFDictionary.prototype.add = function(key, value) {
    if( !TypeCheck(value, GCFValue.prototype) )
        return;

    this.keys[this.keys.length] = key;
    this.values[this.values.length] = value;
    if(this.currentKey == key)
        this.currentValue = value;
}

GCFDictionary.prototype.addInteger = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("int", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addBoolean = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("bool", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addDouble = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("double", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addString = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("QString", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addColor = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("QColor", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addPixmap = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("QPixmap", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addImage = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("QImage", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addUrl = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("QUrl", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addDate = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("QDate", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addTime = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("QTime", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addDateTime = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("QDateTime", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addList = function(key, val) {
    if( !TypeCheck(val, GCFList.prototype) )
        return;

    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("QVariantList", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addDictionary = function(key, val) {
    if( !TypeCheck(val, GCFList.prototype) )
        return;

    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("QVariantMap", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.addStringList = function(key, val) {
    this.keys[this.keys.length] = key;
    this.values[this.values.length] = new GCFValue("QStringList", val);
    if(this.currentKey == key)
        this.currentValue = this.values[this.values.length-1];
}

GCFDictionary.prototype.valueOf = function(key) {
    if(this.currentKey == key)
        return this.currentValue;
       
    var i=0;
    for(i=0; i<this.keys.length; i++)
    {
        if(this.keys[i] == key)
        {
            this.currentKey = this.keys[i];
            this.currentValue = this.values[i];
            return this.currentValue;
        }
    }

    this.currentKey = key;
    this.currentValue = this.invalidValue;
    return this.currentValue;
}

GCFDictionary.prototype.read = function(domElement) {  
    var typeE = domElement.attributes.getNamedItem("Type");
    if(typeE.nodeValue != "QVariantMap")
        return;
        
    this.keys = new Array();
    this.values = new Array();

    var i=0;
    for(i=0; i<domElement.childNodes.length; i++)
    {
        if(domElement.childNodes[i].nodeName != 'Item')
            continue;
            
        var itemE = domElement.childNodes[i];
        var keyE = itemE.getElementsByTagName("Key")[0];
        var valueE = itemE.getElementsByTagName("Value")[0];
        var variantE = valueE.getElementsByTagName("Variant")[0];
        
        this.keys[this.keys.length] = keyE.firstChild.nodeValue;
    
        var value = new GCFValue();
        value.read(variantE);
        this.values[this.values.length] = value;
    }
}

GCFDictionary.prototype.write = function(domElement) {
    var doc = domElement.ownerDocument;
    var variantE = domElement;
    
    // Store Type Attribute as QVariantList
    var typeAttr = doc.createAttribute("Type");
    typeAttr.nodeValue = "QVariantMap";
    variantE.setAttributeNode(typeAttr);
    
    // Now write all the key/value pairs..
    var i=0;
    for(i=0; i<this.keys.length; i++)
    {
        var itemE = doc.createElement("Item");
        variantE.appendChild(itemE);
        
        var keyE = doc.createElement("Key");
        keyE.appendChild(doc.createTextNode(this.keys[i]));
        itemE.appendChild(keyE);
        
        var valueE = doc.createElement("Value");
        itemE.appendChild(itemE);
        
        var valueVariantE = doc.createElement("Variant");
        this.values[i].write(valueVariantE);
        valueE.appendChild(valueVariantE);
    }
}

///////////////////////////////////////////////////////////////////////////////

function GCFRequestMessage() {
    this.objectName = "ApplicationServer.Core";
    this.methodName = "upTime";
    this.argList = new GCFList();
    this.msgId = GlobalMessageCounter;
    this.onReply = false;

    GlobalMessageCounter = GlobalMessageCounter+1;
}

GCFRequestMessage.prototype.getMessageId = function() {
    return this.msgId;
}

GCFRequestMessage.prototype.setObjectName = function(val) {
    this.objectName = val;
}

GCFRequestMessage.prototype.getObjectName = function() {
    return this.objectName;
}

GCFRequestMessage.prototype.setMethodName = function(val) {
    this.methodName = val;
}

GCFRequestMessage.prototype.getMethodName = function() {
    return this.methodName;
}

GCFRequestMessage.prototype.getGUID = function() {
    return GUID;
}

GCFRequestMessage.prototype.addIntegerArg = function(val) {
    this.argList.addInteger(val);
}

GCFRequestMessage.prototype.addBooleanArg = function(val) {
    this.argList.addBoolean(val);
}

GCFRequestMessage.prototype.addDoubleArg = function(val) {
    this.argList.addDouble(val);
}

GCFRequestMessage.prototype.addStringArg = function(val) {
    this.argList.addString(val);
}

GCFRequestMessage.prototype.addColorArg = function(val) {
    this.argList.addColor(val);
}

GCFRequestMessage.prototype.addUrlArg = function(val) {
    this.argList.addUrl(val);
}

GCFRequestMessage.prototype.addDateArg = function(val) {
    this.argList.addDate(val);;
}

GCFRequestMessage.prototype.addTimeArg = function(val) {
    this.argList.addTime(val);
}

GCFRequestMessage.prototype.addDateTimeArg = function(val) {
    this.argList.addDateTime(val);
}

GCFRequestMessage.prototype.toXML = function() {
    var msg = CreateDOMDocument('GCFMessage');
    if(!msg)
        return;

    var rootE = msg.documentElement;
    var msgTypeAttr = msg.createAttribute("Type");
    msgTypeAttr.nodeValue = "262";
    rootE.setAttributeNode(msgTypeAttr);
    
    var guidAttr = msg.createAttribute("GUID");
    guidAttr.nodeValue = GUID;
    rootE.setAttributeNode(guidAttr);

    var msgIdE = msg.createElement("MessageID");
    msgIdE.appendChild(msg.createTextNode(this.msgId));
    rootE.appendChild(msgIdE);

    var objectNameE = msg.createElement("ObjectName");
    objectNameE.appendChild(msg.createTextNode(this.objectName));
    rootE.appendChild(objectNameE);

    var methodNameE = msg.createElement("MethodName");
    methodNameE.appendChild(msg.createTextNode(this.methodName));
    rootE.appendChild(methodNameE);

    var argsE = msg.createElement("Args");
    rootE.appendChild(argsE);
    
    this.argList.write(argsE);

    var sessionIdE = msg.createElement("SessionID");
    sessionIdE.appendChild(msg.createTextNode("-1"));
    rootE.appendChild(sessionIdE);

    var xmlString = DOMDocumentToString(msg);
    return xmlString;
}

GCFRequestMessage.prototype.send = function(url, username, password) {
    var ajax = false;
    try { 
        ajax = new XMLHttpRequest(); 
    } catch(e) {
        return false;
    }
    
    if(!url)
        url = GCFServerSideScript;
    
    ajax.open("POST", url, true, username, password);
    ajax.setRequestHeader("Content-Type", "text/xml");
    if(this.onReply)
    {
        var replyFunction = this.onReply;
        ajax.onreadystatechange = function() {
            if(ajax.readyState == 4) {
                if(ajax.status == 200) {
                    var rMsg = new GCFResponseMessage(ajax.responseXML);
                    replyFunction(rMsg);
                }
                else {
                    var rMsg = new GCFResponseMessage('');
                    replyFunction(rMsg);
                }
            }
        }
    }

    ajax.send(this.toXML());
}

///////////////////////////////////////////////////////////////////////////////

function GCFResponseMessage(xml) {
    var msg = false;
    var success = true;

    if( (typeof xml) == "string" ) {
        try {
            msg = DOMDocumentFromString(xml);
        } catch(e) {
            success = false;
        }
    } else {
        msg = xml;
        success = true;
    }
    
    this.msgId = -1;
    this.objectName = "";
    this.methodName = "";
    this.result = new GCFValue();
    this.errorMessage = "";

    if(!success)
    {
        this.errorMessage = "Could not parse GCFMessageXML";
        return;
    }

    var rootE = false;
    try {
        rootE = msg.documentElement;
    } catch(e) {
        this.errorMessage = "Could not contact GCFApplicationServer!";
        return;
    }

    if(rootE.tagName != "GCFMessage")
    {
        this.errorMessage = "Malformed GCFMessageXML(2): " + rootE.tagName;
        return;
    }

    var msgType = rootE.attributes.getNamedItem("Type").nodeValue;
    if(msgType != "3863")
    {
        this.errorMessage = "Wrong GCFMessageXML type";
        return;
    }
    else
        this.errorMessage = msgType;

    var msgIdE = rootE.getElementsByTagName("MessageID")[0];
    this.msgId = parseInt(msgIdE.firstChild.nodeValue);

    var objectNameE = rootE.getElementsByTagName("ObjectName")[0];
    this.objectName = objectNameE.firstChild.nodeValue;

    var methodNameE = rootE.getElementsByTagName("MethodName")[0];
    this.methodName = methodNameE.firstChild.nodeValue;

    var resultE = rootE.getElementsByTagName("Result")[0];
    var variantListE = resultE.getElementsByTagName("Variant");
    this.result.read( variantListE[0] );

    this.errorMessage = "";
}

GCFResponseMessage.prototype.isValid = function() {
    return this.msgId >= 0;
}

GCFResponseMessage.prototype.getObjectName = function() {
    return this.objectName;
}

GCFResponseMessage.prototype.getMethodName = function() {
    return this.methodName;
}

GCFResponseMessage.prototype.hasResult = function() {
    return this.result.isValid();
}

GCFResponseMessage.prototype.getResultType = function() {
    return this.result.type;
}

GCFResponseMessage.prototype.getResult = function() {
    return this.result.value;
}

GCFResponseMessage.prototype.getErrorMessage = function() {
    return this.errorMessage;
}

GCFResponseMessage.prototype.getGUID = function() {
    return GUID;
}

///////////////////////////////////////////////////////////////////////////////

function GCFInvokeMethod() {
    this.ObjectName = '';
    this.MethodName = '';
    this.ArgList = new GCFList();
}

function GCFMultiRequestMessage() {
    this.invokeMethods = new Array();
    this.msgId = GlobalMessageCounter;
    this.onReply = false;

    GlobalMessageCounter = GlobalMessageCounter+1;
}

GCFMultiRequestMessage.prototype.add = function(objectName, methodName, args) {
    var method = new GCFInvokeMethod();
    method.ObjectName = objectName;
    method.MethodName = methodName;
    if( TypeCheck(args, GCFList.prototype) )
        method.ArgList = args;
    this.invokeMethods[this.invokeMethods.length] = method;
}

GCFMultiRequestMessage.prototype.toXML = function() {
    var msg = CreateDOMDocument('GCFMessage');

    var rootE = msg.documentElement;
    var msgTypeAttr = msg.createAttribute("Type");
    msgTypeAttr.nodeValue = "264";
    rootE.setAttributeNode(msgTypeAttr);
    
    var guidAttr = msg.createAttribute("GUID");
    guidAttr.nodeValue = GUID;
    rootE.setAttributeNode(guidAttr);

    var msgIdE = msg.createElement("MessageID");
    msgIdE.appendChild(msg.createTextNode(this.msgId));
    rootE.appendChild(msgIdE);
    
    var invokeMethodsE = msg.createElement("InvokeMethods");
    rootE.appendChild(invokeMethodsE);
    
    var i=0;
    for(i=0; i<this.invokeMethods.length; i++)
    {
        var invokeMethodE = msg.createElement("InvokeMethod");
        invokeMethodsE.appendChild(invokeMethodE);
    
        var objectNameE = msg.createElement("ObjectName");
        objectNameE.appendChild(msg.createTextNode(this.invokeMethods[i].ObjectName));
        invokeMethodE.appendChild(objectNameE);

        var methodNameE = msg.createElement("MethodName");
        methodNameE.appendChild(msg.createTextNode(this.invokeMethods[i].MethodName));
        invokeMethodE.appendChild(methodNameE);
        
        var argsE = msg.createElement("Args");
        invokeMethodE.appendChild(argsE);
        
        this.invokeMethods[i].ArgList.write(argsE);
    }
    
    var sessionIdE = msg.createElement("SessionID");
    sessionIdE.appendChild(msg.createTextNode("-1"));
    rootE.appendChild(sessionIdE);

    var xmlString = DOMDocumentToString(msg);
    return xmlString;
}

GCFMultiRequestMessage.prototype.send = function(url, username, password) {
    var ajax = false;
    try { 
        ajax = new XMLHttpRequest(); 
    } catch(e) {
        return false;
    }
    
    if(!url)
        url = GCFServerSideScript;
    
    ajax.open("POST", url, true, username, password);
    ajax.setRequestHeader("Content-Type", "text/xml");
    if(this.onReply)
    {
        var replyFunction = this.onReply;
        ajax.onreadystatechange = function() {
            if(ajax.readyState == 4) {
                if(ajax.status == 200) {
                    var rMsg = new GCFMultiResponseMessage(ajax.responseXML);
                    replyFunction(rMsg);
                }
                else {
                    var rMsg = new GCFResponseMessage('');
                    replyFunction(rMsg);
                }
            }
        }
    }

    ajax.send(this.toXML());
}

///////////////////////////////////////////////////////////////////////////////

function GCFInvokedMethod() {
    this.ObjectName = '';
    this.MethodName = '';
    this.Result = new GCFValue();
}

function GCFMultiResponseMessage(xml) {
    var msg = false;
    var success = true;

    if( (typeof xml) == "string" ) {
        try {
            msg = DOMDocumentFromString(xml);
        } catch(e) {
            success = false;
        }
    } else {
        msg = xml;
        success = true;
    }
    
    this.msgId = -1;
    this.invokedMethods = new Array();
    
    if(!success)
    {
        this.errorMessage = "Could not parse GCFMessageXML";
        return;
    }
    
    var rootE = false;
    try {
        rootE = msg.documentElement;
    } catch(e) {
        this.errorMessage = "Could not contact GCFApplicationServer!";
        return;
    }

    if(rootE.tagName != "GCFMessage")
    {
        this.errorMessage = "Malformed GCFMessageXML(2): " + rootE.tagName;
        return;
    }
    
    var msgType = rootE.attributes.getNamedItem("Type").nodeValue;
    if(msgType != "3865")
    {
        this.errorMessage = "Wrong GCFMessageXML type";
        return;
    }
    else
        this.errorMessage = msgType;
        
    var msgIdE = rootE.getElementsByTagName("MessageID")[0];
    this.msgId = parseInt(msgIdE.firstChild.nodeValue);
    
    var invokedMethodsE = rootE.getElementsByTagName("InvokedMethods")[0];
    var invokedMethodEList = invokedMethodsE.getElementsByTagName("InvokedMethod");
    var i=0;
    for(i=0; i<invokedMethodEList.length; i++)
    {
        var invokedMethodE = invokedMethodEList[i];
        
        var method = new GCFInvokedMethod();
        method.ObjectName = invokedMethodE.getElementsByTagName("ObjectName")[0].firstChild.nodeValue;
        method.MethodName = invokedMethodE.getElementsByTagName("MethodName")[0].firstChild.nodeValue;
        
        var success = invokedMethodE.getElementsByTagName("Success")[0].firstChild.nodeValue == "true";
        
        if(success)
        {
            var resultE = invokedMethodE.getElementsByTagName("Result")[0];
            var variantE = resultE.getElementsByTagName("Variant")[0];
            method.Result.read( variantE );
        }
        
        this.invokedMethods[this.invokedMethods.length] = method;
    }

    this.errorMessage = '';
}

GCFMultiResponseMessage.prototype.isValid = function() {
    return this.msgId >= 0;
}

GCFMultiResponseMessage.prototype.moveToFirst = function(msgIndex) {
    if(msgIndex <= 0 || msgIndex >= this.invokedMethods.length)
        return;
        
    var msg0 = this.invokedMethods[0];
    var msgI = this.invokedMethods[msgIndex];
    this.invokedMethods[msgIndex] = msg0;
    this.invokedMethods[0] = msgI;
}

GCFMultiResponseMessage.prototype.hasResult = function(objectName, methodName) {
    var i=0;
    for(i=0; i<this.invokedMethods.length; i++)
    {
        if(this.invokedMethods[i].ObjectName == objectName &&
           this.invokedMethods[i].MethodName == methodName)
        {
            var result = this.invokedMethods[i].Result;
            this.moveToFirst(i);
            return result.isValid();
        }
    }
    
    return false;
}

GCFMultiResponseMessage.prototype.getResultType = function(objectName, methodName) {
    var i=0;
    for(i=0; i<this.invokedMethods.length; i++)
    {
        if(this.invokedMethods[i].ObjectName == objectName &&
           this.invokedMethods[i].MethodName == methodName)
        {
            var result = this.invokedMethods[i].Result;
            this.moveToFirst(i);
            return result.getType();
        }
    }
    
    return false;
}

GCFMultiResponseMessage.prototype.getResult = function(objectName, methodName) {
    var i=0;
    for(i=0; i<this.invokedMethods.length; i++)
    {
        if(this.invokedMethods[i].ObjectName == objectName &&
           this.invokedMethods[i].MethodName == methodName)
        {
            var result = this.invokedMethods[i].Result;
            this.moveToFirst(i);
            return result.getValue();
        }
    }
    
    return false;
}
