<?php 
function saving_resource() {
	$api = array(
		'getEvSavings' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get a article based on node id',
			  'callback' => 'get_saving',
			  'file' => array('type' => 'inc', 'module' => 'saving', 'name' => 'includes/saving.services'),
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


function get_saving(){
$params = drupal_get_query_parameters();
    $lat =$params['lat'];
    $lon =$params['lon'];
    $avgMilesTravelledInMonth=$params['avgMilesTravelledInMonth'];
    $batterySize =$params['batterySize'];
    $range=$params['range'];
   if(sizeof($params)==5)
   {
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
    $pvSysSize = round(getPVSize($solrad_annual,$dailyYield));
    $panelCount = getPanelsCount($pvSysSize);
    $annualUtilityBillSavings = $dailyYield * 365 * number_format($residential, 3);

    $myArr = array(avgMilesTravelledInMonth=>$avgMilesTravelledInMonth,
    batterySize=> $batterySize,range=> $range,pvSize=>$pvSysSize,panelCount=>round($panelCount),annualUtilityBillSavings=>round($annualUtilityBillSavings));
    //return json_encode($myArr,JSON_UNESCAPED_SLASHES);
   return $myArr;
   }
   else{
     return array(message=>'query parameter missing');
   }
   
   //name=>sizeof($params),lat=>$lat,long=>$lon,residential=>$residential,solrad_annual=>$solrad_annual,dailyYield=>$dailyYield,
}



?>