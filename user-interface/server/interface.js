var exec        = require('child_process').exec,
    fs          = require('fs'),
    path        = require('path');

function trainingRun(runConfig, projectRoot, outDir) {
    var logFilePath = path.join(outDir, 'full.log');
    var logFile = fs.createWriteStream(logFilePath);

    console.log('Created log file at', logFilePath, '- logging all results there');

    var date = new Date;
    var dateStr = [date.getMonth(), date.getDate(), date.getYear()].join('-');

    // Must match the order in the driver.cpp interfacing file
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

    logFile.write('Starting training run\n');
    exec(cmd, { cwd: projectRoot }, function(err, stdout, stderr) {
        if (err) throw err;

        if (stderr) {
            console.log('Error:', stderr);
            throw new Error(stderr);
        }

        logFile.write('Training run successful, parsing output\n');

        var reportObj = {};

        var lines = stdout.split('\n');

        var dataKeys = lines[0].split(',');
        var dataValues = lines[1].split(',');

        // Extract metadata
        for (var i in dataKeys) {
            reportObj[dataKeys[i]] = dataValues[i];
        }

        var genStart = 3;
        var genEnd = parseInt(reportObj.numGenerations) + genStart;
        var tmpArray = [];
        for (var i = genStart; i < genEnd; ++i) {
            try {
                var split = lines[i].split(',');
            }
            catch (err) {
                logFile.write(i, err);
            }
            tmpArray.push({ gen: split[0], avg: split[1], highest: split[2] });
        }

        reportObj.generations = tmpArray;

        fs.writeFile(path.join(outDir, 'test.json'), JSON.stringify(reportObj, null, 2), function(err) { if (err) logFile.write(err); });
        
        logFile.write('Output parsed and saved\n');
    });
}

process.on('message', function(m) {
    fs.mkdir(m.outDir, function(err) {
        if (err) throw err;

        console.log('Created', m.outDir);

        trainingRun(m.runConfig, m.projectRoot, m.outDir);
    });
});
