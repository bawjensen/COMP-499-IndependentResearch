var requestNumber = 0;

$(function() {
    // console.log()

    $('.option').click(function() {
        $(this).toggleClass('selected');
    });

    $('#go').click(function() {
        var selected = [];

        $('.selected').each(function(i, ele) {
            selected.push($(ele).text());
        });

        $.ajax({
            url: '/',
            method: 'POST',
            data: {
                selected: selected,
                reqNum: requestNumber++
            },
            success: function(data, status) {
                // console.log('yay');
                // console.log(status);
                // console.log(data);

                $('#plot').remove();

                $('<img>', {
                    id: 'plot',
                    src: data
                }).appendTo($('body'));
            }
        });
    });
});