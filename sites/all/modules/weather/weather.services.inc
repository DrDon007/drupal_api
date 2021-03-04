<?php

function weather_resource() 
{
	$api = array('powerDisruption-api' => array(
		        'operations' => array(
			    'index' => array(
			    'help' => 'get severe weather results',
			    'callback' => 'get_powerDistruption',
			    'file' => array('type' => 'inc', 'module' => 'weather', 'name' => 'weather.services'),
			    'access callback' => 'weather_custom_resource_access',
			    'access arguments' => array(),
			    'access arguments append' => TRUE,
			    'args' => array(),),
                ),),
                'getSevereWeatherResults-api' => array(
		        'operations' => array(
			    'index' => array(
			    'help' => 'get severe weather results',
			    'callback' => 'get_SevereWeatherResults',
			    'file' => array('type' => 'inc', 'module' => 'weather', 'name' => 'weather.services'),
			    'access callback' => 'weather_custom_resource_access',
			    'access arguments' => array(),
			    'access arguments append' => TRUE,
			    'args' => array(),),
                ),),
              'getPspsEvents-api' => array(
			  'operations' => array(
			  'index' => array(
              'help' => 'get PspsEvents',
			  'callback' => 'get_PspsEvents',
			  'file' => array('type' => 'inc', 'module' => 'weather', 'name' => 'weather.services'),
			  'access callback' => 'weather_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),),
              ),),
              'weatherEvent-api' => array(
			  'operations' => array(
			  'index' => array(
              'help' => 'get weatherEventApi',
			  'callback' => 'get_weatherEventApi',
			  'file' => array('type' => 'inc', 'module' => 'weather', 'name' => 'weather.services'),
			  'access callback' => 'weather_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),),
              ),),
              'stormwarnings-api' => array(
			  'operations' => array(
			  'index' => array(
              'help' => 'get stormwarnings',
			  'callback' => 'get_stormwarnings',
			  'file' => array('type' => 'inc', 'module' => 'weather', 'name' => 'weather.services'),
			  'access callback' => 'weather_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),),
              ),),
              'getResiliencyFactor' => array(
			  'operations' => array(
			  'index' => array(
              'help' => 'get ResiliencyFactor',
			  'callback' => 'get_ResiliencyFactor',
			  'file' => array('type' => 'inc', 'module' => 'weather', 'name' => 'weather.services'),
			  'access callback' => 'weather_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),),
              ),),
	    );
    return $api;
}

function weather_custom_resource_access()
{
	return TRUE;
}

function get_powerDistruption()
{
	$params = drupal_get_query_parameters();
    $lat = $params['lat'];
    $long = $params['long'];
	//$stateName = $params['stateName'];
	//$countyName = $params['countyName'];
    $url="https://api.weather.com/v2/indices/powerDisruption/daypart/15day?geocode={$lat},{$long}&language=en-US&format=json&apiKey=84f7d46091b2492ab7d46091b2f92a29";
    $wdata = file_get_contents($url);
    $powerDisruptionData = json_decode($wdata, TRUE);
	//$response = drupal_http_request("https://api.weather.com/v2/indices/powerDisruption/daypart/15day?geocode={$lat},{$long}&language=en-US&format=json&apiKey=84f7d46091b2492ab7d46091b2f92a29");
    //return $response->data;
    return $powerDisruptionData;

}

function powerDisruptionData($lat,$long)
{
    $url="http://localhost/drupal_api/test/powerDisruption-api?lat={$lat}&long={$long}";
    $pdata = file_get_contents($url);
    $powerDisruptionData = json_decode($pdata, TRUE);
    return $powerDisruptionData;
}


function get_PspsEvents()
{
    $params = drupal_get_query_parameters();
	$country = $params['countryName'];
    
    $url="https://preprod.poweroutage.us/api/geojson_1_4/Enphase98!u28!@0gujf?RequestType=Planned&CountryId=US";
    $pspsEventsData = file_get_contents($url);
    //$pspsEventsData=drupal_http_request($url);
    $pspsEventData = json_decode($pspsEventsData);
    return $pspsEventData;

    if(!empty($country))
    {
      // $pspsEventsData = db_query("SELECT id,county_name,MAX(est_customers_impacted) as est_customers_impacted,event_type,cities_impacted,estimated_startdate,est_restoration_date,medical_baseline_customers,customers_restored,status,est_restoration_time FROM psps_outage_data where county_name='$country' GROUP BY county_name")->fetchAll();
	   // return $pspsEventsData;
    }
    else
    {
      //  $pspsEventsData = db_query("SELECT id,county_name,MAX(est_customers_impacted) as est_customers_impacted,event_type,cities_impacted,estimated_startdate,est_restoration_date,medical_baseline_customers,customers_restored,status,est_restoration_time FROM psps_outage_data GROUP BY county_name")->fetchAll();
	   // return $pspsEventsData;
    }
    
}

function PspsEventsFromUrl($country)
{
    $countryName=$country;

    $countryCode="US";
    $url="https://preprod.poweroutage.us/api/geojson_1_4/Enphase98!u28!@0gujf?RequestType=Planned&CountryId=US";
    $pspsEventsData = file_get_contents($url);
    // $pspsEventsData=drupal_http_request($url);
    $pspsEventData = json_decode($pspsEventsData, TRUE);
    return $pspsEventData;
}

function PspsEvents($country)
{
    if(!empty($country))
    {
        $pspsEventsData = db_query("SELECT id,county_name,MAX(est_customers_impacted) as est_customers_impacted,event_type,cities_impacted,estimated_startdate,est_restoration_date,medical_baseline_customers,customers_restored,status,est_restoration_time FROM psps_outage_data where county_name='$country' GROUP BY county_name")->fetchAll();
	    return $pspsEventsData;
    }
    else
    {
        $pspsEventsData = db_query("SELECT id,county_name,MAX(est_customers_impacted) as est_customers_impacted,event_type,cities_impacted,estimated_startdate,est_restoration_date,medical_baseline_customers,customers_restored,status,est_restoration_time FROM psps_outage_data GROUP BY county_name")->fetchAll();
	    return $pspsEventsData;
    }
    
}

function get_weatherEventApi()
{
    http://localhost/drupal_api/test/weatherEvent-api?stateName=Maine&countyName=Androscoggin

    $params = drupal_get_query_parameters();
	$state = $params['stateName'];
	$country = $params['countyName'];
	$url="http://3.7.164.225:3000/api/v1/getWeatherEvents?stateName={$state}&countyName={$country}";
    $weatherEventsData = file_get_contents($url);
    $weatherEventsData = json_decode($weatherEventsData, TRUE);
    return $weatherEventsData;
}

function weatherEventApiData($state,$country)
{
    //$weatherEventsData = db_query("select f.statename,f.alert,f.county from (SELECT distinct s.statename as statename ,a.state as statecode ,a.event as alert,b.county as county,dense_rank() over(partition by b.county order by c.id asc) as rankk from alerts a INNER JOIN affectedzones b INNER JOIN warningpriority c on c.warning= a.event INNER JOIN statecodes s on s.statecode= a.state and a.state=b.state ) f where f.rankk=1 and f.statename='$state' and f.county='$country'")->fetchAll();
	$url="http://localhost/drupal_api/test/weatherEvent-api?stateName={$state}&countyName={$country}";
    $weatherEventsData = file_get_contents($url);
    $weatherEventData = json_decode($weatherEventsData, TRUE);
    //return array(state=>$state,country=>$country);
    return $weatherEventData;
}

function get_ResiliencyFactor()
{
    //http://localhost/drupal_api/test/getResiliencyFactor?state=Maine&county=Androscoggin

    //$stateName, $countyName
    $params = drupal_get_query_parameters();
	$state = $params['state'];
	$country = $params['county'];
	$url="http://3.7.164.225:3000/api/v1/getResiliencyFactor?state={$state}&county={$country}";
    $ResiliencyFactor = file_get_contents($url);
    $ResiliencyFactor = json_decode($ResiliencyFactor, TRUE);
    return $ResiliencyFactor['resiliencyFactorScore'];
}

function ResiliencyFactor($stateName,$countryName)
{
    //http://localhost/drupal_api/test/getSevereWeatherResults-api?stateName=Maine&countryName=Androscoggin

	$state = $stateName;
	$country = $countryName;
	$url="http://3.7.164.225:3000/api/v1/getResiliencyFactor?state={$state}&county={$country}";
    $ResiliencyFactor = file_get_contents($url);
    $ResiliencyFactor = json_decode($ResiliencyFactor, TRUE);
    return $ResiliencyFactor['resiliencyFactorScore'];
}

function get_stormwarnings()
{
    $params = drupal_get_query_parameters();
    $lat = $params['lat'];
    $long = $params['long'];
    $gurl = "https://api.weather.gov/points/{$lat},{$long}";
    $stdata = drupal_http_request($gurl);
    //$std = json_encode($stdata, TRUE);
    return $stdata;
    
}
  

function getMessageMatrix($resiliencyFactor)
{
    if($resiliencyFactor <= 30) 
    {
        $response = array("resiliencyMessage"=>array(
        "resiliencyFactor"=> $resiliencyFactor,
        "message"=>"A solar and storage solution would help you avoid peak demand charges and save on utility bills",
        "outageRisk"=>"Low")
        );
        return $response;
    }
    if($resiliencyFactor >= 31 && $resiliencyFactor <= 60) 
    {
        $response = array("resiliencyMessage"=>array(
        "resiliencyFactor"=> $resiliencyFactor,
        message=>"A solar and storage solution would help you stay powered during unexpected events.",
        "outageRisk"=> "Moderate")
        );
        return $response;
    }
    if($resiliencyFactor >= 61 && $resiliencyFactor <= 100) 
    {
        $response = array("resiliencyMessage"=>array(
        "resiliencyFactor"=> $resiliencyFactor,
        message=>"A solar and storage solution would help you protect yourself from blackouts.",
        "outageRisk"=> "High")
        );
        return $response;
    }
}


function get_SevereWeatherResults()
{
    //http://localhost/drupal_api/test/getSevereWeatherResults-api?stateName=Maine&countryName=Androscoggin
    //http://localhost/drupal_api/test/getSevereWeatherResults-api?lat=32.536382&long=-86.644490&stateName=Maine&countryName=Androscoggin

	$params = drupal_get_query_parameters();
	$state= $params['stateName'];
    $country= $params['countryName'];
    $lattitude= $params['lat'];
    $longitude= $params['long'];
    
    if(!empty($state) && !empty($country) && !empty($lattitude) && !empty($longitude))
    {
        $powerDisruptionData=powerDisruptionData($lattitude,$longitude);   //http://localhost/drupal_api/test/getSevereWeatherResults-api?lat=32.54&long=-86.64
        //$pspsEventData=PspsEventsFromUrl($country);         //from new url
        $pspsEventData= PspsEvents($country);                   // http://localhost/drupal_api/test/getSevereWeatherResults-api?countryName=Fresno
        $wEventData=weatherEventApiData($state,$country);  
                                                                    //localhost/drupal_api/test/getSevereWeatherResults-api?lat=32.54&long=-86.64&stateName=California&countryName=Fresno
                                                                    //http://localhost/drupal_api/test/getSevereWeatherResults-api?stateName=Maine&countryName=Androscoggin
            
        $resiliencyFactorScore=ResiliencyFactor($state,$country); //http://localhost/drupal_api/test/getSevereWeatherResults-api?stateName=Maine&countryName=Androscoggin
        //return $resiliencyFactorScore;
            
        $resiliencyMessages=getMessageMatrix($resiliencyFactorScore);
        //return $resiliencyMessages;

        $response=$powerDisruptionData+array("pspsEvents"=>$pspsEventData)+array("weatherEvents"=>$wEventData)+$resiliencyMessages;
        return $response;
    }
    else
    {
        return $response=array(warning=>"please provide required  parameters",error=>"404 not found");
    }
}
?>