var fork = require('child_process').fork;

function createDetachedChild() {
    // console.log('test');
    var interfaceChild = fork('./child', { detached: true });

    interfaceChild.send({ runConfig: null, projectRoot: null, outDir: __dirname });

    interfaceChild.unref();
}

createDetachedChild();