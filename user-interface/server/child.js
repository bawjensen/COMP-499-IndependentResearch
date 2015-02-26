process.on('message', function(m) {
    console.log('Child got:', m);
    m.test2 = false;

    var i = 0;
    while (++i < 10000000000) {
        if (i % 100000000 === 0) {
            console.log(i);
        }
    }

    process.send(m);
});