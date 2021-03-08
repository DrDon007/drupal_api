<?php

function payback_resource() {
	$api = array(
		'payback-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all payback, rooftop, homevalue',
			  'callback' => 'get_payback_rooftop_homevalue',
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

function get_payback_rooftop_homevalue(){
	$params = drupal_get_query_parameters();
	//--------------------Calucating values for By Default -------------------------------
	$lat = $params['lat'];
    $long = $params['long'];
	$zipcode = $params['zipcode'];
	$countyName = $params['countyName'];
	$stateCode = $params['stateCode'];
	//By Default Values
	$initialCapacity= 6;
	$minAgeOfSolarSystem= 0;
	$monthlyBill= 150;
	
	if(sizeof($params)==5){
		//Values from paramters
		$zipcode = $params['zipcode'];
		$countyName = $params['countyName'];
		$stateCode = $params['stateCode'];
		$lat = $params['lat'];
		$long = $params['long'];
		// By Default Values
		$initialCapacity= 6;
		$minAgeOfSolarSystem= 0;
		$monthlyBill= 150;

		if(isset($countyName) && isset($stateCode) && isset($zipcode) && isset($lat) && isset($long)){		
		$sql = 'SELECT * FROM app_installation_price n WHERE n.county_name = :county AND n.statecode = :state';
		$result1 = db_query($sql, array(':county' => $countyName, ':state' => $stateCode))->fetchAll();   
		if(!$result1==null){
			$installationPrice = $result1[0]->value*1000;
			$installationPriceAfterITC = ($installationPrice-(($installationPrice * 26)/100)) * 6;
			$data = $installationPriceAfterITC;
		}
		else{
			$installationPrice = 3.2*1000;
			$installationPriceAfterITC = ($installationPrice-(($installationPrice * 26)/100)) * 6;	
			$data = $installationPriceAfterITC;
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
	$response1 = array("payBackSavings"=>abs($data), "payBackPeriod"=>$count, "installationCapacity"=>6, "installationPrice" =>$installationPrice);
 	//return $response1;
     
    $url = "http://3.7.164.225:3000/api/v1/getFeasibilityFactor/?lat={$lat}&long={$long}";
	$feasiblityFactorScore = file_get_contents($url);
	$feasiblityFactorScore = json_decode($feasiblityFactorScore, TRUE);
    $feasiblityFactorScore1 = $feasiblityFactorScore['feasiblityFactorScore'];

    $url = "http://3.7.164.225:3000/api/v1/getUtilityFactor/?lat={$lat}&long={$long}";
	$utilityFactorScore = file_get_contents($url);
	$utilityFactorScore = json_decode($utilityFactorScore, TRUE);
    $utilityFactorScore1 = $utilityFactorScore['utilityFactorScore'];
    $roofTopPotential =  (($feasiblityFactorScore1 * (70/100) + $utilityFactorScore1 * (10/100))/80) * 10;



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
	$response2 =  array("savings"=> abs($data), "roofTopPotential" => floor($roofTopPotential),"message" =>$message,"pvSystemSize" => $pvSystemSize1, "roofTopArea" =>$roofTopArea,"note:"=>$note);

	function get_propertyID($zipcode)
	{
		$quant_property_id="https://qvmservices-test.quantarium.com/QDataService/QueryPropertiesByAddress?u=EnphaseEnergy-Test&k=.6-s3V%3B49%5B1G8qDn&address=5700%20N%20OCEAN%20BLVD%20UNIT%201010&citystate_zip={$zipcode}";
   		$res = file_get_contents($quant_property_id);
   		$property_id_data = json_decode($res, TRUE);
		foreach($property_id_data as $data) 
		{
			$property_id = $data['id'];
		}
		return $property_id;
	}

	function get_homeValue($property_id)
	{
   		$quantariumData="https://qvmservices.quantarium.com/QDataService/GetPropertyRecordsXML?u=EnphaseEnergy&k=csF%2BW94CKg}QmTR%3C&id={$property_id}&records=all";
   		$res = file_get_contents($quantariumData);
   		$quantariumEstimateValue = simplexml_load_string($res);
   		//return $quantariumEstimateValue;
   		$valuation= $quantariumEstimateValue[0];
   		foreach($valuation as $data) 
   		{
			$estimated_value = $data->estimated_value;
   		}
		return (int)$estimated_value;
	}

	//--------------------Calucating Home values -------------------------------
	//home values Solar calling a funtion 
    $homeValueSolar=getOperatingCf($initialCapacity,$minAgeOfSolarSystem,$zipcode,$monthlyBill);
	
	//----------------------values from api calls, getting homevalue ----------------------
	$quanteriumPropertyID=get_propertyID(29582);
	
	$homeValueQuantarium=get_homeValue($quanteriumPropertyID);
    $finalHomeValue= $homeValueSolar + $homeValueQuantarium;
	$response3 = array("homeValueWithSolar"=>$finalHomeValue,"estimatedSavings"=>$homeValueSolar,"homeValueWithOutSolar"=>$homeValueQuantarium,"initialCapacity"=>$initialCapacity,"ageOfSolarSystem"=>$minAgeOfSolarSystem);
	//return array("homeValueData"=>$response);
	
	return $response1+$response2+$response3;
	}
}

function myfunction($v1,$v2)
{
	return $v1+$v2;
}

function getOM()
{
	return 0;
}

function getOperatingCf($initialCapacity,$minAgeOfSolarSystem,$zipcode,$monthlyBill)
{
    $getBilSavings = getBillSavings($initialCapacity, $zipcode, $monthlyBill);
	//return $getBilSavings;
	$finalOM = getOM();
	for($i = $minAgeOfSolarSystem + 0; $i <25; $i++) 
	{
			$cfArray[$i] = round($getBilSavings[$i]) + $finalOM;
	
	}
	$sum = array_reduce($cfArray,"myfunction",0);
	return $sum;						
}

// calculating Bill saving for home value
function getBillSavings($initialCapacity, $zipcode,$monthlyBill)
{
	$url="http://34.197.190.88:8888/solarOnlySavings?addr={$zipcode}&bill={$monthlyBill}&pv={$initialCapacity}";
	//$url="http://34.197.190.88:8888/solarOnlySavings?addr=29582&bill=150&pv=6";
   	$res = file_get_contents($url);
	$new = json_decode($res, TRUE);
	foreach($new as $data) 
	{
		$data['annualSavingsElectric'];
	}
			
	return $data;
}
?>