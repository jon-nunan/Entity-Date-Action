<?php
// $Id: node_expire.rules.inc,v 1.1.2.4 2010/11/10 16:36:58 vikramy Exp $

/**
 * @file
 * Rules module integration.
 */

/**
 * Implements hook_rules_action_info().
 * @ingroup rules
 */
function node_expire_rules_action_info() {
  $defaults = array(
   'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Expired Node'),
        'save' => TRUE,
      ),
    ),
    'group' => t('Node'),
  );

  $items['set_active'] =$defaults + array(
    'label'     => t('Set the active flag'),
    'base'    => 'node_expire_set_active',
  );
  $items['set_inactive'] = $defaults + array(
    'label' => t('Unset the active flag'),
    'base' => 'node_expire_unset_active',
  );	
  $items['set_date'] = $defaults + array(
    'label'     => t('Update date'),
    'base'    => 'node_expire_update_lastnotify',
  );
  $items['set_rule'] = $defaults + array (
    'label' => t('Set rule category'),
	'base' => 'node_expire_set_rule',
  );
  return $items;
}

/**
 * Implements hook_rules_condition_info().
 * @ingroup rules
 * TODO: make an entity to reduce num of variables
 */
function node_expire_rules_condition_info() {
 $defaults = array(
   'parameter' => array(
      'term_list' => array(
        'type' => 'list<integer>',
		'options list' => 'node_expire_rule_types',
        'label' => t('Rule term'),
        'save' => TRUE,
      ),
	  'term' => array(
	    'type' => 'integer',
		'label' => t('Rule'),
		'save' => TRUE,
	  ),
    ),
    'group' => t('Rule type'),
  );

  $items['node_expire_rules_rule_type'] =$defaults + array(
    'label'     => t('Node expire has type'),
	'help'      => 'Evaluates to TRUE, if the provided term matches the rule term.',
    'base'    => 'node_expire_rules_rule_type',
  );
  return $items;
}

/**
 * Implements hook_rules_event_info().
 * @ingroup rules
 */
function node_expire_rules_event_info() {
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


/**
 * Update lastnotify on nodes.
 *
 * @param $node
 *   Object. The Node object
 *   that should update lastnotify.
 */
function node_expire_update_lastnotify($node) {
  db_update('node_expire')
    ->fields(array(
	  'lastnotify' => REQUEST_TIME,
    ))
	->condition('nid', $node->nid)
	->execute();
}


/**
 * Check if the node has the the "Expired" flag on.
 *
 * @param $node
 *   Object. The Node object.
 */
function node_expire_rules_expired_check($node) {
  return (!empty($node->expire) && $node->expire <= REQUEST_TIME && $node->expired == 1);
}



/**
 * Unset the "Expired" flag on nodes.
 *
 * @param $nids
 *   Object. The Node object with all node IDs
 *   that should unset the expired flag.
 */
function node_expire_unset_expired($node) {
  db_update('node_expire')
    ->fields(array(
	  'expired' => 0,
	  ))
	->condition('nid', $node->nid)
	->execute();
}


function node_expire_set_expired($node) {
  db_update('node_expire')
    ->fields(array(
	  'expired' => 1,
	  ))
	->condition('nid', $node->nid)
	->execute();
}


function node_expire_set_active() {

}


function node_expire_unset_active() {

}

function node_expire_set_rule() {

}

function node_expire_rule_types () {
  $vid = variable_get('node_expire_default_vocab');
  $terms = taxonomy_get_tree($vid);
  $options = array();
  foreach($terms as $term) {
    $options[$term->tid] = $term->name;
  }

  return $options;
}


function node_expire_rules_rule_type($term_list, $term) {
  watchdog('node expire rule', print_r($term_list, true), NULL);
  watchdog('node expire rule2', print_r($term, true), NULL);
  dpm($term_list);
  dpm($term);
  return in_array($term, $term_list);
}