<?php 

function paybackRooftopHomevalue_resource() 
{
  $api = array(
		'paybackRooftopHomevalue-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'payback rooftop home value calculator',
			  'callback' => 'get_paybackRooftopHomevalue',
			  'file' => array('type' => 'inc', 'module' => 'paybackRooftopHomevalue', 'name' => 'includes/paybackRooftopHomevalue.services'),
			  'access callback' => 'paybackRooftopHomevalue_custom_resource_access',
       			 'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}

function paybackRooftopHomevalue_custom_resource_access()
{
	return TRUE;
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
//http://localhost/drupal_api/ev_saving/paybackRooftopHomevalue-api?zip=85138

function get_paybackRooftopHomevalue()
{
    $params = drupal_get_query_parameters();
    $lat =$params['lat'];
    $lon =$params['long'];
    $countryName=$params['countryName'];
    $stateCode=$params['stateCode'];
    $zipcodenew=$params['zip'];
    $initialCapacity= 6;
    $minAgeOfSolarSystem= 0;
    $monthlyBill= 150;
	
	//----------------------values from api calls, getting homevalue ----------------------
    $quanteriumPropertyID=get_propertyID($zipcodenew);
    //return $quanteriumPropertyID;
	$homeValueQuantarium=get_homeValue($quanteriumPropertyID);
	//return $homeValueQuantarium;	
	//--------------------values from api calls-------------------------------
	
	//home values Solar calling a funtion 
    $homeValueSolar=getOperatingCf($initialCapacity,$minAgeOfSolarSystem,$zipcodenew,$monthlyBill);
	//return $homeValueSolar;
		
	//homeValueQuantarium = shared.getQuantariumValue(lat, long)

    $finalHomeValue= $homeValueSolar + $homeValueQuantarium;

	$response=array("homeValueWithSolar"=>$finalHomeValue,"estimatedSavings"=>$homeValueSolar,"homeValueWithOutSolar"=>$homeValueQuantarium,"initialCapacity"=>$initialCapacity,"ageOfSolarSystem"=>$minAgeOfSolarSystem);
	return array("homeValueData"=>$response);
	
}


function myfunction($v1,$v2)
{
	return $v1+$v2;
}

function getOM()
{
	return 0;
}

function getOperatingCf($initialCapacity,$minAgeOfSolarSystem,$zipcodenew,$monthlyBill)
{
    $getBilSavings = getBillSavings($initialCapacity, $zipcodenew, $monthlyBill);
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
function getBillSavings($initialCapacity, $zipcodenew,$monthlyBill)
{
	$url="http://34.197.190.88:8888/solarOnlySavings?addr={$zipcodenew}&bill={$monthlyBill}&pv={$initialCapacity}";
   	$res = file_get_contents($url);
	$new = json_decode($res, TRUE);
	foreach($new as $data) 
	{
		$data['annualSavingsElectric'];
	}
			
	return $data;
}


?>