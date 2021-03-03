<?php

function evmodels_resource() {
	$api = array(
		'getAllModels' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all ev models',
			  'callback' => 'get_evmodels',
			  'file' => array('type' => 'inc', 'module' => 'evmodels', 'name' => 'evmodels.services'),
			  'access callback' => 'evmodels_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}

function evmodels_custom_resource_access()
{
	return TRUE;
}

function get_evmodels()
{
	$params = drupal_get_query_parameters();
	$BrandID = $params['id'];

	if (!empty($BrandID)) 
    {
		$result = db_query("SELECT * FROM `evmodels` where brandId='$BrandID'")->fetchAll();
		return $result;
	}
	else
    {
		$result = db_query('SELECT * FROM `evmodels`')->fetchAll();
		return $result;
	}

}

?>
