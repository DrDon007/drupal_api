<?php

function payback_resource() {
	$api = array(
		'payback-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all payback, rooftop, homevalue',
			  'callback' => 'get_payback',
			  'file' => array('type' => 'inc', 'module' => 'payback', 'name' => 'includes/payback.services'),
			  'access callback' => 'payback_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}
function payback_resource_access(){
	return TRUE;
}

function get_payback(){
	$params = drupal_get_query_parameters();
	$zipcode = $params['zipcode'];
	
	$countyName = $params['countyName'];
	$stateCode = $params['stateCode'];


	
	//return $Response['annualSavingsElectric'];
	
	if (!empty($countyName) && !empty($stateCode)) {		
		
		$sql = 'SELECT * FROM app_installation_price n WHERE n.county_name = :county AND n.statecode = :state';
		$result1 = db_query($sql, array(':county' => $countyName, ':state' => $stateCode))->fetchAll();   
		$result2 = $result1[0]->value*1000;
		$result3 = ($result2-(($result2 * 26)/100)) * 6;
		$data = $result3;
	
		$url = "http://34.197.190.88:8888/solarOnlySavings?addr={$zipcode}&bill=150&pv=6";
		$Response = drupal_http_request("http://34.197.190.88:8888/solarOnlySavings?addr={$zipcode}&bill=150&pv=6");
		$Response = file_get_contents($url);
		$Response = json_decode($Response, TRUE);
		$arr[] = $Response['annualSavingsElectric'];
		//var_dump($Response['annualSavingsElectric']);
		$length = count($Response['annualSavingsElectric']);
		for ($i = 0; $i < $length; $i++) {
			$data = $data - ceil($Response['annualSavingsElectric'][$i])."\n";
		}
	}
	else{
			$result = 3.2*1000;
			return $result;
	}
}
?>
