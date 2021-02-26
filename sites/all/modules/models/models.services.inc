<?php

function models_resource() {
	$api = array(
		'getAllModels' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all models',
			  'callback' => 'get_models',
			  'file' => array('type' => 'inc', 'module' => 'brands', 'name' => 'brands.services'),
			  'access callback' => '_models_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}

function _models_custom_resource_access()
{
	return TRUE;
}

function get_models()
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
