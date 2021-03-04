<?php 
function paybackRooftopHomevalue_resource() 
{
    'paybackRooftopHomevalue-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get a article based on node id',
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
    $params = drupal_get_query_parameters();
    $lat =$params['lat'];
    $lon =$params['lon'];
    $countryName=$params['countryName'];
    $stateCode=$params['stateCode'];
    $zipCode=$params['zipCode'];
    $initialCapacity= 600;
   $minAgeOfSolarSystem= 0;
   $zipCode;
   $monthlyBill= 150;

		//home values Solar calling a funtion 
    homeValueSolar=getOperatingCf($initialCapacity,$minAgeOfSolarSystem,$zipCode,$monthlyBill);

	//this value will come from the api for home value
	homeValueQuantarium = shared.getQuantariumValue(lat, long)
    finalHomeValue= homeValueSolar + homeValueQuantarium.homevaue;

	/* final result 
			 homeValueWithSolar: finalHomeValue;
            estimatedSavings: homeValueSolar;
            homeValueWithOutSolar: Number(homeValueQuantarium.homeValue)
            initialCapacity: initialCapacity;
            ageOfSolarSystem: minAgeOfSolarSystem,
	*/
    }


    function getOperatingCf(initialCapacity,minAgeOfSolarSystem,zipCode,monthlyBill)
    {
      getBilSavings = getBillSavings(installedCapacity, zipcode, bill);

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
    getBillSavings($installedCapacity, $zipcode, $bill)
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
}
?>