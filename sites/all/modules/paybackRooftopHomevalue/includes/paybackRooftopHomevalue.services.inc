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
	//$zipcode=29582;
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
  // $quantariumData="https://qvmservices.quantarium.com/QDataService/GetPropertyRecordsXML?u=EnphaseEnergy&k=csF%2BW94CKg}QmTR%3C&id=119857621&records=all";
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


function get_payback($lat,$long,$stateCode,$countryName,$zipCode)
{
	if(isset($countryName) && isset($stateCode) && isset($zipCode) && isset($lat) && isset($long))
	{		
		$sql = 'SELECT * FROM app_installation_price n WHERE n.county_name = :county AND n.statecode = :state';
		$result1 = db_query($sql, array(':county' => $countryName, ':state' => $stateCode))->fetchAll();   
		if(!$result1==null)
		{
			$result2 = $result1[0]->value*1000;
			$result3 = ($result2-(($result2 * 26)/100)) * 6;
			$data = $result3;
		}
		else
		{
			$result2 = 3.2*1000;
			$result3 = ($result2-(($result2 * 26)/100)) * 6;	
			$data = $result3;
		}
    }

	$url = "http://34.197.190.88:8888/solarOnlySavings?addr={$zipCode}&bill=150&pv=6";
	$Response = file_get_contents($url);
	$Response = json_decode($Response, TRUE);
	$length = count($Response['annualSavingsElectric']);
		
	$count = 0;
	for ($i = 0; $i < $length; $i++) 
	{
		$data = $data - ceil($Response['annualSavingsElectric'][$i])."\n";
		if($data>=0){
			$count++;
		}
	}	
	$payBackData=array("payBackValue"=>array("payBackSavings"=>abs($data), "payBackPeriod"=>$count, "installationCapacity"=>6, "installationPrice" =>$result2));
	//return $payBackData;
     
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
    $rooftopData = array("rooftopValue"=> array("savings"=> abs($data), "roofTopPotential" => floor($roofTopPotential),"message" =>$message,"pvSystemSize" => $pvSystemSize1, "roofTopArea" =>$roofTopArea,"note:"=>$note));
    return $payBackData+$rooftopData;
}
 


function get_paybackRooftopHomevalue()
{
    $params = drupal_get_query_parameters();
	$zipCode = $params['zipCode'];
    $lat = $params['lat'];
    $long = $params['long'];
    $countryName=$params['countryName'];
    $stateCode=$params['stateCode'];
    $initialCapacity= 6;
    $minAgeOfSolarSystem= 0;
    //$zipCode=77843,29582;
    $monthlyBill= 150;
		
	//----------------------values from api calls, getting homevalue ----------------------
    $quanteriumPropertyID=get_propertyID($zipCode);
    //return $quanteriumPropertyID;
	$homeValueQuantarium=get_homeValue($quanteriumPropertyID);
	//return $homeValueQuantarium;	
	//--------------------values from api calls-------------------------------
	
	//home values Solar calling a funtion 
    $homeValueSolar=getOperatingCf($initialCapacity,$minAgeOfSolarSystem,$zipCode,$monthlyBill);
	//return $homeValueSolar;
		
    $finalHomeValue= $homeValueSolar + $homeValueQuantarium;

//	$paybackRoofTopData=get_payback($lat,$long,$stateCode,$countryName,$zipCode);
	//$rooftopData=get_rooftop($lat,$long);
	$homeValueData=array("homeValue"=>array("homeValueWithSolar"=>$finalHomeValue,"estimatedSavings"=>$homeValueSolar,"homeValueWithOutSolar"=>$homeValueQuantarium,"initialCapacity"=>$initialCapacity,"ageOfSolarSystem"=>$minAgeOfSolarSystem));
	//return $paybackRoofTopData+$homeValueData;
	return $homeValueData;
	
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
function getBillSavings($initialCapacity, $zipCode,$monthlyBill)
{
	$url="http://34.197.190.88:8888/solarOnlySavings?addr={$zipCode}&bill={$monthlyBill}&pv={$initialCapacity}";
   	$res = file_get_contents($url);
	$new = json_decode($res, TRUE);
	foreach($new as $data) 
	{
		$data['annualSavingsElectric'];
	}
			
	return $data;
}

?>