<?php

function payback_resource() {
	$api = array(
		'payback-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all payback, rooftop, homevalue',
			  'callback' => 'get_payback',
			  'file' => array('type' => 'inc', 'module' => 'payback', 'name' => 'includes/payback.services'),
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

function get_payback(){
	//$url = "http://34.197.190.88:8888/solarOnlySavings?addr={$zipcode}&bill=150&pv=5";
	//$Response = file_get_contents($url);
	//$Response = json_decode($Response, TRUE);

	$params = drupal_get_query_parameters();
	
	//$zipcode = $params['zipcode'];
	$countyName = $params['countyName'];
	$stateCode = $params['stateCode'];
	//var_dump($countyName."+".$stateCode);

	
	//return $Response['annualSavingsElectric'];
	
	if (!empty($countyName) && !empty($stateCode)) {		
		
		$sql = 'SELECT value FROM app_installation_price n WHERE n.county_name = :county AND n.statecode = :state';
		$result1 = db_query($sql, array(':county' => $countyName, ':state' => $stateCode))->fetchAll();   
		$result2 = $result1['value']*1000;
		return $result2;
	}
	else{
			$result = 3.2*1000;
			return $result;
	}
}
?>
