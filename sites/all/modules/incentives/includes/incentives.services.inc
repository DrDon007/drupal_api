<?php

function incentives_resource() {
	$api = array(
		'incentives-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all incentives',
			  'callback' => 'get_incentives',
			  'file' => array('type' => 'inc', 'module' => 'incentives', 'name' => 'includes/incentives.services'),
			  'access callback' => '_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),

			),
			'update' => array(
				'help' => 'Incentive Update',
			  'callback' => 'update_incentive',
			  'file' => array('type' => 'inc', 'module' => 'incentives', 'name' => 'includes/incentives.services'),
			  'access callback' => '_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}

function _custom_resource_access()
{
	return TRUE;
}

function get_incentives(){
	$params = drupal_get_query_parameters();

	$stateName = $params['stateName'];
	$limit = $params['limit'];

	if (!empty($stateName) && !empty($limit)) {
		//$result = db_query('SELECT * FROM app_incentives_data_v2 n WHERE n.state = :state LIMIT 2', array(':state' => $stateName))->fetchAll();

		$sql = 'SELECT * FROM app_incentives_data_v2 n WHERE n.state = :state';
		$result1 = db_query_range($sql,0,$limit, array(':state' => $stateName))->fetchAll();

		return $result1;
	}
	else{

		$result = db_query('SELECT * FROM app_incentives_data_v2')->fetchAll();
		return $result;
	}




}

function update_incentive(){
	$params = drupal_get_query_parameters();

	$stateName = $params['stateName'];
	$limit = $params['limit'];

	if (!empty($stateName) && !empty($limit)) {
		//$result = db_query('SELECT * FROM app_incentives_data_v2 n WHERE n.state = :state LIMIT 2', array(':state' => $stateName))->fetchAll();

		$sql = 'SELECT * FROM app_incentives_data_v2 n WHERE n.state = :state';
		$result1 = db_query_range($sql,0,$limit, array(':state' => $stateName))->fetchAll();

		return $result1;
	}
	else{

		$result = db_query('SELECT * FROM app_incentives_data_v2')->fetchAll();
		return $result;
	}
}

?>
