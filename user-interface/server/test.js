var fork = require('child_process').fork;

var child = fork('./child', {
  detached: true
});

child.send('Test');

child.unref();