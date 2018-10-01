<?php require_once "config.php" ?>
<?php require_once "database.php" ?>
<?php require_once "db-helper.php" ?>
<?php 
$database = new Database($DB_HOST, $DB_USER, $DB_PASS);
$database->connect_db();
$database->choose_database_name($DB_NAME);

//Get points
$date = getDateArray();
$monthNameArray = getMonthNameArray();
$yearNameArray = getYearNameArray();

for ($i=0; $i<12; $i++) {
  $dateStart[$i] = $date[$i] . '-01 00:00:00';
  $dateEnd[$i] = $date[$i] . '-31 23:59:59';
  
  $jsDateEnd[$i] = $date[$i] . '-01';
  
  $sql = "SELECT status, cash_in, cash_out, profit, cara_bayar FROM transaction WHERE timestamp >= '%s' AND timestamp <= '%s'";
  $query = sprintf($sql, $dateStart[$i], $dateEnd[$i]);
  $result = $database->query($query);

  $totalCashIn[$i] = 0;
  $totalCashOut[$i] = 0;
  $totalProfit[$i] = 0;

  $totalBuyCash[$i] = 0;
  $totalBuyCredit[$i] = 0;
  $totalSellCash[$i] = 0;
  $totalSellCredit[$i] = 0;
  foreach($result as $object) {
    if ($object->status == 'valid') {
      $totalCashIn[$i] = $totalCashIn[$i] + $object->cash_in;
      if ($object->cash_out != '-') {
        $totalCashOut[$i] = $totalCashOut[$i] + $object->cash_out;
        // determine cash and credit
        if ($object->cara_bayar == 'cash') {
          $totalBuyCash[$i] += $object->cash_out;
        } else {
          $totalBuyCredit[$i] += $object->cash_out;
        }
      }
      if ($object->cash_in != '-') {
        $totalProfit[$i] = $totalProfit[$i] + $object->profit;
        // determine cash and credit
        if ($object->cara_bayar == 'credit') {
          $totalSellCash[$i] += $object->cash_in;
        } else {
          $totalSellCredit[$i] += $object->cash_in;
        }
      }
    }
  }  
}

$database->close_db();

/**
  Return 2014-2 -- 2013-3
*/
function getDateArray() {
  for ($i=0; $i<12; $i++) {
    $mMonthNum = date('m');
    $mYearNum = date('Y');
    
    if (($mMonthNum - $i) <= 0) {
      $mYearNum = $mYearNum - 1;
    }

    $mMonthNum = ($mMonthNum + 12 - $i) % 12;
    if ($mMonthNum == 0) $mMonthNum = 12;
    //$mMonth[$i] = date('F');
    
    $mDate = $mYearNum . "-" . $mMonthNum;
    $date[$i] = $mDate;    
  }
  return $date;
}

/**
  Return January February
*/
function getMonthNameArray() {
  for ($i=0; $i<12; $i++) {
    $mMonthNum = date('m');
    
    $mMonthNum = ($mMonthNum + 12 - $i) % 12;
    if ($mMonthNum == 0) $mMonthNum = 12;
    
    switch ($mMonthNum){
    case 1: $mMonthName[$i] = "January"; break;
    case 2: $mMonthName[$i] = "February"; break;
    case 3: $mMonthName[$i] = "March"; break;
    case 4: $mMonthName[$i] = "April"; break;
    case 5: $mMonthName[$i] = "May"; break;
    case 6: $mMonthName[$i] = "June"; break;
    case 7: $mMonthName[$i] = "July"; break;
    case 8: $mMonthName[$i] = "August"; break;
    case 9: $mMonthName[$i] = "September"; break;
    case 10: $mMonthName[$i] = "October"; break;
    case 11: $mMonthName[$i] = "November"; break;
    case 12: $mMonthName[$i] = "December"; break;      
    }
  }
  return $mMonthName;
}

/**
  Return 2014
*/
function getYearNameArray() {
  for ($i=0; $i<12; $i++) {
    $mMonthNum = date('m');
    $mYearNum[$i] = date('Y');
    
    $mMonthNum = $mMonthNum - $i;
    if ($mMonthNum <= 0) $mYearNum[$i] = $mYearNum[$i] - 1;
    
  }
  return $mYearNum;
}

function jsMonthName($monthName) {
  $monthPrint = implode("','",$monthName);  
}

?>

<?php function showResult($month, $year, $totalCashIn, $totalCashOut, $totalProfit, $totalBuyCash, $totalBuyCredit, $totalSellCash, $totalSellCredit) { ?>
<!-- Filtering -->
<div class="panel panel-primary">
  <!-- Header -->
  <div class="panel-heading">
    <h3 class="panel-title">Summary Report on <?php print $month . ' ' . $year; ?></h3>
  </div>
  <div class="panel-body">            
    <div class="row">

      <div class="col-lg-4">
        <label>Omzet</label>
        <div class="form-group input-group">
          <span class="input-group-addon">Rp</span>
          <input type="text" class="form-control" value="<?php print number_format($totalCashIn); ?>" id="total_asset" name="total_asset" readonly>
          <span class="input-group-addon">.00</span>
        </div>
      </div>
      
      <div class="col-lg-4">
        <label>Pengeluaran</label>
        <div class="form-group input-group">
          <span class="input-group-addon">Rp</span>
          <input type="text" class="form-control" value="<?php print number_format($totalCashOut); ?>" id="total_asset" name="total_asset" readonly>
          <span class="input-group-addon">.00</span>
        </div>
      </div>

      <div class="col-lg-4">
        <label>Profit</label>
        <div class="form-group input-group">
          <span class="input-group-addon">Rp</span>
          <input type="text" class="form-control" value="<?php print number_format($totalProfit); ?>" id="total_asset" name="total_asset" readonly>
          <span class="input-group-addon">.00</span>
        </div>
      </div>
      
    </div>

    <div class="row">

      <div class="col-lg-3">
        <label>Pembelian Cash</label>
        <div class="form-group input-group">
          <span class="input-group-addon">Rp</span>
          <input type="text" class="form-control" value="<?php print number_format($totalBuyCash); ?>" id="total_asset" name="total_asset" readonly>
          <span class="input-group-addon">.00</span>
        </div>
      </div>
      
      <div class="col-lg-3">
        <label>Pembelian Credit</label>
        <div class="form-group input-group">
          <span class="input-group-addon">Rp</span>
          <input type="text" class="form-control" value="<?php print number_format($totalBuyCredit); ?>" id="total_asset" name="total_asset" readonly>
          <span class="input-group-addon">.00</span>
        </div>
      </div>

      <div class="col-lg-3">
        <label>Penjualan Cash</label>
        <div class="form-group input-group">
          <span class="input-group-addon">Rp</span>
          <input type="text" class="form-control" value="<?php print number_format($totalSellCash); ?>" id="total_asset" name="total_asset" readonly>
          <span class="input-group-addon">.00</span>
        </div>
      </div>
      
      <div class="col-lg-3">
        <label>Penjualan Credit</label>
        <div class="form-group input-group">
          <span class="input-group-addon">Rp</span>
          <input type="text" class="form-control" value="<?php print number_format($totalSellCredit); ?>" id="total_asset" name="total_asset" readonly>
          <span class="input-group-addon">.00</span>
        </div>
      </div>
      
    </div>
        
  </div>
</div>
<!-- End Filtering -->
<?php } ?>

<?php function showMorrisChartArea($id, $title) {?>
<div class="panel panel-primary">
  <div class="panel-heading">
    <h3 class="panel-title"><i class="fa fa-bar-chart-o"></i> <?php print $title; ?></h3>
  </div>
  <div class="panel-body">
    <div id="<?php print $id; ?>"></div>
  </div>
</div>
<?php } ?>


<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Dashboard</title>

    <!-- Bootstrap core CSS -->
    <link href="<?php print $root ?>/css/bootstrap.css" rel="stylesheet">

    <!-- Add custom CSS here -->
    <link href="<?php print $root ?>/css/sb-admin.css" rel="stylesheet">
    <link rel="stylesheet" href="<?php print $root ?>/font-awesome/css/font-awesome.min.css">
    <link rel="stylesheet" href="<?php print $root ?>/css/morris-0.4.3.min.css">
    <!-- Page Specific CSS -->    
  </head>

  <body>

    <div id="wrapper">

      <?php include "sidebar.php";?>

      <div id="page-wrapper">
        <div class="row">
          <?php showResult($monthNameArray[1], $yearNameArray[1], $totalCashIn[1], $totalCashOut[1], $totalProfit[1], $totalBuyCash[1], $totalBuyCredit[1], $totalSellCash[1], $totalSellCredit[1]); ?>
          <?php showResult($monthNameArray[0], $yearNameArray[0], $totalCashIn[0], $totalCashOut[0], $totalProfit[0], $totalBuyCash[0], $totalBuyCredit[0], $totalSellCash[0], $totalSellCredit[0]); ?>
          <?php showMorrisChartArea("chart-omzet", "Graph - Omzet"); ?>
          <?php showMorrisChartArea("chart-profit", "Graph - Profit"); ?>
        </div>
      </div><!-- /#page-wrapper -->

    </div><!-- /#wrapper -->


<!-- JavaScript -->
<script src="<?php print $root ?>/js/jquery-1.10.2.js"></script>
<script src="<?php print $root ?>/js/bootstrap.js"></script>

<!-- Page Specific Plugins --> 
<script src="<?php print $root ?>/js/morris/raphael-min.js"></script>
<script src="<?php print $root ?>/js/morris/morris-0.4.3.min.js"></script>
<!--
<script src="<?php print $root ?>/js/morris/chart-data-morris.js"></script>
<script src="<?php print $root ?>/js/tablesorter/jquery.tablesorter.js"></script>
<script src="<?php print $root ?>/js/tablesorter/tables.js"></script>

<!--
<script src="<?php print $root ?>/js/flot/jquery.flot.js"></script>
<script src="<?php print $root ?>/js/flot/jquery.flot.resize.js"></script>
<script src="<?php print $root ?>/js/flot/jquery.flot.tooltip.min.js"></script>
<script src="<?php print $root ?>/js/flot/jquery.flot.pie.js"></script>
<script src="<?php print $root ?>/js/flot/chart-data-flot.js"></script>
-->

<script type="text/javascript">
Morris.Area({
  // ID of the element in which to draw the chart.
  element: 'chart-profit',
  // Chart data records -- each entry in this array corresponds to a point on
  // the chart.
  data: [
	{ d: '<?php print $jsDateEnd[11]; ?>', visits: <?php print round($totalProfit[11]); ?> },
	{ d: '<?php print $jsDateEnd[10]; ?>', visits: <?php print round($totalProfit[10]); ?> },
	{ d: '<?php print $jsDateEnd[9]; ?>', visits: <?php print round($totalProfit[9]); ?> },
	{ d: '<?php print $jsDateEnd[8]; ?>', visits: <?php print round($totalProfit[8]); ?> },
	{ d: '<?php print $jsDateEnd[7]; ?>', visits: <?php print round($totalProfit[7]); ?> },
	{ d: '<?php print $jsDateEnd[6]; ?>', visits: <?php print round($totalProfit[6]); ?> },
	{ d: '<?php print $jsDateEnd[5]; ?>', visits: <?php print round($totalProfit[5]); ?> },
	{ d: '<?php print $jsDateEnd[4]; ?>', visits: <?php print round($totalProfit[4]); ?> },
	{ d: '<?php print $jsDateEnd[3]; ?>', visits: <?php print round($totalProfit[3]); ?> },
	{ d: '<?php print $jsDateEnd[2]; ?>', visits: <?php print round($totalProfit[2]); ?> },
	{ d: '<?php print $jsDateEnd[1]; ?>', visits: <?php print round($totalProfit[1]); ?> },
	{ d: '<?php print $jsDateEnd[0]; ?>', visits: <?php print round($totalProfit[0]); ?> },
  ],
  // The name of the data record attribute that contains x-visitss.
  xkey: 'd',
  // A list of names of data record attributes that contain y-visitss.
  ykeys: ['visits'],
  // Labels for the ykeys -- will be displayed when you hover over the
  // chart.
  labels: ['Profit (Rp)'],
  // Disables line smoothing
  smooth: true,
});

Morris.Area({
  // ID of the element in which to draw the chart.
  element: 'chart-omzet',
  // Chart data records -- each entry in this array corresponds to a point on
  // the chart.
  data: [
	{ d: '<?php print $jsDateEnd[11]; ?>', visits: <?php print round($totalCashIn[11]); ?> },
	{ d: '<?php print $jsDateEnd[10]; ?>', visits: <?php print round($totalCashIn[10]); ?> },
	{ d: '<?php print $jsDateEnd[9]; ?>', visits: <?php print round($totalCashIn[9]); ?> },
	{ d: '<?php print $jsDateEnd[8]; ?>', visits: <?php print round($totalCashIn[8]); ?> },
	{ d: '<?php print $jsDateEnd[7]; ?>', visits: <?php print round($totalCashIn[7]); ?> },
	{ d: '<?php print $jsDateEnd[6]; ?>', visits: <?php print round($totalCashIn[6]); ?> },
	{ d: '<?php print $jsDateEnd[5]; ?>', visits: <?php print round($totalCashIn[5]); ?> },
	{ d: '<?php print $jsDateEnd[4]; ?>', visits: <?php print round($totalCashIn[4]); ?> },
	{ d: '<?php print $jsDateEnd[3]; ?>', visits: <?php print round($totalCashIn[3]); ?> },
	{ d: '<?php print $jsDateEnd[2]; ?>', visits: <?php print round($totalCashIn[2]); ?> },
	{ d: '<?php print $jsDateEnd[1]; ?>', visits: <?php print round($totalCashIn[1]); ?> },
	{ d: '<?php print $jsDateEnd[0]; ?>', visits: <?php print round($totalCashIn[0]); ?> },
  ],
  // The name of the data record attribute that contains x-visitss.
  xkey: 'd',
  // A list of names of data record attributes that contain y-visitss.
  ykeys: ['visits'],
  // Labels for the ykeys -- will be displayed when you hover over the
  // chart.
  labels: ['Omzet (Rp)'],
  // Disables line smoothing
  smooth: true,
});
//*/
</script>

  </body>
</html>
