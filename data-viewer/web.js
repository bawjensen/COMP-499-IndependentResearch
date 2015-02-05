var express = require('express'),
    fs      = require('fs');

var app = express();

var RUNS_BASE_PATH = '../runs/saved-runs/';

function promiseRead(filepath) {
    console.log('Reading ' + filepath);
    return new Promise(function read(resolve, reject) {
        fs.readFile(filepath, 'utf8', function(err, data) {
            if (err)
                reject(Error(err));
            else
                resolve(data);
        });
    });
}

app.use('/js', express.static(__dirname + '/js'));
app.use('/css', express.static(__dirname + '/css'));

app.get('/', function(req, res) {
    res.render('index.jade');
});

app.get('/data/', function(req, res) {
    var path = req.params.path;

    fs.readdir(RUNS_BASE_PATH, function(err, files) {
        if (err) {
            console.log(err.stack);
            res.send(err);
        }

        for (var i in files) {
            var file = files[i];
            // fs.readFile
        }

        Promise.all(
            files.map(function(entry) {
                return promiseRead(RUNS_BASE_PATH + entry + '/output.csv');
            })
        )
        .then(function sendData(data) {
            res.send(data);
        })
        .catch(function handleErr(err) {
            console.log(err.stack);
            res.send(err);
        });
    });
});

var port = (process.argv[2] != "undefined" ? process.argv[2] : undefined) || 7502;
app.listen(port, function() {
    console.log('Listening on ' + port);
});