var requestNumber = 0;

$(function() {
    var defaultLabel = $('input#label').val();
    $('#new-run').submit(function(evt) {
        evt.preventDefault();

        var data = $(this).find('input').map(function() { return { key: this.name, value: this.value }; }).get();

        dataObj = {};
        data.forEach(function(entry) {
            dataObj[entry.key] = entry.value;
        });

        if (dataObj.label === defaultLabel) {
            alert('Must use a distinct label');
            return;
        }

        $.ajax({
            url: '/run',
            method: 'POST',
            data: dataObj,
            success: function(data, status) {
                console.log(data, status);
            },
            error: function(xhr, status, err) {
                console.log(err);
                alert('Label was already used');
            }
        });
    });

    $('#plot-it').click(function() {
        var selected = [];

        $('.selected').each(function(i, ele) {
            selected.push($(ele).find('.label').text());
        });

        if (selected.length) {
            $.ajax({
                url: '/plot',
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

    $(document).on('click', '.run', function() {
        $(this).toggleClass('selected');
    });

    var lastSorted;
    $('#run-label span').click(function(evt) {
        var sortByClass = '.' + $(this).attr('class');

        var sortingSame = (sortByClass === lastSorted);

        var sorted = $('.run').sort(function(a, b) {
            var vA = $(a).find(sortByClass).text();
            var vB = $(b).find(sortByClass).text();

            vA = vA === '' ? -1 : vA;
            vB = vB === '' ? -1 : vB;

            if (!isNaN(vA)) {
                vA = parseFloat(vA);
                vB = parseFloat(vB);
            }

            return sortingSame ?
                ((vA < vB) ? -1 : (vA > vB) ? 1 : 0) :
                ((vA > vB) ? -1 : (vA < vB) ? 1 : 0);
        });

        $('#all-runs').html(sorted);

        if (sortingSame)
            lastSorted = undefined;
        else
            lastSorted = sortByClass;
    });
});