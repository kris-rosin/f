<?php require_once "config.php";?>
<?php require_once "database.php";?>
<?php require_once "privileges.php";?>
<?php 

$database = new Database($DB_HOST, $DB_USER, $DB_PASS);
$database->connect_db();
$database->choose_database_name($DB_NAME);

// Get List Category
$sql = "SELECT * FROM game;";  
$query = sprintf($sql);  
$game_result = $database->query($query);
  
//Checking the Session
if (empty($_SESSION['username'])) {
  header("Location: $root/login.php");
  exit();
}

$database->close_db();
?>
<nav class="navbar navbar-inverse navbar-fixed-top" role="navigation">
  <!-- Brand and toggle get grouped for better mobile display -->
  <div class="navbar-header">
    <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-ex1-collapse">
      <span class="sr-only">Toggle navigation</span>
      <span class="icon-bar"></span>
      <span class="icon-bar"></span>
      <span class="icon-bar"></span>
    </button>
    <a class="navbar-brand" href="dashboard.php"><?php print $name_workspace; ?></a>
  </div>

  <!-- Collect the nav links, forms, and other content for toggling -->
  <div class="collapse navbar-collapse navbar-ex1-collapse">
    <ul class="nav navbar-nav side-nav">
      <li class="">
      	<a href="<?php print $root ?>/dashboard.php"><i class="fa fa-home"></i> Home</a>
      </li>
      <li class="">
        <a href="<?php print $root ?>/page-users/list-users.php"><i class="fa fa-users"></i> Users</a>
      </li>
      <li class="">
        <a href="<?php print $root ?>/page-game/list-game.php"><i class="fa fa-gamepad"></i> Casino Game</a>
      </li>
<!-- 
      <li class="dropdown">
        <a href="#" class="dropdown-toggle" data-toggle="dropdown"><i class="fa fa-history"></i> History <b class="caret"></b></a>
        <ul class="dropdown-menu">
        	<?php foreach ($game_result as $object): ?>
          <li><a href="<?php print $root ?>/page-history/list-history.php?gid=<?php print $object->id ?>"><?php print $object->game ?></a></li>
          <?php endforeach ?>
        </ul>        
      </li>
 -->
      <li class="">
        <a href="<?php print $root ?>/page-api/test-api.php"><i class="fa fa-book"></i> API</a>
      </li>

      <li class="dropdown">
        <a href="#" class="dropdown-toggle" data-toggle="dropdown"><i class="fa fa-trophy"></i> Leaderboard <b class="caret"></b></a>
        <ul class="dropdown-menu">
        	<?php foreach ($game_result as $object): ?>
          <li><a href="<?php print $root ?>/page-leaderboard/list-leaderboard.php?gid=<?php print $object->id ?>"><?php print $object->game ?></a></li>
          <?php endforeach ?>
        </ul>
      </li>      
      <li class="">
        <a href="<?php print $root ?>/page-admin/list-admin.php"><i class="fa fa-user"></i> Admin</a>
      </li> 
   
      
    </ul>

    <ul class="nav navbar-nav navbar-right navbar-user">
      <li class="dropdown user-dropdown">
        <a href="#" class="dropdown-toggle" data-toggle="dropdown"><i class="fa fa-user"></i> <?php print $_SESSION['username'];?> <b class="caret"></b></a>
        <ul class="dropdown-menu">
          <li><a href="#"><i class="fa fa-user"></i> Profile</a></li>
          <li><a href="#"><i class="fa fa-envelope"></i> Inbox <span class="badge">7</span></a></li>
          <li><a href="#"><i class="fa fa-gear"></i> Settings</a></li>
          <li class="divider"></li>
          <li><a href="<?php print $root;?>/logout.php"><i class="fa fa-power-off"></i> Log Out</a></li>
        </ul>
      </li>
    </ul>
  </div><!-- /.navbar-collapse -->
</nav>