var exec        = require('child_process').exec,
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

    var cmd = 'neural-nets/bin/2048f ' + configArgs.join(' ') + ' &';
    var outDir = path.join(projectRoot, 'runs', runConfig.label);
    var outLogPath = path.join(outDir, 'output.csv');

    fs.mkdir(outDir, function(err) {
        if (err) {
            console.log(err);
            // if (err) {}
        }
        console.log('Created', outDir);

        console.log('Starting training run');
        exec(cmd, { cwd: projectRoot }, function(err, stdout, stderr) {
            if (err) throw err;

            console.log('Training run successful');

            var reportObj = {};

            var lines = stdout.split('\n');

            var dataKeys = lines[0].split(',');
            var dataValues = lines[1].split(',');

            // Extract metadata
            for (var i in dataKeys) {
                reportObj[dataKeys[i]] = dataValues[i];
            }

            console.log('Parsing out', reportObj.numGenerations, 'generations');
            var genStart = 3;
            var genEnd = parseInt(reportObj.numGenerations) + genStart;
            var tmpArray = [];
            for (var i = genStart; i < genEnd; ++i) {
                try {
                    var split = lines[i].split(',');
                }
                catch (err) {
                    console.log(i, err);
                }
                tmpArray.push({ gen: split[0], avg: split[1], highest: split[2] });
            }

            reportObj.generations = tmpArray;

            fs.writeFile(path.join(outDir, 'test.json'), JSON.stringify(reportObj, null, 2), function() {});
        }); 
    });
}

module.exports = {
    run: trainingRun
}