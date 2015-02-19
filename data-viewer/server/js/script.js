var requestNumber = 0;

$(function() {
    $(document).on('click', '.run', function() {
        $(this).toggleClass('selected');
    });

    $('#go').click(function() {
        var selected = [];

        $('.selected').each(function(i, ele) {
            selected.push($(ele).find('.label').text());
        });

        if (selected.length) {
            $.ajax({
                url: '/',
                method: 'POST',
                data: {
                    selected: selected,
                    reqNum: requestNumber++
                },
                success: function(data, status) {
                    var $newPlot = $('<img>', {
                        id: 'plot',
                        src: data
                    });

                    $('#plot-container').html($newPlot);
                }
            });
        }
        else {
            alert('Please select some runs to be plotted');
        }
    });

    var lastSorted;
    $('#run-label span').click(function(evt) {
        var sortByClass = '.' + $(this).attr('class');

        var sortingSame = (sortByClass === lastSorted);

        var sorted = $('.run').sort(function(a, b) {
            var vA = $(a).find(sortByClass).text();
            var vB = $(b).find(sortByClass).text();

            if (!isNaN(vA)) {
                vA = parseInt(vA);
                vB = parseInt(vB);
            }

            return sortingSame ?
                ((vA > vB) ? -1 : (vA < vB) ? 1 : 0) :
                ((vA < vB) ? -1 : (vA > vB) ? 1 : 0);
        });

        $('#all-runs').html(sorted);

        if (sortingSame)
            lastSorted = undefined;
        else
            lastSorted = sortByClass;
    });
});