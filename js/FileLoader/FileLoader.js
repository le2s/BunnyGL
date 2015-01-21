
var __FILE__ = "";


function handleFileSelect(evt) {
    var files = evt.target.files; // FileList object

    // files is a FileList of File objects. List some properties.
    var output = [];
    for (var i = 0, f; f = files[i]; i++) {
      output.push('<li>', escape(f.name), '</li>');
      __FILE__ = escape(f.name);
    }
    document.getElementById('list').innerHTML = '<ul>' + output.join('') + '</ul>';
    //if(count == 100){count = 1;}
    init(count);
    count = true;
  }

  document.getElementById('files').addEventListener('change', handleFileSelect, false);