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
	$params = drupal_get_query_parameters();

    //$lat = $params['lat'];
    //$long = $params['long'];
    $countyName = $params['countyName'];
	$stateCode = $params['stateCode'];
	//$zipCode = $params['zipCode'];
	//var_dump($params);



	/*$response = drupal_http_request("http://34.197.190.88:8888/solarOnlySavings?addr=95054&bill=150&pv=5");
    return $response->data;*/

	if (!empty($countyName) && !empty($stateCode)) {
		//$result = db_query('SELECT * FROM app_installation_price n WHERE n.county_name = :countyName AND n.statecode = :stateCode' , array(':county_name' => $countyName))->fetchAll();

		$sql = 'SELECT value FROM app_installation_price n WHERE n.county_name = :countyName AND n.statecode = :stateCode';
		$result1 = db_query($sql, array(':countyName' => $countyName, ':stateCode' => $stateCode))->fetchAll();
		//return $result1;
		
	}
	else{

		$result = db_query('SELECT * FROM app_installation_price')->fetchAll();
		return $result;
	}

}

?>
