Drupal.behaviors.fieldModelBehavior = {
  attach: function (context, settings) {
	  $('select[name^="field_model"]').bind('change',function() {
          // Add parent() to hide input wrapper
          var selected_other = 'none';
          $(this).children(':selected').each(function() {
            selected_other = ($(this).val() == 'other') ? 'block' : 'none';
          });
          
          $('input[name^="field_model"]').parent().css('display', selected_other);
        });
	  var default_model = $('input[name="select_other_hidden"]').val();
	  $('select[name^="field_model"]').val(default_model);
	  $('select[name^="field_model"]').change();
  }
};