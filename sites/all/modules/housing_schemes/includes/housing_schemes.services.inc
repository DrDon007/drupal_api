<?php

function housing_schemes_services_resource() {
  $api = array(
    'housing_schemes' => array(
      'operations' => array(
        'retrieve' => array(
          'help' => 'Retrieves housing schemes info.',
          'file' => array('type' => 'inc','module' => 'housing_schemes_services','name' => 'housing_schemes.services',),
          'callback' => 'housing_schemes_services_resource_retrieve',
          'access callback' => 'user_access',
          'access arguments' => array('access content'),
          'access arguments append' => FALSE,
          'args' => array(
            array(
              'name' => 'nid',
              'type' => 'int',
              'description' => 'Function to perform',
              'source' => array(
                'path' => '0'
              ),
              'optional' => TRUE,
              'default' => '0',
            ),
          ),
        ),
      ),
    ),
  );
  return $api;
}