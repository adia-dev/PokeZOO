(function () {
    var memoryInitializer = 'out.js';
    console.log('Downloading...');
    if (typeof Module['locateFile'] === 'function') {
        memoryInitializer = Module['locateFile'](memoryInitializer);
    } else if (Module['memoryInitializerPrefixURL']) {
        memoryInitializer = Module['memoryInitializerPrefixURL'] + memoryInitializer;
    }
    var xhr = Module['memoryInitializerRequest'] = new XMLHttpRequest();
    xhr.open('GET', memoryInitializer, true);
    xhr.responseType = 'arraybuffer';
    xhr.send(null);
})();

var script = document.createElement('script');
script.src = "../wasm/out.js";
document.body.appendChild(script);