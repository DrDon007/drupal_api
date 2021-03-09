<?php

function evsavingsBrands_resource() {
	$api = array(
		'evsavings-brands-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all brands',
			  'callback' => 'get_all_brands',
			  'file' => array('type' => 'inc', 'module' => 'evsavingsBrands', 'name' => 'includes/evsavingsBrands.services'),
			  'access callback' => 'evsavingsBrands_custom_resource_access',

			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}

function evsavingsBrands_custom_resource_access(){
	return TRUE;
}

function get_all_brands(){
	$params = drupal_get_query_parameters();
	$key = $params['key'];
	$key_access_val = 'pD62NsMZD3';
	if( $key === $key_access_val){
        $result = db_query('SELECT * FROM evbrands')->fetchAll();
		return $result;
	}else {
		return array(warning=>"Not authorized to access this resource/api",error=>"403 Forbidden");

	}
}
