<?php

// header('Content-Type:Application/json');
function Get_Data($url,$timeout=3){
    $ch= curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch,CURLOPT_RETURNTRANSFER,1);
    curl_setopt($ch,CURLOPT_TIMEOUT,2);
    $output=curl_exec($ch);
    if (curl_errno($ch)){
        // throw new Exception (curl_errno($ch));
        // echo "<br>";
        // echo "I'm Sorry !!";
    }
    curl_close($ch);
    return $output;
}
$root="http://192.168.0.10";
$Ultra=$_POST["sensorultra"];
$Pir=$_POST['sensorpir'];
$Relay1=$_POST['Relay1'];
$Relay2=$_POST['Relay2'];
$Relay3=$_POST['Relay3'];


$ips=["2","0"];
$HPD=[];
for($i=0; $i < count($ips); $i++){
    $urls=$root.$ips[$i].":8080/sensors.json";
    $urlst=$root.$ips[$i].":8080/status.json?show_avail=2";
    $dt=Get_Data($urls);
    $dt=json_decode($dt,TRUE);
    $dts=Get_Data($urlst);
    $dts=json_decode($dts,TRUE);
    if (isset($dt)){
        $bl=$dt["battery_level"]["data"][2][1][0];
        $bt=$dt["battery_temp"]["data"][2][1][0];
        $bv=$dt["battery_voltage"]["data"][2][1][0];
        $vc=$dts["video_connections"];
        $vs=$dts["video_status"]["mode"];
        $ip=$dts["curvals"]["ip_address"];
        $fl=$dts["curvals"]["torch"];
        
        $HPD[]=[
            'HP '.$i=>[
                'Phone_Status'=>"on",
                'Ip_Address'=>$ip,
                'Video_Status'=>$vs,
                'Video_Connections'=>$vc,
                'Flash'=>$fl,
                'Battery_Lvl'=>$bl,
                'Battery_Tmp'=>$bt,
                'Battery_Vlt'=>$bv,
            ]
            ];
    }else{
        // echo "Cannot Connect !";
        $HPD[]=[
            'HP '.$i=>[
                'Phone_Status'=>"off",
                'Ip_Address'=>"null",
                'Video_Status'=>0,
                'Video_Connections'=>0,
                'Battery_Lvl'=>0,
                'Battery_Tmp'=>0,
                'Battery_Vlt'=>0,
            ]
        ];
    }
    // echo $urls;
}

$Data = [
    'Nodemcu'=>[
        'Sensorultra' => $Ultra,
        'Sensorpir' => $Pir,
        'Relay1' => $Relay1,
        'Relay2' => $Relay2,
        'Relay3' => $Relay3
    ]
];

// $EZ=array_merge($Data,$HPD);
$EZ=$Data+$HPD;
$Datas=json_encode($EZ,JSON_PRETTY_PRINT);

$fp = fopen('Fils.json','w');
fwrite($fp,$Datas);
fclose($fp);
// var_dump($Datas)
?>