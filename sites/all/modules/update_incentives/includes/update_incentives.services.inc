<?php

function update_incentives_resource() {
	$api = array(
		'incentives-update-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'Update Indentive',
			  'callback' => 'update_incentives',
			  'file' => array('type' => 'inc', 'module' => 'update_incentives', 'name' => 'includes/update_incentives.services'),
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),

			),
		  ),
		),
	);
  return $api;
}
// function _custom_resource_access(){
// 	return TRUE;
// }

function update_incentives(){
	$params = drupal_get_query_parameters();

	$id = $params['id'];
	$des = $params['des'];

	$cr_update_no= db_update('app_incentives_data_v2') // Table name no longer needs {}
                ->fields(array('text' => $des))
                ->condition('id', $id, '=')
                ->execute();

	return $cr_update_no;
}

?>
