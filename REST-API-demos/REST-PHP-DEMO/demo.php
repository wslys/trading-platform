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
define('SECRET_KEY', 'ab7e1b06-897bc505-7a1c0799-52d77'); // 你的SECRET_KEY

include "lib.php";

//实例化类库
$req = new req();

echo " start >>>>>>>>>>>>>>>>>>>>>> ";

var_dump($req->get_detail_merged('btcusdt'));
echo "start >>>>>>>>>>>>>>>>>>>>>>";
