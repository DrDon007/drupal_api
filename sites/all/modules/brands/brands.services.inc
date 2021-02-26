<?php

function brands_resource() {
	$api = array(
		'getAllBrands' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all brands',
			  'callback' => 'get_brands',
			  'file' => array('type' => 'inc', 'module' => 'brands', 'name' => 'brands.services'),
			  'access callback' => 'brands_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}

function brands_custom_resource_access()
{
	return TRUE;
}

function get_brands()
{
	$params = drupal_get_query_parameters();
	$stateName = $params['stateName'];
	$limit = $params['limit'];

	if (!empty($stateName) && !empty($limit)) 
    {
		//$result = db_query('SELECT * FROM app_incentives_data_v2 n WHERE n.state = :state LIMIT 2', array(':state' => $stateName))->fetchAll();
		$sql = 'SELECT * FROM app_incentives_data_v2 n WHERE n.state = :state';
		$result1 = db_query_range($sql,0,$limit, array(':state' => $stateName))->fetchAll();
		return $result1;
	}
	else
    {
		$result = db_query('SELECT * FROM `evbrands`')->fetchAll();
		return $result;
	}

}

?>
