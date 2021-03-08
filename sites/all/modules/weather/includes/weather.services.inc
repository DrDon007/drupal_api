<?php

function weather_resource() 
{
	$api = array('getSevereWeatherResults-api' => array(
		        'operations' => array(
			    'index' => array(
			    'help' => 'get severe weather results',
			    'callback' => 'get_SevereWeatherResults',
			    'file' => array('type' => 'inc', 'module' => 'weather', 'name' => 'includes/weather.services'),
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

/*----------------------------Power-Disruption api function---------------------------*/
function powerDisruptionData($lat,$long)
{
    $url="https://api.weather.com/v2/indices/powerDisruption/daypart/15day?geocode={$lat},{$long}&language=en-US&format=json&apiKey=84f7d46091b2492ab7d46091b2f92a29";
    $pdata = file_get_contents($url);
    $powerDisruptionData = json_decode($pdata, TRUE);
    return $powerDisruptionData;
}

/*----------------------------psps-Event api function---------------------------*/
function PspsEventsFromUrl($countryCode)
{
    $countryCode="US";
    $url="https://preprod.poweroutage.us/api/geojson_1_4/Enphase98!u28!@0gujf?RequestType=Planned&CountryId=US";
    $pspsEventsData = file_get_contents($url);
    // $pspsEventsData=drupal_http_request($url);
    $pspsEventData = json_decode($pspsEventsData, TRUE);
    return $pspsEventData;
}

/*----------------------------weather-Event api function---------------------------*/
function weatherEventApiData($state,$country)
{
    //$weatherEventsData = db_query("select f.statename,f.alert,f.county from (SELECT distinct s.statename as statename ,a.state as statecode ,a.event as alert,b.county as county,dense_rank() over(partition by b.county order by c.id asc) as rankk from alerts a INNER JOIN affectedzones b INNER JOIN warningpriority c on c.warning= a.event INNER JOIN statecodes s on s.statecode= a.state and a.state=b.state ) f where f.rankk=1 and f.statename='$state' and f.county='$country'")->fetchAll();
    $url="http://3.7.164.225:3000/api/v1/getWeatherEvents?stateName={$state}&countyName={$country}";
    $weatherEventsData = file_get_contents($url);
    $weatherEventData = json_decode($weatherEventsData, TRUE);
    //return array(state=>$state,country=>$country);
    return $weatherEventData;
}

/*----------------------------Resiliency-Factor api function---------------------------*/
function ResiliencyFactor($stateName,$countryName)
{
	$state = $stateName;
	$country = $countryName;
	$url="http://3.7.164.225:3000/api/v1/getResiliencyFactor?state={$state}&county={$country}";
    $ResiliencyFactor = file_get_contents($url);
    $ResiliencyFactor = json_decode($ResiliencyFactor, TRUE);
    return $ResiliencyFactor['resiliencyFactorScore'];
}

/*----------------------------Storm-Warning api function---------------------------*/
function get_stormwarnings()
{
    $params = drupal_get_query_parameters();
    $lat = $params['lat'];
    $long = $params['long'];
    $url = "https://api.weather.gov/points/{$lat},{$long}";
    $stdata = drupal_http_request($url);
    $res=json_decode($stdata->data);
    $pro=$res->properties;
    $gridX=$pro->gridX;
    $gridY=$pro->gridY;
    $office=$pro->gridId;
    $forecast=$pro->forecast;
    /*-----------------------call for second api, function forecast_Api()--------------------------------------*/
    //https://api.weather.gov/gridpoints/PSR/30,61/forecast
    $forecastData=forecast_Api($gridX,$gridY,$office);
    return $forecastData;  
}

function forecast_Api($gridX,$gridY,$office)  
{
    $forcastApi="https://api.weather.gov/gridpoints/{$office}/{$gridX},{$gridY}/forecast";
    $response = drupal_http_request($forcastApi);
    $res = json_decode($response->data);
    $properties=$res->properties;
    $periods=$properties->periods;
    return $periods;
}

/*----------------------------Defining MessageMatrix for Resiliency Factor---------------------------*/
function getMessageMatrix($resiliencyFactor)
{
    if($resiliencyFactor <= 30) 
    {
        $response = array(
        "resiliencyFactor"=> $resiliencyFactor,
        "message"=>"A solar and storage solution would help you avoid peak demand charges and save on utility bills",
        "outageRisk"=>"Low"
        );
        return $response;
    }
    if($resiliencyFactor >= 31 && $resiliencyFactor <= 60) 
    {
        $response = array(
        "resiliencyFactor"=> $resiliencyFactor,
        "message"=>"A solar and storage solution would help you stay powered during unexpected events.",
        "outageRisk"=> "Moderate"
        );
        return $response;
    }
    if($resiliencyFactor >= 61 && $resiliencyFactor <= 100) 
    {
        $response = array(
        "resiliencyFactor"=> $resiliencyFactor,
        "message"=>"A solar and storage solution would help you protect yourself from blackouts.",
        "outageRisk"=> "High"
        );
        return $response;
    }
}

/*----------------------------Main Api function (Get severe weather and forecast Api's)---------------------------*/
function get_SevereWeatherResults()
{   
    /*-----------------------------url's for testing-------------------------------
    http://localhost/drupal_api/test/getSevereWeatherResults-api?lat=32.54&long=-86.64&stateName=Maine&countryName=Androscoggin
    http://localhost/drupal_api/test/getSevereWeatherResults-api?lat=33.13&long=-115.52&stateName=Maine&countryName=Androscoggin
    */

	$params = drupal_get_query_parameters();
	$state= $params['stateName'];
    $country= $params['countryName'];
    $lattitude= $params['lat'];
    $longitude= $params['long'];
    
    if(!empty($state) && !empty($country) && !empty($lattitude) && !empty($longitude))
    {
        
        /*----------------------------Power-Disruption api function---------------------------*/
        $powerDisruptionData=powerDisruptionData($lattitude,$longitude);   
        //return $powerDisruptionData;
                

        /*----------------------------psps-Event api call---------------------------*/
        $pspsEventData=PspsEventsFromUrl($country);       
        //return $pspsEventData;


        /*----------------------------weather-Event api call---------------------------*/
        $wEventData=weatherEventApiData($state,$country);                        
        //return $wEventData;

        
        /*----------------------------Resiliency-Factor api call---------------------------*/
        $resiliencyFactorScore=ResiliencyFactor($state,$country);
        $resiliencyMessages=getMessageMatrix($resiliencyFactorScore);
        //return $resiliencyMessages;


        /*----------------------------Storm-Warning api call---------------------------*/
        $StormWarningsData=get_stormwarnings();
        //return $StormWarningsData;

        /*------------------------------Forming Output From Api Response Data--------------------------*/
        $response=array("powerDisruptionData"=>$powerDisruptionData ,"pspsEvents"=>$pspsEventData ,"weatherEvents"=>$wEventData ,"resiliencyMessage" => $resiliencyMessages , "StormWarnings"=>$StormWarningsData);
        return $response;
    }
    else
    {
        return $response=array(warning=>"please provide required  parameters",error=>"404 not found");
    }
}


?>