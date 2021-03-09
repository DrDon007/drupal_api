<?php

function payback_rooftop_homevalue_resource() {
	$api = array(
		'finetune-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get all payback, rooftop, homevalue for finetune values',
			  'callback' => 'get_finetune',
			  'file' => array('type' => 'inc', 'module' => 'payback_rooftop_homevalue', 'name' => 'includes/payback_rooftop_homevalue.services'),
			  'access callback' => 'finetune_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}
function finetune_resource_access(){
	return TRUE;
}

function get_finetune(){
	$params = drupal_get_query_parameters();
	$zipcode = $params['zipCode'];
    $lat = $params['lat'];
    $long = $params['long'];
    $initialCapacity = $params['initialCapacity'];
    $installationPrice = $params['installationPrice'];
    $sunRoofArea = $params['sunRoofArea'];
    $ageOfSolar = $params['ageOfSolar'];
    $bill = $params['bill'];
		$key = $params['key'];
		$key_access_val = 'pD62NsMZD3';
		if( $key === $key_access_val){
	if(isset($initialCapacity) && isset($installationPrice) && isset($zipcode) && isset($lat) && isset($long) && isset($sunRoofArea) && isset($ageOfSolar) && isset($bill)){

     $data  = ($installationPrice-(($installationPrice * 26)/100)) * 6;

    $url = "http://34.197.190.88:8888/solarOnlySavings?addr={$zipcode}&bill={$bill}&pv=6";
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
       $roofTopArea1 = $sunRoofArea;
         }
   else{
      $pvSystemSize1 = 6;
      $roofTopArea1 = $sunRoofArea;
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
    $response1 =  array("savings"=> abs($data), "roofTopPotential" => floor($roofTopPotential),"message" =>$message,"pvSystemSize" => $pvSystemSize1, "roofTopArea" =>$roofTopArea1,"note:"=>$note);
    $response2 = array("payBackSavings"=>abs($data), "payBackPeriod"=>$count, "installationCapacity"=>6, "installationPrice" =>$installationPrice);


}
}else {
	return array(warning=>"Not authorized to access this resource/api",error=>"403 Forbidden");

}
  $homeValueSolar=getOperatingCf($initialCapacity,$ageOfSolar,$zipcode,$bill);

	//----------------------values from api calls, getting homevalue ----------------------
	$quanteriumPropertyID=get_propertyID($zipcode);

	$homeValueQuantarium=get_homeValue($quanteriumPropertyID);
    $finalHomeValue= $homeValueSolar + $homeValueQuantarium;
	$response3=array("homeValueWithSolar"=>$finalHomeValue,"estimatedSavings"=>$homeValueSolar,"homeValueWithOutSolar"=>$homeValueQuantarium,"initialCapacity"=>$initialCapacity,"ageOfSolarSystem"=>$ageOfSolar);
     $response= array("payback" => $response2, "rooftop" => $response1, "homevalue" => $response3);
     return $response;
}

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
		//return $estimated_value;
   }
	return (int)$estimated_value;
}

function myfunction($v1,$v2)
{
	return $v1+$v2;
}

function getOM()
{
	return 0;
}

function getOperatingCf($initialCapacity,$ageOfSolar,$zipcode,$bill)
{
    $getBilSavings = getBillSavings($initialCapacity, $zipcode, $bill);
	//return $getBilSavings;
	$finalOM = getOM();
	for($i = $ageOfSolar + 0; $i <25; $i++)
	{
			$cfArray[$i] = round($getBilSavings[$i]) + $finalOM;

	}
	$sum = array_reduce($cfArray,"myfunction",0);
	return $sum;
}

// calculating Bill saving for home value
function getBillSavings($initialCapacity, $zipcode,$bill)
{
	$url="http://34.197.190.88:8888/solarOnlySavings?addr={$zipcode}&bill={$bill}&pv={$initialCapacity}";
	//$url="http://34.197.190.88:8888/solarOnlySavings?addr=29582&bill=150&pv=6";
   	$res = file_get_contents($url);
	$new = json_decode($res, TRUE);
	foreach($new as $data)
	{
		$data['annualSavingsElectric'];
	}

	return $data;
}
