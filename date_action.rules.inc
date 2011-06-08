<?php

/**
 * @file
 * Rules module integration.
 */

/**
 * Implements hook_rules_action_info().
 * @ingroup rules
 */
function date_action_rules_action_info() {
  $defaults = array(
   'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Expired Node'),
        'save' => TRUE,
      ),
	  'field_name' => array(
	    'type' => 'text',
        'label' => t('Field name'),
	  ),
	  'delta' => array(
	    'type' => 'integer',
        'label' => t('Field delta'),
	  ),
    ),
    'group' => t('Date Rules'),
  );

  $items['set_active'] =$defaults + array(
    'label'     => t('Set the active flag'),
    'base'    => 'date_action_set_active',
  );
  $items['set_inactive'] = $defaults + array(
    'label' => t('Unset the active flag'),
    'base' => 'date_action_unset_active',
  );	
  $items['set_date'] = $defaults + array(
    'label'     => t('Update date'),
    'base'    => 'date_action_update_lastnotify',
  );
  $items['set_rule'] = array (
    'parameter' => array(
	  'node' => array(
        'type' => 'node',
        'label' => t('Expired Node'),
        'save' => TRUE,
      ),
	  'field_name' => array(
	    'type' => 'text',
        'label' => t('Field name'),
        'save' => TRUE,
	  ),
	  'delta' => array(
	    'type' => 'integer',
        'label' => t('Field delta'),
        'save' => TRUE,
	  ),
	  'new_rule' =>array(
	    'type' => 'list<integer>',
		'options list' => 'date_action_rule_types',
		'label' => t('Rule TID'),
		'save' => TRUE,
	  ),
	),
    'label' => t('Set rule category'),
	'base' => 'date_action_set_rule',
	'group' => t('Date Rules'),
  );
  
   $items['add_days'] = array (
    'parameter' => array(
	  'node' => array(
        'type' => 'node',
        'label' => t('Expired Node'),
        'save' => TRUE,
      ),
	  'field_name' => array(
	    'type' => 'text',
        'label' => t('Field name'),
        'save' => TRUE,
	  ),
	  'delta' => array(
	    'type' => 'integer',
        'label' => t('Field delta'),
        'save' => TRUE,
	  ),
	  'days_add' =>array(
	    'type' => 'integer',
		'label' => t('Days from now'),
		'save' => TRUE,
	  ),
	),
    'label' => t('Add days to date action field'),
	'base' => 'date_action_add_days',
	'group' => t('Date Rules'),
  );
  
  return $items;
}

/**
 * Implements hook_rules_condition_info().
 * @ingroup rules
 * TODO: make an entity to reduce num of variables
 */
function date_action_rules_condition_info() {
 $defaults = array(
   'parameter' => array(
      'term_list' => array(
        'type' => 'list<integer>',
		'options list' => 'date_action_rule_types',
        'label' => t('Term for date action'),
        'save' => TRUE,
      ),
	  'term' => array(
	    'type' => 'integer',
		'label' => t('Rule'),
		'save' => TRUE,
	  ),
    ),
    'group' => t('Date action'),
  );

  $items['date_action_rules_rule_type'] =$defaults + array(
    'label'     => t('Node expire has type'),
	'help'      => 'Evaluates to TRUE, if the provided term matches the rule term.',
    'base'    => 'date_action_rules_rule_type',
  );
  return $items;
}

/**
 * Implements hook_rules_event_info().
 * @ingroup rules
 */
function date_action_rules_event_info() {
  return array(
    'node_expired' => array(
      'label'     => t('Content expired'),
	  'group' => t('Node'),
	  'variables' => array(
	    'expired' => array('type' => 'node', 'label' => t('content expired')),
		'term' => array('type' => 'integer', 'label' => t('term id')),
		'field' => array('type' => 'text' , 'label' => t('field name')),
		'delta' => array('type' => 'integer', 'label' => t('delta'))
	  ),
    ),
  );
}


/*
TODO: remove ugly admin hack, should only have permissions it needs!
*/


function date_action_set_active($node, $field, $delta) {
  $date_action = $node->$field;
  $date_action['und'][$delta]['active'] = 1;
  $node->$field = $date_action;
  node_save($node);
}

function date_action_unset_active($node, $field, $delta) {
  $date_action = $node->$field;
  $date_action['und'][$delta]['active'] = 0;
  $node->$field = $date_action;
  node_save($node);
}

function date_action_set_rule($node, $field, $delta, $new_rule) {
   $date_action = $node->$field;
   $date_action['und'][$delta]['tid'] = array_pop($new_rule);
   $node->$field = $date_action;
   node_save($node);

   // return false, otherwise an error is thrown. Not sure why though.
   return false;
}

function date_action_add_days($node, $field, $delta, $day_add) {

  $new_date = REQUEST_TIME + $day_add*24*60*60;
  $date_action = $node->$field;
  $date_action['und'][$delta]['date'] = date("Y-m-d H:i:s", $new_date);
  $node->$field = $date_action;
  node_save($node);
  
}



function date_action_rule_types () {
  $vid = variable_get('date_action_default_vocab');
  $terms = taxonomy_get_tree($vid);
  $options = array();
  foreach($terms as $term) {
    $options[$term->tid] = $term->name;
  }

  return $options;
}


function date_action_rules_rule_type($term_list, $term) {
  return in_array($term, $term_list);
}

function date_action_update_lastnotify() {
  return;
}