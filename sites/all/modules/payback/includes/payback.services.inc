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
    $lat = $params['lat'];
    $long = $params['long'];
	
	if(isset($countyName) && isset($stateCode) && isset($zipcode) && isset($lat) && isset($long)){		
		$sql = 'SELECT * FROM app_installation_price n WHERE n.county_name = :county AND n.statecode = :state';
		$result1 = db_query($sql, array(':county' => $countyName, ':state' => $stateCode))->fetchAll();   
		if(!$result1==null){
			$result2 = $result1[0]->value*1000;
			$result3 = ($result2-(($result2 * 26)/100)) * 6;
			$data = $result3;
		}
		else{
			$result2 = 3.2*1000;
			$result3 = ($result2-(($result2 * 26)/100)) * 6;	
			$data = $result3;
		}
    }
	$url = "http://34.197.190.88:8888/solarOnlySavings?addr={$zipcode}&bill=150&pv=6";
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
 	//return $response;
     
    $url = "http://3.7.164.225:3000/api/v1/getFeasibilityFactor/?lat={$lat}&long={$long}";
	$feasiblityFactorScore = file_get_contents($url);
	$feasiblityFactorScore = json_decode($feasiblityFactorScore, TRUE);
    $feasiblityFactorScore1 = $feasiblityFactorScore['feasiblityFactorScore'];

    $url = "http://3.7.164.225:3000/api/v1/getUtilityFactor/?lat={$lat}&long={$long}";
	$utilityFactorScore = file_get_contents($url);
	$utilityFactorScore = json_decode($utilityFactorScore, TRUE);
    $utilityFactorScore1 = $utilityFactorScore['utilityFactorScore'];
    $roofTopPotential =  (($feasiblityFactorScore1 * (70/100) + $utilityFactorScore1 * (10/100))/80) * 10;
   // return floor($roofTopPotential);



  $url = "http://3.7.164.225:3000/api/v1/getSunRoof?lat={lat}&long={long}";
	$pvSystemSize = file_get_contents($url);
	$pvSystemSize= json_decode($pvSystemSize, TRUE);
   if($pvSystemSize['statusCode'] == 200){
       $pvSystemSize1 = $pvSystemSize['pvSystemSize'];
       $roofTopArea = $pvSystemSize1 * 100;
         }
   else{
      $pvSystemSize1 = 6;
      $roofTopArea = 600;
      $note =  "The exact rooftop information for this address is not available right now. Please enter your rooftop area to get a more accurate estimate.";
   }

	if($feasiblityFactorScore1 > 70) {
    	if ($utilityFactorScore1 > 70) {
      		$message = "This home is ideal for high solar energy production and can save a lot on utility bills with a solar installation";
    }
    if($utilityFactorScore1 > 50 && $utilityFactorScore1 < 70) {
      	$message = "This home is ideal for high solar energy production and can save a lot on utility bills with a solar installation.";
    }
    if($utilityFactorScore1 > 30 && $utilityFactorScore1 < 50) {
      	$message = "This home is ideal for high solar energy production.";
    }
    if($utilityFactorScore1 < 30) {
      	$message = "This home is ideal for high solar energy production.";
    }
  	}
  	if ($feasiblityFactorScore1 > 50 && $feasiblityFactorScore1 < 70) {
    	if (utility > 70) {
      	$message = "This home is ideal for high solar energy production and can save a lot on utility bills with a solar installation";
    }
    if ($utilityFactorScore1 > 50 && $utilityFactorScore1 < 70) {
      	$message = "This home is ideal for high solar energy production and can save a lot on utility bills with a solar installation.";
    }
    if ($utilityFactorScore1 > 30 && $utilityFactorScore1 < 50) {
      	$message = "This home is ideal for high solar energy production.";
    }
    if ($utilityFactorScore1 < 30) {
      	$message = "This home is ideal for high solar energy production.";
    }
  }
  if ($feasiblityFactorScore1 > 30 && $feasiblityFactorScore1 < 50) {
    if ($utilityFactorScore1 > 70) {
      	$message = "Your home is not currently optimal for solar installation. You may still enjoy high solar output and save on utility bills. Check our knowledge section to learn how to optimize for solar power generation.";
    }
    if ($utilityFactorScore1 > 50 && $utilityFactorScore1 < 70) {
      	$message = "Your home is not currently optimal for solar installation. You may still enjoy high solar output and save on utility bills. Check our knowledge section to learn how to optimize for solar power generation.";
    }
    if ($utilityFactorScore1 > 30 && $utilityFactorScore1 < 50) {
      	$message = "Your home is not currently optimal for solar installation. Check our knowledge section to learn how to optimize for solar power generation.";
    }
    if ($utilityFactorScore1 < 30) {
      	$message = "Your home is not currently optimal for solar installation. Check our knowledge section to learn how to optimize for solar power generation.";
    }
  }
  if ($feasiblityFactorScore1 < 30) {
    if (utility > 70) {
      	$message = "Your home is not currently optimal for solar installation. Check our knowledge section to learn how to optimize for solar power generation.";
    }
    if ($utilityFactorScore1 > 50 && $utilityFactorScore1 < 70) {
      	$message = "Your home is not currently optimal for solar installation. Check our knowledge section to learn how to optimize for solar power generation.";
    }
    if ($utilityFactorScore1 > 30 && $utilityFactorScore1 < 50) {
      	$message = "Your home is not currently optimal for solar installation. Check our knowledge section to learn how to optimize for solar power generation.";
    }
    if ($utilityFactorScore1 < 30) {
      	$message = "Your home is not currently optimal for solar installation. Check our knowledge section to learn how to optimize for solar power generation.";
    }
  }
    $response =  array("savings"=> abs($data), "roofTopPotential" => floor($roofTopPotential),"message" =>$message,"pvSystemSize" => $pvSystemSize1, "roofTopArea" =>$roofTopArea,"note:"=>$note);
    return $response;
}
 

?>