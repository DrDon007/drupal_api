<?php

//error_reporting(0);

function evsavingcalculator_resource() 
{
	$api = array(
		'getEvSavings' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get ev savings',
			  'callback' => 'get_evsavingcalculator',
			  'file' => array('type' => 'inc', 'module' => 'evsavingcalculator', 'name' => 'evsavingcalculator.services'),
			  'access callback' => 'evsavingcalculator_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}

function evsavingcalculator_custom_resource_access()
{
	return TRUE;
}

//-------------------------------------------------

function getDailyYield($miles, $batterySize, $range) 
{
  $dailyYield = ($miles * $batterySize * 12) / ($range * 365);
  return $dailyYield;
}

function getPVSize($solrad_annual, $pvdata) 
{
  $pvSize = $pvdata / $solrad_annual;
  return $pvSize;
}

function getPanelsCount($pvdata) 
{
  $peakACpowerOutputMicroInverter=295;
  $count = (($pvdata * 1000) / $peakACpowerOutputMicroInverter) * 1.17;
  return $count;
}
//-------------------------------------------------

function drupalmy_json_output($var) 
{
  // We are returning JSON, so tell the browser.
  drupal_add_http_header('Content-Type', 'application/json');
  if (isset($var)) {
    echo drupal_json_encode($var);
  }
}

function get_evsavingcalculatorS()
{
	$params = drupal_get_query_parameters();
	
    $avgMilesTravelledInMonth=270;
    $batterySize=50;
    $range=100;
    
    //long lat values
    //SELECT * FROM `app_longlat_values` WHERE `id`=1

    $lat=37.2844269;
    $long=-122.0148829;
    
    //utility_rates from pv watts
    //$pvwatts="https://developer.nrel.gov/api/utility_rates/v3.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat={0}&lon={1}";
    //$pvwatts="https://developer.nrel.gov/api/utility_rates/v3.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat=37.2844269&lon=-122.0148829";
	
    $pvdata = file_get_contents("https://developer.nrel.gov/api/utility_rates/v3.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat=32.536382&lon=-86.644490");
    $pvWatts = json_decode($pvdata, TRUE);
    foreach ($pvWatts as $pvWatts) 
    {
     $residential=$pvWatts['residential'];
    }

    //system capacity -->solrad_annualy
    //$pvWattsSunshine="https://developer.nrel.gov/api/pvwatts/v6.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat=37.2844269&lon={1}&system_capacity=1&azimuth=180&tilt=20&array_type=1&module_type=1&losses=14&inv_eff=97";
    $pvWattsSunshineData=file_get_contents("https://developer.nrel.gov/api/pvwatts/v6.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat=32.536382&lon=-86.644490&system_capacity=1&azimuth=180&tilt=20&array_type=1&module_type=1&losses=14&inv_eff=97");
    $pvWattsSunshine=json_decode($pvWattsSunshineData, TRUE);
    foreach ($pvWattsSunshine as $pvWattsSunshine) 
    {
     $solrad_annual=$pvWattsSunshine['solrad_annual'];
    }

    $dailyYield = getDailyYield($avgMilesTravelledInMonth,$batterySize,$range);
    $pvSysSize = getPVSize($solrad_annual,$dailyYield);
    $panelCount = ceil(getPanelsCount($pvSysSize));
    $annualUtilityBillSavings = $dailyYield * 365 * $residential;

    $myArr = array(solrad_annual=>$solrad_annual,dailyYield=>$dailyYield,avgMilesTravelledInMonth=>$avgMilesTravelledInMonth,batterySize=> $batterySize,range=> $range,pvSize=>ceil($pvSysSize),panelCount=>$panelCount,annualUtilityBillSavings=>round($annualUtilityBillSavings));
    return $myArr;
}

function get_evsavingcalculator()
{
	  $params = drupal_get_query_parameters();
    $lat =$params['lat'];
    $lon =$params['lon'];
    $avgMilesTravelledInMonth=$params['avgMilesTravelledInMonth'];
    $batterySize =$params['batterySize'];
    $range=$params['range'];
    //$avgMilesTravelledInMonth=270;
    //$batterySize=50;
    //$range=100;
   
    //utility_rates from pv watts
    //$pvwatts="https://developer.nrel.gov/api/utility_rates/v3.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat={0}&lon={1}";
    $url1="https://developer.nrel.gov/api/utility_rates/v3.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat={$lat}&lon={$lon}";
    $pvdata = file_get_contents($url1);
    $pvWatts = json_decode($pvdata, TRUE);
    $residential=0;
    foreach ($pvWatts as $pvWatts) 
    {
     $residential=$pvWatts['residential'];
    }

    //system capacity -->solrad_annualy
    //$pvWattsSunshine="https://developer.nrel.gov/api/pvwatts/v6.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat=37.2844269&lon={1}&system_capacity=1&azimuth=180&tilt=20&array_type=1&module_type=1&losses=14&inv_eff=97";
    $url2="https://developer.nrel.gov/api/pvwatts/v6.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat={$lat}&lon={$lon}&system_capacity=1&azimuth=180&tilt=20&array_type=1&module_type=1&losses=14&inv_eff=97";
    $pvWattsSunshineData=file_get_contents($url2);
    $pvWattsSunshine=json_decode($pvWattsSunshineData, TRUE);
    foreach ($pvWattsSunshine as $pvWattsSunshine) 
    {
     $solrad_annual=$pvWattsSunshine['solrad_annual'];
    }

    $dailyYield = getDailyYield($avgMilesTravelledInMonth,$batterySize,$range);
    $pvSysSize = getPVSize($solrad_annual,$dailyYield);
    $panelCount = ceil(getPanelsCount($pvSysSize));
    $annualUtilityBillSavings = $dailyYield * 365 * $residential;

    $myArr = array(lat=>$lat,long=>$lon,residential=>$residential,solrad_annual=>$solrad_annual,dailyYield=>$dailyYield,avgMilesTravelledInMonth=>$avgMilesTravelledInMonth,batterySize=> $batterySize,range=> $range,pvSize=>ceil($pvSysSize),panelCount=>$panelCount,annualUtilityBillSavings=>round($annualUtilityBillSavings));
    //return json_encode($myArr,JSON_UNESCAPED_SLASHES);
   return $myArr;

}

function get_evsavingcalculatorssss()
{
	  $params = drupal_get_query_parameters();
    $lat =$params['lat'];
    $lon =$params['lon'];
    $avgMilesTravelledInMonth=$params['avgMilesTravelledInMonth'];
    $batterySize =$params['batterySize'];
    $range=$params['range'];
    //$avgMilesTravelledInMonth=270;
    //$batterySize=50;
    //$range=100;
   
    //utility_rates from pv watts
    //$pvwatts="https://developer.nrel.gov/api/utility_rates/v3.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat={0}&lon={1}";
    $url1="https://developer.nrel.gov/api/utility_rates/v3.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat={$lat}&lon={$lon}";
    $pvdata = file_get_contents($url1);
    $pvWatts = json_decode($pvdata, TRUE);
    $residential=0;
    foreach ($pvWatts as $pvWatts) 
    {
     $residential=$pvWatts['residential'];
    }

    //system capacity -->solrad_annualy
    //$pvWattsSunshine="https://developer.nrel.gov/api/pvwatts/v6.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat=37.2844269&lon={1}&system_capacity=1&azimuth=180&tilt=20&array_type=1&module_type=1&losses=14&inv_eff=97";
    $url2="https://developer.nrel.gov/api/pvwatts/v6.json?api_key=lwzpUkBIWvUVpsBKVSMctWpf5ejDxmLLD93rbrce&lat={$lat}&lon={$lon}&system_capacity=1&azimuth=180&tilt=20&array_type=1&module_type=1&losses=14&inv_eff=97";
    $pvWattsSunshineData=file_get_contents($url2);
    $pvWattsSunshine=json_decode($pvWattsSunshineData, TRUE);
    foreach ($pvWattsSunshine as $pvWattsSunshine) 
    {
     $solrad_annual=$pvWattsSunshine['solrad_annual'];
    }

    $dailyYield = getDailyYield($avgMilesTravelledInMonth,$batterySize,$range);
    $pvSysSize = getPVSize($solrad_annual,$dailyYield);
    $panelCount = ceil(getPanelsCount($pvSysSize));
    $annualUtilityBillSavings = $dailyYield * 365 * $residential;

    $myArr = array(lat=>$lat,long=>$lon,residential=>$residential,solrad_annual=>$solrad_annual,dailyYield=>$dailyYield,avgMilesTravelledInMonth=>$avgMilesTravelledInMonth,batterySize=> $batterySize,range=> $range,pvSize=>ceil($pvSysSize),panelCount=>$panelCount,annualUtilityBillSavings=>round($annualUtilityBillSavings));
    //return json_encode($myArr,JSON_UNESCAPED_SLASHES);
   return $myArr;

}


?>
