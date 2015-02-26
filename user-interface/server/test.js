var fork = require('child_process').fork;

function createDetachedChild() {
    console.log('test');
    var interfaceChild = fork('./interface', { detached: true });

    interfaceChild.send({ runConfig: null, projectRoot: null, outDir: null });

    // interfaceChild.unref();

}

createDetachedChild();
