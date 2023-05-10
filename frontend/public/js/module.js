var Module = {
    onRuntimeInitialized: function () {
        console.log('onRuntimeInitialized');
        // const newCanvas = document.createElement('canvas');
        // newCanvas.id = 'canvas';
        // newCanvas.width = window.innerWidth;
        // newCanvas.height = window.innerHeight;
        // document.body.appendChild(newCanvas);
        // // Module.canvas = newCanvas;
        // start_function(document.getElementById('canvas'));
    },
    canvas: (function () {
        var canvas = document.getElementById('canvas');

        // if (!canvas) {
        //     canvas = document.createElement('canvas');
        //     document.body.appendChild(canvas);
        // }

        // canvas.width = window.innerWidth;
        // canvas.height = window.innerHeight;

        // window.addEventListener('resize', function () {
        //     canvas.width = window.innerWidth;
        //     canvas.height = window.innerHeight;
        // });

        return canvas;
    })(),
    locateFile: function (s) {
        return '../wasm/' + s;
    }
};

var start_function = function (o) {
    console.log('start_function');
    console.log(Module);
    // o.style.visibility = "hidden";
    // document.getElementById("fullScreenButton").style.visibility = "visible";
    Module.ccall('mainf', null, null);
};