<?php
// $Id: node_expire.nodeapi.inc,v 1.8.2.3 2010/11/30 01:19:01 vikramy Exp $

/**
 * @file
 * Node API integration.
 */



function _node_expire_form_alter_nodeform(&$ntypes, &$form, &$form_state, $form_id) {
  // Check if the Node expire feature is enabled for the node type
  $node = isset($form['#node']) ? $form['#node'] : NULL;
  // Convert the timestamp into a human readable date
  if (isset($node->expire) && is_numeric($node->expire)) {
    $node->expire = format_date($node->expire, 'custom', NODE_EXPIRE_FORMAT);
  } else {
    $node->expire = '';
  }
  // This supports node to never expire.
  if (empty($ntypes[$node->type]['default']) && empty($node->expire)) {
    $ntypes[$node->type]['required'] = FALSE;
  } 
  if (user_access('edit node expire')) {
    $expire_field = array(
      '#title'         => t('Expiration date'),
      '#description'   => t('Time date to consider the node expired. Format: %time (YYYY-MM-DD).',
        array('%time' => format_date(REQUEST_TIME, 'custom', NODE_EXPIRE_FORMAT))),
      '#type'          => 'textfield',
      '#maxlength'     => 25,
	  '#size' => 40,
      '#required'      => $ntypes[$node->type]['required'],
      '#default_value' => $node->expire,
    );

    drupal_add_library('system', 'ui.datepicker');
    drupal_add_js(drupal_get_path('module', 'node_expire') . '/node_expire.js');
    $min = empty($node->created) ? REQUEST_TIME : $node->created;
    $min = array(
      format_date($min, 'custom', 'Y'),
      format_date($min, 'custom', 'm'),
      format_date($min, 'custom', 'd'),
    );
    if (!empty ($ntypes[$node->type]['max'])) {
      $max = strtotime($ntypes[$node->type]['max'], (empty($node->created) ? REQUEST_TIME : $node->created));
      $max = array(
        format_date($max, 'custom', 'Y'),
        format_date($max, 'custom', 'm'),
        format_date($max, 'custom', 'd'),
      );
    }
    else {
      $max = NULL;
    }
    drupal_add_js(array(
      'dateFormat'  => NODE_EXPIRE_FORMAT_JS,
      'minDate'     => $min,
      'maxDate'     => $max,
    ), array('type' => 'setting', 'scope' => JS_DEFAULT));

  }
  else {
    $expire_field = array(
      '#type'         => 'value',
      '#value'        => $node->expire
    );
  }

  // Put the expire field into 'Publising options' if possible
  if (user_access('administer nodes')) {
    $form['options']['expire']  = &$expire_field;
  }
  else {
    $form['expire']             = &$expire_field;
  }

  if (isset($node->expired)) {
    $form['node_expire'] = array(
      '#type'         => 'value',
      '#value'        => TRUE,
    );
  }
}
