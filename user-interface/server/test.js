var fs = require('fs');
var fork = require('child_process').fork;
var out = fs.createWriteStream('./out.log');
var err = fs.openSync('./out.log', 'a');

var child = fork('./child', {
  detached: true,
  silent: true
});

child.stdout.pipe(out);

child.send('Test');

child.unref();