var Module = {
    onRuntimeInitialized: () => {
        console.log('Runtime initialized.');
    },
    canvas: (() => {
        const canvas = document.getElementById('canvas');
        return canvas;
    })(),
    locateFile: (filename) => {
        const wasmDirectory = '/public/wasm'
        if (!wasmDirectory) {
            throw new Error('VITE_WASM_DIRECTORY environment variable not found.');
        }
        return `${wasmDirectory}/${filename}`;
    }
};