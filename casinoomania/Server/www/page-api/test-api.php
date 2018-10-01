<?php require_once "../config.php" ?>
<?php require_once "../database.php" ?>
<?php require_once "../privileges.php" ?>
<?php 
$is_success = false;
$is_failed = false;

$database = new Database($DB_HOST, $DB_USER, $DB_PASS);
$database->connect_db();
$database->choose_database_name($DB_NAME);

$key1 = $_REQUEST['key1'];
$key2 = $_REQUEST['key2'];


// $client_md5_string = $_REQUEST['md5_string'];
// 
// $server_md5_string = md5("reg_id=".$reg_id."&from=".$from.$SECRET_KEY);
// 
// if (!empty($reg_id) && !empty($from) && strcmp($client_md5_string, $server_md5_string) == 0) {

//http://www.casinoomania.dev/page-api/test-api.php?username=hammy&facebookid=1&bank=bca&md5_string=a7c55fa63e50b4390058bacaccc58142
//http://www.casinoomania.dev/page-api/test-api.php?action=get_user&id=1&md5_string=a7c55fa63e50b4390058bacaccc58142

$mRequest = $_GET;
ksort($mRequest);
$mString = "";
foreach($mRequest as $key => $value) {
	if (strcmp($key,"md5_string") != 0) {
		if ($num == 0) {
			$mString .= $key."=".$value;
		} else {
			$mString .= "&".$key."=".$value;
		}
		$num++;
	}
}
$mString .= $SECRET_KEY;
$server_md5_string = md5($mString);
$textarea = $mString."\n\n".$server_md5_string;

    
$database->close_db();
?>
<script src="<?php print $root ?>/js/jquery-1.10.2.js"></script>
<script type="text/javascript">
  function confirm_delete(name, id) {
    var r = confirm("Delete " + name + "?");
    if (r == true) {
      hostAddress = top.location.host.toString();
      url = "<?php print $root; ?>/page-admin/list-admin.php?delete_id="+id+"&name="+name;
      window.location = url;
    } else {
    }
  }
  
  function edit(id) {
    hostAddress = top.location.host.toString();
    url = "<?php print $root; ?>/page-admin/edit-admin.php?id="+id;
    window.location = url;
  }

  function add_page() {
    hostAddress = top.location.host.toString();
    url = "<?php print $root; ?>/page-admin/create-admin.php";
    window.location = url;    
  }  
</script>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Leaderboard</title>

    <!-- Bootstrap core CSS -->
    <link href="<?php print $root ?>/css/bootstrap.css" rel="stylesheet">

    <!-- Add custom CSS here -->
    <link href="<?php print $root ?>/css/sb-admin.css" rel="stylesheet">
    <link rel="stylesheet" href="<?php print $root ?>/font-awesome/css/font-awesome.min.css">
    <!-- Page Specific CSS -->    
  </head>

  <body>

    <div id="wrapper">

      <?php include "../sidebar.php";?>

      <div id="page-wrapper">
      
        <?php $isUserAllowed = true; // function from privileges.php?>
        
        <?php if($isUserAllowed): ?>
        
        <div class="row">
          <div class="col-lg-12">
            <h1>API - Test API</h1>
            <ol class="breadcrumb">
              <li><i class="fa fa-book"></i> API</a></li>
              <li class="active">Test API</li>
            </ol>
          </div>
        </div><!-- /.row -->

        <?php if ($is_success == true): ?>
        <div class="alert alert-dismissable alert-success">
          <button type="button" class="close" data-dismiss="alert">×</button>
          <?php print $message; ?>
        </div>
        <?php endif; ?>
        
        <?php if ($is_failed == true): ?>
        <div class="alert alert-dismissable alert-danger">
          <button type="button" class="close" data-dismiss="alert">×</button>
          <?php print $message;?>
        </div>
        <?php endif; ?>
            
        <form action="test-api.php" method="post" enctype="multipart/form-data">
          <div class="row">
          
	          <form role="form">
	            <div class="col-lg-12">	            

<!-- 
                <div class="form-group">
                  <label>Key1 </label>
                  <input class="form-control" name="key1" size="50" value="<?php print $key1 ?>">
                </div>            
             	 	<div class="form-group">
               		<label>Key2 </label>
                	<input class="form-control" name="key2" size="50" value="<?php print $key2 ?>">
              	</div>
              	<input type="submit" class="btn btn-primary" name="md5" value="md5">            
	          	</div>
	          	<div class="col-lg-2">
              	<div class="form-group">
               		<label>Val1 </label>
                	<input class="form-control" name="val1" size="50" value="<?php print $val1 ?>">
              	</div>            
              	<div class="form-group">
                	<label>Val2 </label>
                	<input class="form-control" name="val2" size="50" value="<?php print $val2 ?>">
              	</div>  
 --> 
              	
			          <div class="form-group">
    		          <label>Message</label>
        		      <textarea class="form-control" name="message" size="100" style='overflow: auto;'"><?php print $textarea; ?></textarea>
		            </div>                  
              	         
	          	</div>
	          </form>
	          

          </div>
        </form>            
        
<!-- 
        <div class="row">
          <div class="col-lg-4">
            <input type="submit" class="btn btn-primary" value="Add" onclick="add_page();">
          </div>
        </div>        
 -->
        
        <?php else: ?>
        <div class="alert alert-dismissable alert-danger">
          <button type="button" class="close" data-dismiss="alert">×</button>
          User is not authorized to see this page!
        </div>
        <?php endif; ?>        
        
      </div><!-- /#page-wrapper -->

    </div><!-- /#wrapper -->

    <!-- JavaScript -->
    <script src="<?php print $root ?>/js/jquery-1.10.2.js"></script>
    <script src="<?php print $root ?>/js/bootstrap.js"></script>

    <!-- Page Specific Plugins -->    
    <script src="<?php print $root ?>/js/morris/chart-data-morris.js"></script>
    <script src="<?php print $root ?>/js/tablesorter/jquery.tablesorter.js"></script>
    <script src="<?php print $root ?>/js/tablesorter/tables.js"></script>

  </body>
</html>
