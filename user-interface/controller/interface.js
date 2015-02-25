var spawn       = require('child_process').spawn,
    fs          = require('fs'),
    path        = require('path');

function trainingRun(runConfig, projectRoot) {
    var date = new Date;
    var dateStr = [date.getMonth(), date.getDate(), date.getYear()].join('-');

    /*
    1. Label for this training set (same thing as folder name)
    2. Number of generations
    3. Number of nets per generation
    4. Number of games per net per generation
    5. Size of the hidden layer
    6. Depth of parsing for minimax tree
    7. Mean for gaussian random number
    8. Standard deviation for gaussian random number
    9. Mode for evaluating the "quality" of a net
    */

    var keyOrdering = [
        'label',
        'gens',
        'nets',
        'games',
        'layer',
        'depth',
        'mean',
        'std-dev',
        'mode'
    ];

    var configArgs = [];
    keyOrdering.forEach(function(key) {
        configArgs.push(runConfig[key]);
    });

    var cmd = 'neural-nets/bin/2048f';
    var outDir = path.join(projectRoot, 'runs', runConfig.label);
    var outLogPath = path.join(outDir, 'output.csv');

    fs.mkdir(outDir, function(err) {
        if (err) throw err;
        console.log('Created', outDir);

        console.log('Starting training run, writing output to', outLogPath);
        spawn(cmd, configArgs, { cwd: projectRoot, stdio: [ 0, fs.openSync(outLogPath, 'w'), 2 ] }, function(err) {
            if (err) throw err;
            console.log('Training run good');
        }); 
    });
}

module.exports = {
    run: trainingRun
}