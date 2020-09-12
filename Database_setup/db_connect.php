<?php
$filepath = realpath (dirname(__FILE__));
require_once($filepath."/db_config.php");
$conn = new mysqli(DB_SERVER, DB_USER, DB_PASSWORD, DB_DATABASE);
if($conn->connect_error){
    die("connection failed:".$conn->connect_error);
}
?>