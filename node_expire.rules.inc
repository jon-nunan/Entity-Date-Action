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

  $items['unset_expired'] =$defaults + array(
    'label'     => t('Unset the expired flag'),
    'base'    => 'node_expire_unset_expired',
  );
  $items['set_expired'] = $defaults + array(
    'label' => t('Set the expired flag'),
    'base' => 'node_expire_set_expired',
  );	
  $items['update_lastnotify'] = $defaults + array(
    'label'     => t('Update lastnotify'),
    'base'    => 'node_expire_update_lastnotify',
  );
  return $items;
}

/**
 * Implements hook_rules_condition_info().
 * @ingroup rules
 */
function node_expire_rules_condition_info() {
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

  $items['node_expire_rules_expired_check'] =$defaults + array(
    'label'     => t('Content is expired'),
	'help'      => 'Evaluates to TRUE, if the given content has one of the selected content types.',
    'base'    => 'node_expire_rules_expired_check',
  );

  $items['node_expire_rules_expired_check_lastnotify'] = array(
    'label'     => t('Content is expired: Check lastnotify'),
	'help'      => 'Evaluates to TRUE, if the given content is expired and lastnotified is older than the days value.',
	'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Expired Node'),
        'save' => TRUE,
      ),
	  'days' => array(
	    'type' => 'integer',
		'label' => 'Days before sending followup reminders',
		'save' => TRUE,
	  ),
    ),
    'base'  => 'node_expire_rules_expired_check_lastnotify',
	'group' => t('Node'),
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
 * Check if the node lastnotify is greater than 
 * or equal to days supplied.
 * @param $node
 *   Object. The Node object.
 */
function node_expire_rules_expired_check_lastnotify($node, $days) {
  return ($node->lastnotify <= (REQUEST_TIME - ($days * 24 * 60 * 60)));
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

