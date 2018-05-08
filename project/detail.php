<?php
/**
 * Created by PhpStorm.
 * User: user
 * Date: 18-5-8
 * Time: 下午3:59
 */
include "./lib/gate.php";
include "./lib/huobi.php";

$gate  = new Gate();
$huobi = new Huobi();

$name  = isset($_GET['name'])?$_GET['name']:'BTC';
$title = $name;
$name  = strtolower($name);

// 火币数据                  get_market_detail
$usdt_huobi_datas = $huobi->get_market_trade($name . 'usdt');
$btc_huobi_datas  = $huobi->get_market_trade($name . 'btc');
$eth_huobi_datas  = $huobi->get_market_trade($name . 'eth');

// GateIO数据
$usdt_gateio_datas = $gate->get_ticker($name . '_usdt');
$btc_gateio_datas  = $gate->get_ticker($name . '_btc');
$eth_gateio_datas  = $gate->get_ticker($name . '_eth');
?>

<!DOCTYPE html>
<!-- saved from url=(0030)https://confidentcustomer.com/ -->
<html class="no-js" lang="en">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">

    <meta name="robots" content="index, follow">
    <!-- title -->
    <title> <?= $title ?> 当下行情 </title>

    <meta property="og:description" content="">

    <meta property="og:type" content="website">

    <link rel="stylesheet" href="test/font-awesome-4.7.0/css/font-awesome.css">

    <link rel="stylesheet" href="test/font-awesome.min.css">
    <link rel="stylesheet" href="test/style_new_common.css">
    <link rel="stylesheet" href="test/style_cc_new.css">
    <link rel="stylesheet" href="test/app.css">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">

    <script src="http://cdn.static.runoob.com/libs/jquery/2.1.1/jquery.min.js"></script>
    <script src="http://cdn.static.runoob.com/libs/bootstrap/3.3.7/js/bootstrap.min.js"></script>
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
                <h2 class="section-heading">多交易平台虚拟货币交易价<br class="hidden-xs"> 和各虚拟货币在各交易平台的 <span class="text-green"> 价格对比 </span></h2>
                <div class="row">
                    <ul id="myTab" class="nav nav-tabs">
                        <li class="active"><a href="#USDT" data-toggle="tab">USDT</a></li>
                        <li><a href="#BTC" data-toggle="tab">BTC</a></li>
                        <li><a href="#ETH" data-toggle="tab">ETH</a></li>
                    </ul>
                    <div id="myTabContent" class="tab-content">
                        <div class="tab-pane fade in active" id="USDT">

                            <table  class="table">
                                <thead>
                                <tr>平台</tr>
                                <tr>价格</tr>
                                <tr>涨幅</tr>
                                <tr>差价</tr>
                                </thead>
                                <tr>
                                    <td>火币交易平台</td>
                                    <td>$ <?= $usdt_huobi_datas->status == "ok" ? $usdt_huobi_datas->tick->data[0]->price : "暂不支持" ?></td>
                                    <td>$ <?= $usdt_huobi_datas->status == "ok" ? $usdt_huobi_datas->tick->data[0]->price : "暂不支持" ?></td>
                                    <td rowspan="2">$ <?= $usdt_huobi_datas->status == "ok"?abs($usdt_huobi_datas->tick->data[0]->price - $usdt_gateio_datas['last']):'' ?></td>
                                </tr>
                                <tr>
                                    <td>GateIo交易平台最新值【<?= $title ?> / USDT 】 </td>
                                    <td>$ <?= $usdt_gateio_datas['result'] == 'true' ? $usdt_gateio_datas['last']:'暂不支持' ?></td>
                                </tr>
                            </table>

                            <pre>
                                <?= var_dump($usdt_huobi_datas)?>
                            </pre>
                            <pre>
                                <?= var_dump($usdt_gateio_datas)?>
                            </pre>

                        </div>

                        <div class="tab-pane fade" id="BTC">

                            <table  class="table">
                                <tr>
                                    <td>火币交易平台最新值【<?= $title ?> / BTC 】 </td>
                                    <td><?= $btc_huobi_datas->status == "ok" ? $btc_huobi_datas->tick->data[0]->price : "暂不支持" ?></td>
                                    <td rowspan="2"><?= $btc_huobi_datas->status == "ok" ? abs($btc_huobi_datas->tick->data[0]->price - $btc_gateio_datas['last']) :'暂不支持' ?></td>
                                </tr>
                                <tr>
                                    <td>GateIo交易平台最新值【<?= $title ?> / BTC 】 </td>
                                    <td><?= $btc_gateio_datas['result'] == 'true' ? $btc_gateio_datas['last']:'暂不支持' ?></td>
                                </tr>
                            </table>

                            <pre>
                                <?= var_dump($btc_huobi_datas)?>
                            </pre>
                            <pre>
                                <?= var_dump($btc_gateio_datas)?>
                            </pre>
                        </div>

                        <div class="tab-pane fade" id="ETH">

                            <table  class="table">
                                <tr>
                                    <td>火币交易平台最新值【<?= $title ?> / ETH 】 </td>
                                    <td><?= $eth_huobi_datas->status == "ok" ? $eth_huobi_datas->tick->data[0]->price : "暂不支持" ?></td>
                                    <td rowspan="2"><?= $eth_huobi_datas->status == "ok" ? abs($eth_huobi_datas->tick->data[0]->price - $eth_gateio_datas['last']) : "暂不支持" ?></td>
                                </tr>
                                <tr>
                                    <td> GateIo交易平台最新值【<?= $title ?> / ETH 】 4 </td>
                                    <td> <?= $eth_gateio_datas['result'] == 'true' ? $eth_gateio_datas['last']:'暂不支持' ?> </td>
                                </tr>
                            </table>

                            <pre>
                                <?= var_dump($eth_huobi_datas)?>
                            </pre>
                            <pre>
                                <?= var_dump($eth_gateio_datas)?>
                            </pre>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</section>
<!-- End Start Trial -->

</body>
</html>

