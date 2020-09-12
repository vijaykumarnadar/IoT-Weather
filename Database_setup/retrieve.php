<?php
header("Access-Control-Allow-Origin: *");
header("Content-Type: application/json; charset=UTF-8");
$response = array();
function getDistance($latitude1, $longitude1, $latitude2, $longitude2)
{
    $earth_radius = 6371;
    $dLat = deg2rad($latitude2 - $latitude1);
    $dLon = deg2rad($longitude2 - $longitude1);
    $a = sin($dLat/2) * sin($dLat/2) + cos(deg2rad($latitude1)) * cos(deg2rad($latitude2)) * sin($dLon/2) * sin($dLon/2);
    $c = 2 * asin(sqrt($a));
    $d = $earth_radius * $c;
    return $d;
}
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
if (isset($_GET['lat']) && isset($_GET['lon']) ) 
{
    $client_lat = $_GET['lat'];
    $client_lon = $_GET['lon'];
    if (isset($_GET['format'])) $format = strtolower($_GET['format']);
    $filepath = realpath (dirname(__FILE__));
    require_once($filepath."/db_connect.php");
    $result = array();
    $mac = array();
    $distance = array();
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
        array_push($mac, $data['mac']);
    }
    foreach ($mac as $data)
    {
        $temp_lat = mysqli_query($conn,"SELECT lat FROM idtomacmap WHERE mac = '$data'");
        $row = mysqli_fetch_array($temp_lat);
        $temp_lat = $row[0];

        $temp_lon = mysqli_query($conn,"SELECT lon FROM idtomacmap WHERE mac = '$data'");
        $row = mysqli_fetch_array($temp_lon);
        $temp_lon = $row[0];

        $dist = getDistance($client_lat,$client_lon,$temp_lat,$temp_lon);
        $distance[$data] = $dist;
    }
    $temp_id = array_keys($distance, min($distance));
    $temp_id = $temp_id[0];
    $result = mysqli_query($conn,"SELECT * FROM lastrecord WHERE mac ='$temp_id'");
    if (!empty($result))
    {
        if (mysqli_num_rows($result) > 0) {
            $result = mysqli_fetch_array($result);
            $weather = array();
            $weather["ID"] = $result["mac"];
            $weather["temp"] = $result["temp"];
            $weather["hum"] = $result["hum"];
            $weather["pres"] = $result["pres"];
            $weather["aqi"] = $result["aqi"];
            $weather["distance"] = "".min($distance)*1000;
            $response["success"] = 1;
            $response["data"] = array();
            array_push($response["data"], $weather);
            if (isset($_GET['format'])){
                if($format == "xml") echo arrayToXml($response);
                else if($format == "json") echo json_encode($response);
                else echo "Error: Invalid format";
            }
            else echo json_encode($response);
        }
        else{
            $response["success"] = 0;
            $response["message"] = "No data on weather found";
            if (isset($_GET['format'])){
                if($format == "xml") echo arrayToXml($response);
                else if($format == "json") echo json_encode($response);
                else echo "Error: Invalid format";
            }
            else echo json_encode($response);
        }
    }
    else {
        $response["success"] = 0;
        $response["message"] = "No data on weather found";
        if (isset($_GET['format'])){
            if($format == "xml") echo arrayToXml($response);
            else if($format == "json") echo json_encode($response);
            else echo "Error: Invalid format";
        }
        else echo json_encode($response);
    }
}
?>
