<?php 
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

	$rows = $db->query("SELECT id, `name`, tp_id, pair FROM currencys ORDER BY name, tp_id, pair DESC")->fetchAll();
	$data_arr = [];
	foreach ($rows as $item) {
		$data_arr[$item['name']]['name']  = $item['name'];
		
		if (!isset($data_arr[$item['name']]['tp_id']))
			$data_arr[$item['name']]['tp_id'] = [];
		if (!isset($data_arr[$item['name']]['pair']))
			$data_arr[$item['name']]['pair'] = [];
		
		array_push($data_arr[$item['name']]['tp_id'], $item['tp_id']);
		array_push($data_arr[$item['name']]['pair'], $item['pair']);

		$data_arr[$item['name']]['tp_id'] = array_unique($data_arr[$item['name']]['tp_id']);
		$data_arr[$item['name']]['pair']  = array_unique($data_arr[$item['name']]['pair']);

		// $data_arr[$item['name']]['tp_id_counts'] = count($data_arr[$item['name']]['tp_id']) ? $data_arr[$item['name']]['tp_id'] : 0;
	}

	foreach ($data_arr as $key => $value) {
		$data_arr[$key]['tp_id_counts'] = count($data_arr[$key]['tp_id']);
		$data_arr[$key]['pair_counts'] = count($data_arr[$key]['pair']);
	}

	array_multisort(array_column($data_arr,'tp_id_counts', 'pair_counts'), SORT_DESC, $data_arr);
	// var_dump($data_arr);
?>

<!DOCTYPE html>
<!-- saved from url=(0030)https://confidentcustomer.com/ -->
<html class="no-js" lang="en">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">

    <meta name="robots" content="index, follow">
    <!-- title -->
    <title> Demo </title>

    <meta property="og:description" content="">

    <meta property="og:type" content="website">

    <link rel="stylesheet" href="test/font-awesome-4.7.0/css/font-awesome.css">

    <link rel="stylesheet" href="test/font-awesome.min.css">
    <link rel="stylesheet" href="test/style_new_common.css">
    <link rel="stylesheet" href="test/style_cc_new.css">
    <link rel="stylesheet" href="test/app.css">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
</head>
<body>
<!-- Start header -->
<header>
    <nav class="navbar navbar-default">
        <div class="container">

            <div class="navbar-header">
                <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#bs-example-navbar-collapse-1" aria-expanded="false">
                    <span class="sr-only">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="#"><img src="test/logo.png" class="img-responsive"></a>
            </div>

            <!--<div class="collapse navbar-collapse text-center" id="bs-example-navbar-collapse-1">
                <ul class="nav">
                    <li><a href="#solution" class="page-scroll">首页</a></li>
                    <li><a href="#how_it_works" class="page-scroll">Title</a></li>
                    <li><a href="#features" class="page-scroll">Title</a></li>
                    <li><a href="#testimonials" class="page-scroll">Title</a></li>
                    <li><a href="#pricing" class="page-scroll">Title</a></li>

                    <a href="#register" class="btn btn-sm start-trial">sign up for free</a>
                </ul>
            </div>-->
        </div>
    </nav>
</header>
<!-- End header -->

<!-- Start Trial -->
<section class="trial">
    <div class="container">
        <div class="row margin-b-lg">
            <div class="col-md-12 text-center">
                <h2 class="section-heading">多交易平台虚拟货币交易价<br class="hidden-xs"> 和各虚拟货币 <span class="text-green">在各交易平台的行情</span></h2>
                <div class="row">
                    <table class="table">
                        <thead>
                        <th>币名</th>
                        <th>火币平台</th>
                        <th>GateIO平台</th>
                        <th>USDT</th>
                        <th>BTC</th>
                        <th>ETH</th>
                        </thead>
                        <tbody>
                        <?php foreach ($data_arr as $item) {?>
                        <tr>
                            <td><a href="detail.php?name=<?= $item['name'] ?>"><?= $item['name'] ?></a></td>
                            <td><?= in_array('1', $item['tp_id'])?'<i class="fa fa-check" aria-hidden="true" style="color:green"></i>':'<i class="fa fa-times" aria-hidden="true" style="color:red"></i>' ?></td>
                            <td><?= in_array('2', $item['tp_id'])?'<i class="fa fa-check" aria-hidden="true" style="color:green"></i>':'<i class="fa fa-times" aria-hidden="true" style="color:red"></i>' ?></td>
                            <td><?= in_array('usdt', $item['pair'])?'<i class="fa fa-check" aria-hidden="true" style="color:green"></i>':'<i class="fa fa-times" aria-hidden="true" style="color:red"></i>' ?></td>
                            <td><?= in_array('btc', $item['pair'])?'<i class="fa fa-check" aria-hidden="true" style="color:green"></i>':'<i class="fa fa-times" aria-hidden="true" style="color:red"></i>' ?></td>
                            <td><?= in_array('eth', $item['pair'])?'<i class="fa fa-check" aria-hidden="true" style="color:green"></i>':'<i class="fa fa-times" aria-hidden="true" style="color:red"></i>' ?></td>
                        </tr>
                        <?php } ?>
                        </tbody>
                    </table>
                </div>
            </div>
        </div>
    </div>
</section>
<!--End Start Trial -->

</body>
</html>
