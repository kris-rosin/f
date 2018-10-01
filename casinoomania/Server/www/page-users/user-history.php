<?php require_once "../config.php" ?>
<?php require_once "../database.php" ?>
<?php require_once "../privileges.php" ?>
<?php 
$is_success = false;
$is_failed = false;

$database = new Database($DB_HOST, $DB_USER, $DB_PASS);
$database->connect_db();
$database->choose_database_name($DB_NAME);

$game_id = $_REQUEST['gid'];
$user_id = $_REQUEST['uid'];
$page = $_REQUEST['page'];
$search = $_REQUEST['search_username'];
$per_page = 4;
$is_gid_defined = false;

if (!empty($game_id) && !empty($user_id) ) {
	$sql = "
	SELECT 
		history.id as hid,
		users.id, 
		users.username, 
		history.bet, 
		history.win, 
		history.diff, 
		history.time_created 
	FROM 
		history
	INNER JOIN
		users
	ON 
		history.uid = users.id 
	WHERE 
		history.gid=%d AND history.uid=%d";
	if (!empty($search)) {
		$sql .= " AND users.username LIKE '%%%s%%'";
	}
	
	if (empty($page) || $page == 1) {
		$page = 0;
	} else {
		$page = $page-1;
	}
	$start = $page * $per_page;
	$sql .= " ORDER BY history.time_created DESC LIMIT $start,$per_page;";
	
	
 	$query = sprintf($sql, $game_id, $user_id, $search);
	$result = $database->query($query);
	$is_gid_defined = true;
	
	$sql = "SELECT * FROM game WHERE id=%d";
	$query = sprintf($sql, $game_id);
	$result2 = $database->query($query);
	foreach($result2 as $object2){
		$history_name = $object2->game;
	}
	
	$sql = "SELECT * FROM users WHERE id=%d";
	$query = sprintf($sql, $user_id);
	$result3 = $database->query($query);
	foreach ($result3 as $object3) {
		$username = $object3->username;
		break;
	}
	
	
} else {
	$is_gid_defined = false;
}
    
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

    <title>History</title>

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
      
        <?php $isUserAllowed = $is_gid_defined; // function from privileges.php?>
        
        <?php if($isUserAllowed): ?>
        
        <div class="row">
          <div class="col-lg-12">
            <h1>History <?php print $history_name?> - <a href="<?php print $root ?>/page-users/edit_users.php?id=<?php print $user_id; ?>"><?php print $username; ?></a></h1>
            <ol class="breadcrumb">
              <li><i class="fa fa-history"></i> History <?php print $history_name?></a></li>
              <li class="active"><a href="<?php print $root ?>/page-users/edit_users.php?id=<?php print $user_id; ?>"><?php print $username; ?></a></li>
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
      
        <div class="row">
          <div class="col-lg-12">              
            <div class="table-responsive">
              <table class="table table-bordered table-hover tablesorter">
                <thead>
                  <tr>
                    <th class="header" width="1">No </i></th>
                    <th class="header">Bet </i></th>
                    <th class="header">Win </i></th>
                    <th class="header">Diff </i></th>
                    <th class="header">Last Created </i></th>
                    <!--<th class="header">Writer <i class="fa fa-sort"></i></th>-->
                  </tr>
                </thead>
                <tbody>
                  <?php $num = 0; ?>
                  <?php foreach($result as $object): ?>
                  <?php $num++; ?>
                  <tr>
                    <td style="text-align: center"><?php print $num + $page * $per_page;?></td>
                    <td><?php print $object->bet;?></td>
                    <td><?php print $object->win;?></td>
                    <td><?php print $object->diff;?></td>
                    <td><?php print $object->time_created;?></td>
                  </tr>
                  <?php endforeach; ?>
                </tbody>
              </table>
            </div>
          </div>      
        </div>
        
        <?php $current_page = $page+1;?>
        <ul class="pagination">
				  <li><a href="<?php print $root ?>/page-users/user-history.php?uid=<?php print $game_id; ?>&gid=<?php print $user_id;?>&page=<?php print ($current_page-1);?>"><< prev</a></li>
				  <li><a href="#"><?php print $current_page;?></a></li>
				  <?php if ($num >= $per_page) : ?>
				  <li><a href="<?php print $root ?>/page-users/user-history.php?uid=<?php print $game_id; ?>&gid=<?php print $user_id;?>&page=<?php print ($current_page+1);?>">next >></a></li>
				  <?php endif; ?>
				</ul>
        
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
