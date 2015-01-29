function get(url) {
    return Promise.resolve(
        $.ajax({
            type: 'GET',
            url: url
        })
    );
}

function doStuff() {
    get('/data/')
        .then(function logIt(dataArray) {
            var dataObj = {};
            
            for (var i in dataArray) {
                var dataSet = dataArray[i];

                dataLines = dataSet.split('\n');

                for (var j in dataLines[0]) {
                    var key = dataLines[0][j];
                    var value = dataLines[1][j];


                }
            }
        });
}

$(function() {
    doStuff();
});