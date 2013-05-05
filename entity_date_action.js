Drupal.behaviors.entityDateAction = {
  attach: function(context, settings) {
    var max = settings.maxDate;
    if (max) {
      max = new Date(max[0], max[1] -1, max[2]);
    }
    var min = settings.minDate;
    min = new Date(min[0], min[1] -1, min[2]);
    jQuery("#edit-expire").datepicker({
      dateFormat: settings.dateFormat,
      minDate:    min,
      maxDate:    max
    });
  }
}
