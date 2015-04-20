var bodyParser  = require('body-parser'),
    exec        = require('child_process').exec,
    fork        = require('child_process').fork,
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

var PROJECT_ROOT = path.join(__dirname, '../..');
var savedRunsRelativePath = path.join(PROJECT_ROOT, 'saved-runs/');

console.log('Running in:', __dirname);
console.log('Project root:', PROJECT_ROOT);

app.get('/', function(req, res) {
    fs.readdir(savedRunsRelativePath, function(err, runDirs) {
        if (err) {
            res.status(503).send('503');
        }
        else {
            var date = new Date();
            var dateStr = [date.getMonth()+1, date.getDate(), date.getUTCFullYear()%100].join('-');

            res.locals.allKeys = [
                { key: 'label',                 display: 'Label',       _class: 'label',    _default: dateStr + '_'},
                { key: 'numGenerations',        display: 'Gens',        _class: 'gens',     _default: '1000'},
                { key: 'numNets',               display: 'Nets',        _class: 'nets',     _default: '100'},
                { key: 'numGamesPerNet',        display: 'Games',       _class: 'games',    _default: '10'},
                { key: 'netHiddenLayerSize',    display: 'Size',        _class: 'layer',    _default: '16'},
                { key: 'chMode',                display: 'Mode',        _class: 'mode',     _default: 'h'},
                { key: 'treeDepth',             display: 'Depth',       _class: 'depth',    _default: '2'},
                { key: 'randomMean',            display: 'Mean',        _class: 'mean',     _default: '0'},
                { key: 'randomStdDev',          display: 'StdDev',      _class: 'std-dev',  _default: '2'},
                { key: 'numParents',            display: 'Parents',     _class: 'parents',  _default: '1'}
            ];

            var runs = [];
            runDirs.forEach(function(runDir) {
                var metaData = JSON.parse(fs.readFileSync(path.join(savedRunsRelativePath, runDir, 'output.json')));

                delete metaData.generations;
                metaData['label'] = runDir;

                runs.push(metaData);
            });

            res.render('index.jade', { runs: runs });
        }
    });
});

app.post('/plot', function(req, res) {
    var selected = req.body.selected;

    if (selected.length === 0) {
        res.status(400).end();
    }

    var plotName = 'img/plot' + req.body.reqNum + '.png';
    var plotMode = req.body.plotMode;

    exec('python plotter.py ' + plotName + ' ' + savedRunsRelativePath + ' ' + plotMode + ' ' + selected.join(' '), { cwd: '.' }, function(err, stdout) {
        if (err) {
            console.log(err.stack);
            res.status(500).end();
        }
        else {
            // console.log(stdout);
            console.log('Plot successfully created');
            res.send(plotName);
        }
    });
});

app.post('/run', function(req, res) {
    var data = req.body;

    var destinationDir = path.join(PROJECT_ROOT, 'runs', data.label);
    var savedRunsDir = path.join(PROJECT_ROOT, 'saved-runs', data.label);

    fs.exists(destinationDir, function(exists) {
        if (exists) {
            res.status(500).end();
            return;
        }

        fs.exists(savedRunsDir, function(exists) {
            if (exists) {
                res.status(500).end();
                return;
            }

            console.log('Starting the detached child process');

            var interfaceChild = fork('./interface', { detached: true/*, silent: true*/ });
            interfaceChild.send({ runConfig: data, projectRoot: PROJECT_ROOT, outDir: destinationDir });
            interfaceChild.unref();

            res.status(200).send('started');
        });
    });
});

var port = process.argv[2] || 8000;
app.listen(port, function() {
    console.log('Listening on ' + port);
});
