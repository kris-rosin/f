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
$is_gid_defined = false;

if (!empty($game_id)) {
	$sql = "
	SELECT 
		users.id, 
		users.username, 
		all_time_leaderboard.bet, 
		all_time_leaderboard.win, 
		all_time_leaderboard.diff, 
		all_time_leaderboard.diff, 
		all_time_leaderboard.time_updated 
	FROM 
		all_time_leaderboard
	INNER JOIN
		users
	ON 
		all_time_leaderboard.uid = users.id 
	WHERE 
		gid=%d;";
 	$query = sprintf($sql, $game_id);
	$result = $database->query($query);
	$is_gid_defined = true;
	
	$sql = "SELECT * FROM game WHERE id=%d";
	$query = sprintf($sql, $game_id);
	$result2 = $database->query($query);
	foreach($result2 as $object){
		$leaderboard_name = $object->game;
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
      
        <?php $isUserAllowed = $is_gid_defined; // function from privileges.php?>
        
        <?php if($isUserAllowed): ?>
        
        <div class="row">
          <div class="col-lg-12">
            <h1>Leaderboard</h1>
            <ol class="breadcrumb">
              <li><i class="fa fa-trophy"></i> Leaderboard</a></li>
              <li class="active"><?php print $leaderboard_name; ?></li>
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
                    <th class="header">Username </i></th>
                    <th class="header">Bet </i></th>
                    <th class="header">Win </i></th>
                    <th class="header">Diff </i></th>
                    <th class="header">Last Updated </i></th>
                    <!--<th class="header">Writer <i class="fa fa-sort"></i></th>-->
                  </tr>
                </thead>
                <tbody>
                  <?php foreach($result as $object): ?>
                  <tr>
                    <td><a href="<?php print $root ?>/page-users/edit_users.php?id=<?php print $object->id; ?>"><?php print $object->username;?></a></td>
                    <td><?php print $object->bet;?></td>
                    <td><?php print $object->win;?></td>
                    <td><?php print $object->diff;?></td>
                    <td><?php print $object->time_updated;?></td>
                  </tr>
                  <?php endforeach; ?>
                </tbody>
              </table>
            </div>
          </div>      
        </div>
        
        <div class="row">
          <div class="col-lg-4">
            <input type="submit" class="btn btn-primary" value="Add" onclick="add_page();">
          </div>
        </div>        
        
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
