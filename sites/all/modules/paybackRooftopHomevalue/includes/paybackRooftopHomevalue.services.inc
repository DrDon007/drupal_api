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

function get_paybackRooftopHomevalue()
{
  //  $params = drupal_get_query_parameters();
  //  $lat =$params['lat'];
 //   $lon =$params['lon'];
 //   $countryName=$params['countryName'];
 //   $stateCode=$params['stateCode'];
//    $zipCode=$params['zipCode'];
    $initialCapacity= 600;
   $minAgeOfSolarSystem= 0;
   $zipCode;
   $monthlyBill= 150;
    
  $quant_property_id="https://qvmservices-test.quantarium.com/QDataService/QueryPropertiesByAddress?u=EnphaseEnergy-Test&k=.6-s3V%3B49%5B1G8qDn&address=5700%20N%20OCEAN%20BLVD%20UNIT%201010&citystate_zip=29582";
  $data=drupal_http_request("https://qvmservices-test.quantarium.com/QDataService/QueryPropertiesByAddress?u=EnphaseEnergy-Test&k=.6-s3V%3B49%5B1G8qDn&address=5700%20N%20OCEAN%20BLVD%20UNIT%201010&citystate_zip=29582");
  $res=json_encode($data);
  return $res;
  //$quanthomeValue=get_homeValue();
 // return $quanthomeValue;
		
    //home values Solar calling a funtion 
    //homeValueSolar=getOperatingCf($initialCapacity,$minAgeOfSolarSystem,$zipCode,$monthlyBill);

	//this value will come from the api for home value
	  //homeValueQuantarium = shared.getQuantariumValue(lat, long)
    //finalHomeValue= homeValueSolar + homeValueQuantarium.homevaue;

	/* final result 
			 homeValueWithSolar: finalHomeValue;
            estimatedSavings: homeValueSolar;
            homeValueWithOutSolar: Number(homeValueQuantarium.homeValue)
            initialCapacity: initialCapacity;
            ageOfSolarSystem: minAgeOfSolarSystem,
	*/
}


//-------------------------------------by Preet------------------------------------------

function get_homeValue()
{
  $quant_property_id="https://qvmservices-test.quantarium.com/QDataService/QueryPropertiesByAddress?u=EnphaseEnergy-Test&k=.6-s3V%3B49%5B1G8qDn&address=5700%20N%20OCEAN%20BLVD%20UNIT%201010&citystate_zip=29582";
  $data=drupal_http_request($quant_property_id);
  $response=json_decode($data);
  return $response;
 // $quant_property_records="https://qvmservices.quantarium.com/QDataService/GetPropertyRecordsXML?u=EnphaseEnergy&k=csF%2BW94CKg%7DQmTR%3C&id=12345678&records=all";
}

//---------------------------------------end-------------------------------------------


//--------------------------------------By Poshan------------------------------------

function getOperatingCf($initialCapacity,$minAgeOfSolarSystem,$zipCode,$monthlyBill)
{
    $getBilSavings = getBillSavings(installedCapacity, zipcode, bill);
	  /*this is node js code have to write in php 
							let cfMap = new Map();
						let cfArray = [];
						let finalOM = paybackService.getOM();
						for (let i = Number(ageOfSolarSystem) + 1; i <= 25; i++) {
							let billSavingsValue = getBilSavings.get(i) + finalOM;
							cfMap.set(i, Math.round(billSavingsValue));
							cfArray.push(billSavingsValue);
						}
						let sum = cfArray.reduce((a, b) => a + b, 0);
						// let payBackSavings = (sum / cfArray.length) * 20;
						let payBackSavings = sum;
						// console.log(Math.round(payBackSavings));
						return Math.round(payBackSavings);*/
							
}

// calculating Bill saving for home value
function getBillSavings($installedCapacity, $zipcode, $bill)
{

		$url1="http://34.197.190.88:8888/solarOnlySavings?addr=${zipcode}&bill=${bill}&pv=${installedCapacity}";
   		 $res = file_get_contents($url1);
			
			// we have to fetch the annual saving elctrics array from above apis and store into map with key and value
    	$annualSavingsElectrics = json_decode($res, TRUE);
	
			/* this is taken from node js as same we have to wrte in php  and it will return the array into getOperatingCf function
			savingsMap = new Map();
		for (let i = 0; i <= 24; i++) {
		let billSavings = response.data.annualSavingsElectric[i];
		savingsMap.set(i + 1, Math.round(billSavings));
		}*/

		return savingsMap;
}


//----------------------------------end----------------------------------------

?>