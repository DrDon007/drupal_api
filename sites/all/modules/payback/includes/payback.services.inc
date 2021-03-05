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
	
	if(isset($countyName) && isset($stateCode) && isset($zipcode)){		
		$sql = 'SELECT * FROM app_installation_price n WHERE n.county_name = :county AND n.statecode = :state';
		$result1 = db_query($sql, array(':county' => $countyName, ':state' => $stateCode))->fetchAll();   
		if(!$result1==null){
			$result2 = $result1[0]->value*1000;
			//echo "Installation Price: ".$result2."\n";
			$result3 = ($result2-(($result2 * 26)/100)) * 6;
			$data = $result3;
		}
		else{
			$result2 = 3.2*1000;
			//echo "Installation Price: ".$result2."\n";
			$result3 = ($result2-(($result2 * 26)/100)) * 6;	
			$data = $result3;
		}
	}
	$url = "http://34.197.190.88:8888/solarOnlySavings?addr={$zipcode}&bill=150&pv=6";
	$Response = drupal_http_request("http://34.197.190.88:8888/solarOnlySavings?addr={$zipcode}&bill=150&pv=6");
	$Response = file_get_contents($url);
	$Response = json_decode($Response, TRUE);
	$length = count($Response['annualSavingsElectric']);
		
	$count = 0;
	for ($i = 0; $i < $length; $i++) {
		$data = $data - ceil($Response['annualSavingsElectric'][$i])."\n";
		if($data>=0){
			$count++;
		}
	}	
	$response=array("payBackSavings"=>abs($data), "payBackPeriod"=>$count, "installationCapacity"=>6, "installationPrice" =>$result2);
	return $response;
}

?>