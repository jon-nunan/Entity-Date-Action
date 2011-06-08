<?php
// $Id: date_action.views.inc,v 1.1 2009/01/26 23:56:49 brmassa Exp $

/**
 * @file
 * Views module integration.
 */

/**
 * Implements hook_views_data().
 */
function date_action_views_data() {
  // Join to 'node' as a base table.
  $data['date_action']['table']['group']  = t('Node');
  $data['date_action']['table']['join'] = array(
    'node' => array(
      'left_field'  => 'nid',
      'field'       => 'nid',
    ),
  );

  $data['date_action']['expire'] = array(
    'title' => t('Expire date'),
    'help'  => t('Date of when the content expire.'),
    'field' => array(
      'handler'         => 'views_handler_field_date',
      'click sortable'  => TRUE,
    ),
    'sort' => array(
      'handler'         => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler'         => 'views_handler_filter_date',
    ),
  );
  $data['date_action']['expired'] = array(
    'title' => t('Expired'),
    'help'  => t('Whether the content is expired.'),
    'field' => array(
      'handler'         => 'views_handler_field_boolean',
      'click sortable'  => TRUE,
    ),
    'filter' => array(
      'handler'         => 'views_handler_filter_boolean_operator',
      'label'           => t('Expired'),
      'type'            => 'yes-no',
    ),
    'sort' => array(
      'handler'         => 'views_handler_sort',
    ),
  );

  return $data;
}
