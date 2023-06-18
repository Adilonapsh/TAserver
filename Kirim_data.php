<?php

header('Content-Type:Application/json');
header('Response:250');

$json = file_get_contents('Fils.json');
$data= json_decode($json,true);

echo($json);

?>