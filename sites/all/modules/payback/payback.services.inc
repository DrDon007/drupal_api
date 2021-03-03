<?php

function payback_resource() {
	$api = array(
		'payback-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all payback, rooftop, homevalue',
			  'callback' => 'get_payback',
			  'file' => array('type' => 'inc', 'module' => 'payback', 'name' => 'payback.services'),
			  'access callback' => 'payback_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),

			),
		  ),
		),
	);
  return $api;
}

function payback_custom_resource_access()
{
	return TRUE;
}

function get_payback()
{
	//http://localhost/drupal_api/test/payback-api?countyName=Abbeville&stateCode=SC
	//http://34.197.190.88:8888/solarOnlySavings?addr=95054&bill=150&pv=5
	
	$params = drupal_get_query_parameters();
    //$lat = $params['lat'];
    //$long = $params['long'];
    $countyName = $params['countyName'];
	$stateCode = $params['stateCode'];
	//$zipCode = $params['zipCode'];

	if(!empty($countyName) && !empty($stateCode)) 
	{
		$result1=db_query("SELECT * FROM app_installation_price n WHERE n.county_name='$countyName' AND n.statecode='$stateCode'")->fetchAll();
		return $result1;
	}
	else
	{
		$result = db_query('SELECT * FROM app_installation_price')->fetchAll();
		return $result;
	}
	
}

?>