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
            res.status(404).send('404');
        }
        else {
            var runs = {};
            runDirs.forEach(function(runDir) {
                var csvLines = fs.readFileSync(path.join(savedRunsRelativePath, runDir, 'output.csv'), 'utf8').split('\n');

                var keys = csvLines[0].split(',');
                var values = csvLines[1].split(',');

                runs[runDir] = {};
                keys.forEach(function(key, i) {
                    runs[runDir][key] = values[i];
                });
            });

            res.render('index.jade', { runs: runs });
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
            console.log('success');
            res.send(plotName);
        }
    });
});

var port = process.argv[2] || 5000;
app.listen(port, function() {
    console.log('Listening on ' + port);
});
