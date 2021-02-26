<?php

function forcast_resource() {
	$api = array(
		'getSevereWeatherResults' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get forcasted data',
			  'callback' => 'get_forcast',
			  'file' => array('type' => 'inc', 'module' => 'forcast', 'name' => 'forcast.services'),
			  'access callback' => 'forcast_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}

function forcast_custom_resource_access()
{
	return TRUE;
}

function get_forcast()
{
	$params = drupal_get_query_parameters();
	$state_name = $params['stateName'];
    $country_name= $params['countryName'];
    $lattitude= $params['lat'];
    $longitude= $params['long'];

	if (!empty($lattitude) && !empty($longitude) && !empty($state_name) && !empty($county_name)) 
    {
		$result = db_query("SELECT * FROM `app_longlat_values` where lattitude='$lattitude' and longitude='$longitude' and state_name='$state_name' and  county_name='$country_name'")->fetchAll();
		return $result;
	}
	else
    {
		$result = db_query("SELECT * FROM `app_longlat_values`")->fetchAll();
		return $result;
	}

}

?>
