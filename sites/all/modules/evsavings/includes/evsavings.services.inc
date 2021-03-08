<?php

function evsavingsBrands_resource() {
	$api = array(
		'evsavings-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all brands',
			  'callback' => 'get_all_brands',
			  'file' => array('type' => 'inc', 'module' => 'evsavings', 'name' => 'includes/evsavings.services'),
			  'access callback' => 'evsavingsBrands_resource_access',
			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}

//function evsavingsBrands_resource_access(){
//	return TRUE;
//}

function get_all_brands(){
	
        $result = db_query('SELECT * FROM evbrands')->fetchAll();
		return $result;
}


