var bodyParser = require('body-parser'),
    exec    = require('child_process').exec,
    express = require('express'),
    fs      = require('fs'),
    jade    = require('jade');

app = express();

app.use(bodyParser.json());       // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({     // to support URL-encoded bodies
  extended: true
})); 

app.use('/js', express.static(__dirname + '/js'));
app.use('/css', express.static(__dirname + '/css'));
app.use('/img', express.static(__dirname + '/img'));

app.get('/', function(req, res) {
    var allRuns = fs.readdirSync('../../runs/saved-runs/');

    res.render('index.jade', { runs: allRuns });
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
            console.log(stdout);
            res.send(plotName);
        }
    });
});

var port = process.argv[2] || 5000;
app.listen(port, function() {
    console.log('Listening on ' + port);
});
