<?php

function evsavingsModels_resource() {
	$api = array(
		'evsavings-models-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get models and models by brandid in evsavings',
			  'callback' => 'getAllModels',
			  'file' => array('type' => 'inc', 'module' => 'evsavingsModels', 'name' => 'includes/evsavingsModels.services'),
			  'access callback' => 'evsavingsModels_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),

			),
		  ),
		),
	);
  return $api;
}
function evsavingsModels_custom_resource_access()
{
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
