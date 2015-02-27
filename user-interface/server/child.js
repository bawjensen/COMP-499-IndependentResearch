// var fs = require('fs'),
//     path = require('path');

// process.on('message', function(m) {
//     // console.log('Child got:', m);
//     // m.test2 = false;

//     // var i = 0;
//     // while (++i < 1000000000) {
//     //     if (i % 100000000 === 0) {
//     //         console.log(i);
//     //     }
//     // }

//     // process.send(m);
//     console.log(__dirname);
    
//     var outLog = fs.createWriteStream(path.join(__dirname, 'morenew.log'));

//     outLog.write('test1');
// });

// process.stdout.write('yay');

process.on('message', function(m) {
    // process.stdout.write('yay2');
    console.log('yay2');
})