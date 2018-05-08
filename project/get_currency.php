<?php 
/**
 * Created by PhpStorm.
 * User: user
 * Date: 18-5-5
 * Time: 上午10:22
 *
 * 定义参数
 */

define('ACCOUNT_ID', '3473787'); // 你的账户ID
define('ACCESS_KEY', '78694c8e-49f6f767-9cdc8b41-bc8b9');  // 你的ACCESS_KEY
define('SECRET_KEY', 'ab7e1b06-897bc505-7a1c0799-52d77');  // 你的SECRET_KEY

include "./lib/gate.php";
include "./lib/huobi.php";
include "./lib/medoo.php";

// 初始化
$db = new Medoo([
    'database_type' => 'mysql',
    'database_name' => 'trading_platform',
    'server' => 'localhost',
    'username' => 'root',
    'password' => '123456',
    'charset' => 'utf8',
    //可选：端口
    'port' => 3306,
    //可选：表前缀
    'prefix' => '',
    // PDO驱动选项 http://www.php.net/manual/en/pdo.setattribute.php
    'option' => [
        PDO::ATTR_CASE => PDO::CASE_NATURAL
    ]
]);

//实例化类库 
$gate = new Gate();
$huobi = new Huobi();

$resules = $gate->get_marketlist();

$sql = "INSERT INTO currencys(tp_id, name, pair) VALUES ";
foreach ($resules['data'] as $item) {
	$sql .= "('2', '" . strtoupper($item['symbol']) . "', '" . explode("_",$item['pair'])[1] . "'),";
}
var_dump(substr($sql, 0, -1) );

$datas = $huobi->get_common_symbols();
$sql = "INSERT INTO currencys(tp_id, name, pair) VALUES ";
foreach ($datas->data as $item) {
    $sql .= "('1', '" . strtoupper($item->{'base-currency'}) . "', '" . $item->{'quote-currency'} . "'),";
}
var_dump(substr($sql, 0, -1));

