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
    $params = drupal_get_query_parameters();
    $lat =$params['lat'];
    $lon =$params['lon'];
    $countryName=$params['countryName'];
    $stateCode=$params['stateCode'];
    $zipCode=$params['zipCode'];
    
}

?>