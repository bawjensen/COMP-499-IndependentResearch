var bodyParser  = require('body-parser'),
    exec        = require('child_process').exec,
    express     = require('express'),
    fs          = require('fs'),
    jade        = require('jade'),
    path        = require('path');

app = express();

app.use(bodyParser.json());       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
})); 

app.use('/js', express.static(__dirname + '/js'));
app.use('/css', express.static(__dirname + '/css'));
app.use('/img', express.static(__dirname + '/img'));

var savedRunsRelativePath = '../../runs/saved-runs/';

app.get('/', function(req, res) {
    fs.readdir(savedRunsRelativePath, function(err, runDirs) {
        if (err) {
            res.status(503).send('503');
        }
        else {
            var runs = {};
            var allKeys = [
                { key: 'numGenerations',        display: 'Gens',        _class: 'gens'},
                { key: 'numNets',               display: 'Nets',        _class: 'nets'},
                { key: 'numGamesPerNet',        display: 'Games',       _class: 'games'},
                { key: 'netHiddenLayerSize',    display: 'Size',        _class: 'layer'},
                { key: 'chMode',                display: 'Mode',        _class: 'mode'},
                { key: 'treeDepth',             display: 'Depth',       _class: 'depth'},
                { key: 'randomMean',            display: 'Mean',        _class: 'mean'},
                { key: 'randomStdDev',          display: 'StdDev',      _class: 'std-dev'}
            ];

            runDirs.forEach(function(runDir) {
                var csvLines = fs.readFileSync(path.join(savedRunsRelativePath, runDir, 'output.csv'), 'utf8').split('\n');

                var keys = csvLines[0].split(',');
                var values = csvLines[1].split(',');

                runs[runDir] = {};
                keys.forEach(function(key, i) {
                    runs[runDir][key] = values[i];
                });
            });

            res.render('index.jade', { runs: runs, allKeys: allKeys });
        }
    });
});

app.post('/', function(req, res) {
    var selected = req.body.selected;

    if (selected.length === 0) {
        res.status(400).end();
    }

    var plotName = 'img/plot' + req.body.reqNum + '.png';

    exec('python plotter.py ' + plotName + ' ' + selected.join(' '), { cwd: '.' }, function(err, stdout) {
        if (err) {
            console.log(err.stack);
            res.status(500).end();
        }
        else {
            console.log('Plot successfully created');
            res.send(plotName);
        }
    });
});

var port = process.argv[2] || 5000;
app.listen(port, function() {
    console.log('Listening on ' + port);
});
