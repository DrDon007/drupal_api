<?php

function incentives_resource() {
	$api = array(
		'incentives-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all incentives',
			  'callback' => 'get_incentives',
			  'file' => array('type' => 'inc', 'module' => 'incentives', 'name' => 'includes/incentives.services'),
			  'access callback' => 'incentives_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),

			),
		  ),
		),
	);
  return $api;
}
function incentives_custom_resource_access(){
	return TRUE;
}

function get_incentives()
{
	$params = drupal_get_query_parameters();

	$stateName = $params['stateName'];
	$limit = $params['limit'];
	$key = $params['key'];

	if($key === "pD62NsMZD3" ){

	if(!empty($stateName) && !empty($limit)) {
		//$result = db_query('SELECT * FROM app_incentives_data_v2 n WHERE n.state = :state LIMIT 2', array(':state' => $stateName))->fetchAll();

		$sql = 'SELECT * FROM app_incentives_data_v2 n WHERE n.state = :state';
		$result1 = db_query_range($sql,0,$limit, array(':state' => $stateName))->fetchAll();

		return $result1;
	}
	else{

		$result = db_query('SELECT * FROM app_incentives_data_v2')->fetchAll();
		return $result;
	}

	}else{
		return "not authorized to access this resource/api";
	}


}

/*function update_incentive(){
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
}*/

?>
