<?php
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");
$response = array();
function arrayToXml($array, $rootElement = null, $xml = null) { 
    $_xml = $xml; 
    if ($_xml === null) { 
        $_xml = new SimpleXMLElement($rootElement !== null ? $rootElement : '<weather/>'); 
    }
    foreach ($array as $k => $v) {
        if (is_array($v)) {  
            arrayToXml($v, $k, $_xml->addChild($k)); 
        } 
        else { 
            $_xml->addChild($k, $v); 
        } 
    }
    return $_xml->asXML(); 
}
if (isset($_GET['temp']) && isset($_GET['hum']) && isset($_GET['pres']) && isset($_GET['aqi']) && isset($_GET['mac']) && isset($_GET['lat']) && isset($_GET['lon'])) 
{
    $new_temp = $_GET['temp'];
    $new_hum = $_GET['hum'];
    $new_pres = $_GET['pres'];
    $new_aqi = $_GET['aqi'];
    $mac = $_GET['mac'];
    $lat = $_GET['lat'];
    $lon = $_GET['lon'];
    if (isset($_GET['format'])) $format = strtolower($_GET['format']);
    $filepath = realpath (dirname(__FILE__));
    require_once($filepath."/db_connect.php");
    $result = array();
    $result1 = array();
    $temp = mysqli_query($conn,"SELECT mac FROM idtomacmap");
    if (mysqli_num_rows($temp))
    {
        while($row = mysqli_fetch_assoc($temp))
        {
            $result[] = $row;
        }
    }
    foreach ($result as $data )
    {
        array_push($result1, $data['mac']);
    }
    $table_name = 'device_'.strtolower($mac);
    if (in_array($mac,$result1))
    {
        $temp_lat = mysqli_query($conn,"SELECT lat FROM idtomacmap WHERE mac = '$mac'");
        $row = mysqli_fetch_array($temp_lat);
        $temp_lat = $row[0];

        $temp_lon = mysqli_query($conn,"SELECT lon FROM idtomacmap WHERE mac = '$mac'");
        $row = mysqli_fetch_array($temp_lon);
        $temp_lon = $row[0];

        if($temp_lat == $lat && $temp_lon == $lon)
        {

            $res4 = mysqli_query($conn,"UPDATE `lastrecord` SET `temp` = '$new_temp', `hum` = '$new_hum', `pres` = '$new_pres' , `aqi` = '$new_aqi' WHERE `lastrecord`.`mac` = '$mac'");
            $res5 = mysqli_query($conn,"INSERT INTO $table_name(temp,hum,pres,aqi) VALUES('$new_temp','$new_hum','$new_pres','$new_aqi'); ");
                if($res4 == 1 && $res5 == 1)
                $result = 1; // success
                else
                $result = 0; // error
        }
        else
        {

            $res4 = mysqli_query($conn,"UPDATE `lastrecord` SET `temp` = '$new_temp', `hum` = '$new_hum', `pres` = '$new_pres' , `aqi` = '$new_aqi' WHERE `lastrecord`.`mac` = '$mac'");
            $res5 = mysqli_query($conn,"INSERT INTO $table_name(temp,hum,pres,aqi) VALUES('$new_temp','$new_hum','$new_pres','$new_aqi'); ");
            $res6 = mysqli_query($conn,"UPDATE `idtomacmap` SET `lat` = '$lat', `lon` = '$lon' WHERE `idtomacmap`.`mac` = '$mac'");
            if($res4 == 1 && $res5 == 1 && $res6 == 1)
                $result = 1; // success
            else
                $result = 0; // error
        }
    }
    else
    {
        $temp_lat = mysqli_query($conn,"SELECT lat FROM idtomacmap WHERE mac = '$mac'");

        $res1 = mysqli_query($conn,"INSERT INTO idtomacmap(mac, lat, lon )VALUES ('$mac','$lat','$lon');");
        $res2 = mysqli_query($conn,"INSERT INTO lastrecord(mac,temp, hum, pres, aqi )VALUES ('$mac','$new_temp','$new_hum','$new_pres','$new_aqi');");
        $res3 = mysqli_query($conn,"CREATE TABLE $table_name(`id` INT(30) NOT NULL AUTO_INCREMENT ,`temp` VARCHAR(30) NOT NULL , `hum` VARCHAR(30) NOT NULL ,`pres` VARCHAR(30) NOT NULL, `aqi` VARCHAR(30) NOT NULL , PRIMARY KEY (`ID`)) ENGINE = InnoDB;");
        $res7 =mysqli_query($conn,"INSERT INTO $table_name(temp,hum,pres,aqi) VALUES ('$new_temp','$new_hum','$new_pres','$new_aqi');");
        if($res1 == 1 && $res2 == 1 && $res3 == 1 && $res7 == 1)
            $result = 1; // success
        else
            $result = 0; // error
    }
    if ($result) 
    {
        $response["success"] = 1;
        $response["message"] = "Weather successfully created.";
        if (isset($_GET['format'])){
            if($format == "xml") echo arrayToXml($response);
            else if($format == "json") echo json_encode($response);
            else echo "Error: Invalid format";
        }
        else echo json_encode($response);
    } 
    else 
    {
        $response["success"] = 0;
        $response["message"] = "Something has been wrong";
        if (isset($_GET['format'])){
            if($format == "xml") echo arrayToXml($response);
            else if($format == "json") echo json_encode($response);
            else echo "Error: Invalid format";
        }
        else echo json_encode($response);
    }
} 
else 
{
    $response["success"] = 0;
    $response["message"] = "Parameter(s) are missing. Please check the request";
    if (isset($_GET['format'])){
        if($format == "xml") echo arrayToXml($response);
        else if($format == "json") echo json_encode($response);
        else echo "Error: Invalid format";
    }
    else echo json_encode($response);
}
?>