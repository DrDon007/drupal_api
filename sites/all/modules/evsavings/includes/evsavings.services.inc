<?php

function evsavings_resource() {
	$api = array(
		'evsavings-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all evsavings',
			  'callback' => 'getAllModels',
			  'file' => array('type' => 'inc', 'module' => 'evsavings', 'name' => 'includes/evsavings.services'),
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

function getAllModels(){
	$params = drupal_get_query_parameters();
	$brandId = $params['brandId'];
	$limit = $params['limit'];

	if (!empty($brandId) && !empty($limit)) {
		//$result = db_query('SELECT * FROM evmodels WHERE n.brandid = :brandid LIMIT 2', array(':state' => $stateName))->fetchAll();

		$sql = 'SELECT * FROM evmodels n WHERE n.brandId = :brandId';
		$result1 = db_query_range($sql,0,$limit, array(':brandId' => $brandId))->fetchAll();

		return $result1;
	}
	else{

		$result = db_query('SELECT * FROM evmodels')->fetchAll();
		return $result;
	}




}

?>
