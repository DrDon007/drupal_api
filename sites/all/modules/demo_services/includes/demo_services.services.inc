<?php 

function demo_services_resource() {
	$api = array(
		'my-custom-api' => array(
		  'operations' => array(
			'index' => array(
			  'help' => 'get a article based on node id',
			  'callback' => 'get_articles',
			  'file' => array('type' => 'inc', 'module' => 'demo_services', 'name' => 'includes/demo_services.services'),
			  'access callback' => '_custom_resource_access',
			  'access arguments' => array(),
			  'access arguments append' => TRUE,
			  'args' => array(),
			),
		  ),
		),
	);
  return $api;
}
function _custom_resource_access(){
	return TRUE;
}

function get_articles(){
	//$sql = "SELECT * FROM {node} node WHERE node.type = :type AND node.status = :status";
	//$ar_list = db_query($sql, array(':type' => 'calculator', ':status' => 1))->fetchAll();
	
	$ar_list = db_query('SELECT * FROM app_incentives_data_v2')->fetchAll();

	return $ar_list;
}


?>