<?php

function demo_resource() {
	$api = array(
		'demo-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all incentives',
			  'callback' => 'demo',
			  'file' => array('type' => 'inc', 'module' => 'demo', 'name' => 'includes/demo.services'),
			  'access callback' => '_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),

			),
			'update' => array(
				'help' => 'Incentive Update',
			  'callback' => 'update_incentive',
			  'file' => array('type' => 'inc', 'module' => 'demo', 'name' => 'includes/incentives.services'),
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
function _custom_resource_access(){
	return TRUE;
}



function demo()
{
    $params = drupal_get_query_parameters();

	$stateName = $params['stateName'];
	$limit = $params['limit'];
    $sql = 'SELECT * FROM app_incentives_data_v2 n WHERE n.state = :state';
		$result1 = db_query_range($sql,0,$limit, array(':state' => $stateName))->fetchAll();

		return $result1;
}

?>
